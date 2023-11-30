#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"sudoku.h"
#include"hidden_singles.h"
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values){
    int counter= 0 ;
    for(int i = 0 ; i<BOARD_SIZE;i++){
        for(int j = 0 ; j <BOARD_SIZE;j++){
            if(p_cells[i]->num_candidates==1){
                int hidden_single_value = get_candidates(p_cells[i]);
                hidden_single_values[counter++]=hidden_single_value;

            }
        }
    }
    // return the total single value
    return counter;
}
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles,
                        int *p_counter){
    for(int i = 0 ; i< BOARD_SIZE;i++){
        if(p_cells[i]->num_candidates=1){
          HiddenSingle*hidden_single;
            hidden_single.p_cell=p_cells[i];
            hidden_single.value=get_candidates(p_cells[i]);
            p_hidden_singles[i]=hidden_single;
            (*p_counter)++;
        }
    }
                        }
int hidden_singles(SudokuBoard *p_board, Cell ***p_solved_cells){
    int solved_counter= 0 ;
    //solved each row
    for (int i = 0 ; i, BOARD_SIZE; i++){
        solved_counter+= find_hidden_single_values(p_board->p_rows[i],*(*p_solved_cells+solved_counter));
    }
    //solved each column
    for(int j =0 ; j< BOARD_SIZE;j++){
        solved_counter+=find_hidden_single_values(p_board->p_cols[j],*(*p_solved_cells+solved_counter));
    }
    // solved_each_box
    for(int i = 0 ;i<BOARD_SIZE;i++ ){
        solved_counter+=find_hidden_single_values(p_board->p_boxes[i],*(*p_solved_cells+ solved_counter));
    }
    return solved_counter;
}


