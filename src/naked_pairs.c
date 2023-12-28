#include "naked_pairs.h"
#include "sudoku.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool are_in_same_unit(Cell *p_cell1, Cell *p_cell2) {
    return (p_cell1->row_index == p_cell2->row_index ||
            p_cell1->col_index == p_cell2->col_index ||
            p_cell1->box_index == p_cell2->box_index);
}



int find_naked_pairs(Cell **p_cells, int *naked_pairs) {
    int count = 0;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = i + 1; j < BOARD_SIZE; j++)
        {
            if(p_cells[i]->num_candidates == 2 && p_cells[j]->num_candidates == 2){
            int *candidates1 = get_candidates(p_cells[i]);
            int *candidates2 = get_candidates(p_cells[j]);
            if(candidates1[0] == candidates2[0] && candidates1[1] == candidates2[1]){
                naked_pairs[count] = candidates1[0];
                count++;
                naked_pairs[count] = candidates1[1];
                count++;
            }

            }
        }
    }
    return count;
}
void find_naked_pair(Cell **p_cells, NakedPairs *p_naked_pairs, int *p_counter) {
    int naked_pair[BOARD_SIZE * 2];
    int count = find_naked_pairs(p_cells, naked_pair);
    for (int i = 0; i < count;i += 2) {
        int pval1 = naked_pair[i];
        int pval2 = naked_pair[i + 1];
        for (int j = 0; j < BOARD_SIZE; j++) {
           for(int  k = j+1; k<BOARD_SIZE ; k++){
            if(is_candidate(p_cells[j],pval1)&&is_candidate(p_cells[j],pval2)&&is_candidate(p_cells[i],pval1)&& is_candidate(p_cells[i],pval2))
           
           {
                p_naked_pairs[*p_counter].p_cell1 = p_cells[j];
                p_naked_pairs[*p_counter].p_cell2 = p_cells[j];
                p_naked_pairs[*p_counter].value[0] = pval1;
                p_naked_pairs[*p_counter].value[1] = pval2;
                (*p_counter)++;
            }
        }
       
        }
    } 
        NakedPairs*current_pairs= p_naked_pairs;
        int pval1=current_pairs->value[0];
        int pval2= current_pairs->value[1];
    
        // we gonna unset all elements have the same things like this same in units and have the num elememt is greater than 2
        for(int i = 0 ; i< BOARD_SIZE;i++){
            for(int j = 0 ; j <p_cells[i]->num_candidates;j++){
           if (p_cells[i]->num_candidates > 2 &&
            ((p_cells[i]->candidates[j] == pval1 || p_cells[i]->candidates[j] == pval2) &&
            are_in_same_unit(p_cells[i], p_naked_pairs[*p_counter].p_cell1))){
                unset_candidate(p_cells[i],pval1);
                unset_candidate(p_cells[i],pval2);
                }
            }
        }
        }



int naked_pairs(SudokuBoard *p_board) {
    int solved_counter = 0;
    NakedPairs temp [BOARD_SIZE*BOARD_SIZE];
    // Find naked pairs each column
    for (int i = 0; i < BOARD_SIZE; i++) {
            find_naked_pair(p_board->p_cols[i], temp, &solved_counter);
    }

    // Find naked pairs each row
    for (int i = 0; i < BOARD_SIZE; i++) {
            find_naked_pair(p_board->p_rows[i], temp, &solved_counter);
        }

    // Find naked pairs each box
    for (int i = 0; i < BOARD_SIZE; i++) {
       
        find_naked_pair(p_board->p_boxes[i], temp, &solved_counter);
    }
return 0 ;
}

 /*   for (int j = 0; j < solved_counter; j++) {
        NakedPairs *current_pair = &temp[j];
        int value1 = current_pair->value[0];
        int value2 = current_pair->value[1];

        for (int k = 0; k < 2; k++) {
            current_cell = (k == 0) ? current_pair->p_cell1 : current_pair->p_cell2;
        
 unset the candidates number
        for (int pval = 1; pval <= 9; pval++) {
            if (pval != current_pair->value[0] && pval != current_pair->value[1]) {
                unset_candidate(current_cell, pval);
            }
        }
         if (current_cell->num_candidates == 1) {
            int value = current_cell->candidates[0];
            set_candidates(current_cell, &value, 1);
        }
    }

        }   
       return 0;
    }
*/