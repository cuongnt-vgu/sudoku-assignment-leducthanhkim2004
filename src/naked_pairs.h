#pragma once

#include "sudoku.h"

typedef struct NakedPair_impl {
    Cell *p_cell_1;
    Cell *p_cell_2;
    Cell **p_cells;
    int value1;
    int value2;
} NakedPair;


int is_naked_pair(Cell **p_cells, int value1, int value2);

bool is_in_naked_pair_cell(NakedPair *p_array, Cell *p);

int is_naked_cell(Cell *p_cell, int value1, int value2);

int find_naked_pair_values(Cell **p_cells, int *naked_pair_values);

void find_naked_pair(Cell **p_cells, NakedPair *p_naked_pair, int *p_counter);

int naked_pairs(SudokuBoard *p_board);
