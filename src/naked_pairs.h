#pragma once
#include "sudoku.h"
typedef struct NakedPairs_impl{ 
    Cell*p_cell1;
    Cell*p_cell2;//there are two cells in Naked Pairs so we need to define 2 pointers cells
    int value[2]; // initialize the array to hold 2 value of these cells
} NakedPairs;
bool are_in_same_unit( Cell*p_cell1,Cell*p_cell2);
bool is_in_naked_pair(Cell*p_cell1,Cell*p_cell2,int pval1,int pval2);
int find_naked_pairs(Cell*p_cell1,Cell*p_cell2,int*naked_pairs);
void find_naked_pair(Cell**p_cell1,Cell**p_cell2,NakedPairs*p_naked_pairs,int *p_counter);
int naked_pairs(SudokuBoard *p_board);