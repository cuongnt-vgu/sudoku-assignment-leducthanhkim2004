#include "naked_pairs.h"
#include "sudoku.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool are_in_same_unit(Cell *p_cell1, Cell *p_cell2) {
    return (p_cell1->row_index == p_cell2->row_index &&
            p_cell1->col_index == p_cell2->col_index &&
            p_cell1->box_index == p_cell2->box_index);
}

int find_naked_pairs(Cell *p_cell1, Cell *p_cell2, int *naked_pairs) {
    int count = 0;
    int *candidates1 = get_candidates(p_cell1);// return  array of numbers of candidates cell1
    int *candidates2 = get_candidates(p_cell2);// return array of numbers of candidates cell2
    for (int i = 0; i < p_cell1->num_candidates; i++) {
        for (int j = 0; j < p_cell2->num_candidates; j++) {
            int pval1 = candidates1[i];
            int pval2 = candidates2[j];

            if (are_in_same_unit(p_cell1, p_cell2) &&
                p_cell1->num_candidates == 2 &&
                p_cell2->num_candidates == 2 &&
                p_cell1->candidates[pval1 - 1] == p_cell2->candidates[pval2 - 1]) {
                naked_pairs[count++] = pval1;
                naked_pairs[count++] = pval2;
              
            }
        }
    }

    free(candidates1);
    free(candidates2);
    return count/2 ;
}

bool is_in_naked_pair(Cell *p_cell1, Cell *p_cell2, int pval1, int pval2) {
    int count = 0;

    for (int i = 1; i <= BOARD_SIZE; i++) {
        if (i != pval1 && i != pval2) {
            if (is_candidate(p_cell1, i) || is_candidate(p_cell2, i)) {
                // If a candidate is found outside the naked pair, return false
                return false;
            }
        } else {
            // Count the occurrences of pval1 and pval2 in the naked pair
            if (is_candidate(p_cell1, i)) {
                count++;
            }
            if (is_candidate(p_cell2, i)) {
                count++;
            }
        }
    }

    // Return true only if there are exactly two occurrences of pval1 and pval2 combined
    return count == 2;
}

void find_naked_pair(Cell **p_cell1, Cell **p_cell2, NakedPairs *p_naked_pairs, int *p_counter) {
    int naked_pair[BOARD_SIZE * 2];
    int count = find_naked_pairs(p_cell1[0], p_cell2[0], naked_pair);

    for (int i = 0; i < count; i += 2) {
        int pval1 = naked_pair[i];
        int pval2 = naked_pair[i + 1];

        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_candidate(p_cell1[j], pval1) && is_candidate(p_cell2[j], pval2) &&
                is_in_naked_pair(p_cell1[j], p_cell2[j], pval1, pval2)) {
                p_naked_pairs[*p_counter].p_cell1 = p_cell1[j];
                p_naked_pairs[*p_counter].p_cell2 = p_cell2[j];
                p_naked_pairs[*p_counter].value[0] = pval1;
                p_naked_pairs[*p_counter].value[1] = pval2;
                (*p_counter)++;
            }
        }
    }
}

int naked_pairs(SudokuBoard *p_board) {
    int solved_counter;
    NakedPairs temp[BOARD_SIZE * BOARD_SIZE];
    
    // Find naked pairs each column
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = i + 1; j < BOARD_SIZE - 1; j++) {
            
            find_naked_pair(p_board->p_cols[i], p_board->p_cols[j], temp, &solved_counter);
        }
    }

    // Find naked pairs each row
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = i + 1; j < BOARD_SIZE - 1; j++) {
            
            find_naked_pair(p_board->p_rows[i], p_board->p_rows[j], temp, &solved_counter);
        }
    }

    // Find naked pairs each box
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = i + 1; j < BOARD_SIZE; j++) {
       
        find_naked_pair(p_board->p_boxes[i], p_board->p_boxes[j], temp, &solved_counter);
    }

    }

    for (int j = 0; j < solved_counter; j++) {
        NakedPairs *current_pair = &temp[j];
        Cell *current_cell;

        for (int k = 0; k < 2; k++) {
            current_cell = (k == 0) ? current_pair->p_cell1 : current_pair->p_cell2;
        

        for (int pval = 1; pval <= 9; pval++) {
            if (pval != current_pair->value[0] && pval != current_pair->value[1]) {
                unset_candidate(current_cell, pval);
            }
        }

        }   
    
    }

    return 0;
}
