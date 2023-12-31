// #include "naked_pairs.h"

// int naked_pairs(SudokuBoard *p_board)
// {
//     return 0;
// }

#include "naked_pairs.h"
#include "sudoku.h"
#include <stdlib.h>
#include <stdio.h>
int naked_cell_array[BOARD_SIZE];
int is_naked_pair(Cell **p_cells, int value1, int value2){
    int count = 0;
    for (int i =0; i < BOARD_SIZE; i++){
        if (p_cells[i]->num_candidates == 2){
            if(is_candidate(p_cells[i], value1) && is_candidate(p_cells[i], value2)){
            count ++;
            }
        }
    }
    return (count ==2);
}

bool is_in_naked_pair_cell(NakedPair *p_array, Cell *p){
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++){
        if ((p_array[i].p_cell_1 == p)||(p_array[i].p_cell_2 == p)){
            return true;
        }
    }
    return false;
}

int is_naked_cell(Cell *p_cell, int value1, int value2){
    if (p_cell->num_candidates == 2){
        return (is_candidate(p_cell, value1) && is_candidate(p_cell, value2));
    }
    return 0;
}

int find_naked_pair_values(Cell **p_cells, int *naked_pair_values)
{
    int naked_pair_count = 0;
    int naked_pair_array[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++){
        naked_pair_array[i] = 0;
    }
    for (int i = 0; i < BOARD_SIZE; i++){
        if (p_cells[i]->num_candidates >= 2){
            int* candidate_array = get_candidates(p_cells[i]); 
            for (int j = 0; j < p_cells[i]->num_candidates; j++){
                naked_pair_array[candidate_array[j]-1] += 1; 
            }
            free(candidate_array);
        }
    }
    for (int i = 0; i < BOARD_SIZE; i++){
        if (naked_pair_array[i] >= 2){
            naked_pair_values[naked_pair_count++] = i+1;                  
        }
    }
    return naked_pair_count;
}

void find_naked_pair(Cell **p_cells, NakedPair *p_naked_pair, int *p_counter)
{       
    int naked_pair_value_array[BOARD_SIZE];
    int num_value = find_naked_pair_values(p_cells, naked_pair_value_array);

    for (int i = 0; i < num_value-1; i++){
        for (int j = i+1; j < num_value; j++){
            if(is_naked_pair(p_cells, naked_pair_value_array[i], naked_pair_value_array[j])){
                    for (int k = 0; k < BOARD_SIZE-1; k++){
                        for (int l = k+1; l < BOARD_SIZE; l++){
                            if (is_naked_cell(p_cells[k], naked_pair_value_array[i], naked_pair_value_array[j]) && is_naked_cell(p_cells[l], naked_pair_value_array[i], naked_pair_value_array[j])){
                                NakedPair naked_pair_object;
                                naked_pair_object.p_cell_1 = p_cells[k];
                                naked_pair_object.p_cell_2 = p_cells[l];
                                naked_pair_object.p_cells = p_cells;
                                naked_pair_object.value1 = naked_pair_value_array[i];
                                naked_pair_object.value2 = naked_pair_value_array[j];
                                p_naked_pair[(*p_counter)++] = naked_pair_object;
                            }
                        }                
                    }
            }
        }   
    }
}

int naked_pairs(SudokuBoard *p_board)
{
    
    NakedPair p_naked_pair[BOARD_SIZE*BOARD_SIZE];
    int p_counter = 0;
    for (int i = 0; i< BOARD_SIZE; i++){
        find_naked_pair(p_board->p_cols[i], p_naked_pair, &p_counter);
        find_naked_pair(p_board->p_rows[i], p_naked_pair, &p_counter);
        find_naked_pair(p_board->p_boxes[i], p_naked_pair, &p_counter);
    }
    int repeated = 0;
    NakedPair solved_cell[BOARD_SIZE*BOARD_SIZE];
    int solved_cell_count = 0;
    for (int i = 0; i < p_counter; i++){
        Cell **p_cells = p_naked_pair[i].p_cells;
        if (is_in_naked_pair_cell(solved_cell, p_naked_pair[i].p_cell_1))
        {
            repeated += 1;
            continue;
        }
        solved_cell[solved_cell_count++] = p_naked_pair[i];
        for (int k = 0; k < BOARD_SIZE; k++)
        {
            if (p_cells[k] == p_naked_pair[i].p_cell_1 || p_cells[k] == p_naked_pair[i].p_cell_2){
                continue; 
            }
            int* candidate_array = get_candidates(p_cells[k]);
            int num_candidates = p_cells[k]->num_candidates;
            for (int j = 0; j < num_candidates; j++){
                if ((candidate_array[j]== p_naked_pair[i].value1)){
                unset_candidate(p_cells[k], candidate_array[j]);
                }
                if ((candidate_array[j]== p_naked_pair[i].value2)){
                unset_candidate(p_cells[k], candidate_array[j]);
                }
            } 
            free(candidate_array);
        }
    }
    return p_counter - repeated;
}