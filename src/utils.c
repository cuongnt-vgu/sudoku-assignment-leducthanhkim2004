#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "sudoku.h"
// initialize the sudoku board 
void init_sudoku(SudokuBoard *p_board)
{
    p_board->data = malloc(BOARD_SIZE * sizeof(Cell *));
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        p_board->data[i] = malloc(BOARD_SIZE * sizeof(Cell));
        p_board->p_rows[i] = malloc(BOARD_SIZE * sizeof(Cell *));
        p_board->p_cols[i] = malloc(BOARD_SIZE * sizeof(Cell *));
        p_board->p_boxes[i] = malloc(BOARD_SIZE * sizeof(Cell *));
    }

    // assign rows, cols, boxes constraint
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            p_board->p_rows[i][j] = &p_board->data[i][j];
            p_board->p_cols[j][i] = &p_board->data[i][j];
            p_board->p_boxes[(i / 3) * 3 + j / 3][(i % 3) * 3 + j % 3] =
                &p_board->data[i][j];

            // set row index and col index
            p_board->data[i][j].row_index = i;
            p_board->data[i][j].col_index = j;
            p_board->data[i][j].box_index = (i / 3) * 3 + j / 3;
        }
    }

    p_board->solved_counter = 0;
}
// free memory for each row cols boxes 
void free_sudoku(SudokuBoard *p_board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        free(p_board->data[i]);
        free(p_board->p_rows[i]);
        free(p_board->p_cols[i]);
        free(p_board->p_boxes[i]);
    }
    free(p_board->data);
}
// compare the solved counter to  the total number of the cells to check whether it has been solved
bool is_solved(SudokuBoard *p_board)
{
    return p_board->solved_counter == BOARD_SIZE * BOARD_SIZE;
}
// print the solution of the Sudoku Board 
void print_solution(SudokuBoard *p_board)
{
    assert(is_solved(p_board));

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            int *candidates = get_candidates(&p_board->data[i][j]);
            printf("%d", candidates[0]);
            free(candidates);
        }
    }
}
// set the value as a candidate for cell
void set_candidate(Cell *cell, int value)
{
    cell->candidates[value - 1] = 1;
    cell->num_candidates += 1;
}
// unsets candidates for a given cells
void unset_candidate(Cell *cell, int value)
{
    cell->candidates[value - 1] = 0;
    cell->num_candidates -= 1;
}
// check wether this number is candidates
bool is_candidate(Cell *cell, int value)
{
    return cell->candidates[value - 1] == 1;
}
//Sets a list of candidates for a cell and updates the numbers of candidates
void set_candidates(Cell *cell, int *candidates, int size)
{
    // reset candidates
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cell->candidates[i] = 0;
    }

    cell->num_candidates = 0;
    

    // set candidate list
    for (int i = 0; i < size; i++)
    {
        set_candidate(cell, candidates[i]);
    }

    if (size == 1)
    {
        cell->value = candidates[0];
    }
}
//return an array of candidates for given cells
int *get_candidates(Cell *cell)
{
    int *out = malloc(cell->num_candidates * sizeof(int));
    int counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (cell->candidates[i])
        {
            out[counter++] = i + 1;
        }
    }
    return out;
}
// loads a sudoku from string board representation
void load_sudoku(SudokuBoard *p_board, char *textData)
{
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        if (textData[i] - '0')
        {
            int candidates[] = {textData[i] - '0'};
            int num_candidates = 1;
            set_candidates(&(p_board->data[i / BOARD_SIZE][i % BOARD_SIZE]),
                           candidates, num_candidates);
        }
        else
        {
            int candidates[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            int num_candidates = BOARD_SIZE;
            set_candidates(&(p_board->data[i / BOARD_SIZE][i % BOARD_SIZE]),
                           candidates, num_candidates);
        }
    }
}
// apply a constraint to a group of cells
bool apply_constraint(Cell **p_cells, int value)
{
    bool ret = false;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates > 1)
        {
            if (is_candidate(p_cells[i], value))
            {
                unset_candidate(p_cells[i], value);
                ret = true;
            }
        }
    }
    return ret;
}
// show all possilble candidates for a group of solved cells
bool show_possible(SudokuBoard *p_board, Cell **p_solved_cells, int counter)
{
    bool ret = false;
    for (int i = 0; i < counter; i++)
    {
        Cell *solved_cell = p_solved_cells[i];
        ret |= apply_constraint(p_board->p_rows[solved_cell->row_index],
                                solved_cell->value);
        ret |= apply_constraint(p_board->p_cols[solved_cell->col_index],
                                solved_cell->value);
        ret |= apply_constraint(p_board->p_boxes[solved_cell->box_index],
                                solved_cell->value);
    }
    return ret;
}
// check the elements where in a each list
bool is_in_list(Cell **p_array, int size, Cell *p)
{
    for (int i = 0; i < size; i++)
    {
        if (p == p_array[i])
            return true;
    }
    return false;
}
// check wether has been solved
int check_solved_cells(SudokuBoard *p_board, Cell ***p_solved_cells)
{
    int counter = p_board->solved_counter;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (p_board->data[i][j].num_candidates == 1 &&
                !is_in_list(p_board->solved_cells, counter,
                            &p_board->data[i][j]))
            {
                int *candidates = get_candidates(&p_board->data[i][j]);
                p_board->data[i][j].value = candidates[0];
                free(candidates);
                p_board->solved_cells[p_board->solved_counter++] =
                    &p_board->data[i][j];
            }
        }
    }
    *p_solved_cells = &p_board->solved_cells[counter];
    return p_board->solved_counter - counter;
}
//print the candidcate number

void print_candidate_num(SudokuBoard *p_board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("%d ", p_board->data[i][j].num_candidates);
        }
        printf("\n");
    }
}
