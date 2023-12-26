#include "hidden_singles.h"
#include <stdio.h>
#include <assert.h>
#include<stdlib.h>
#include"sudoku.h"
#include<stdbool.h>


/* find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box) return the 
numbers of hidden single values */
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values) {
    int count = 0;
    int occurence=0 ; //count the number of apperances of these numbers in cells column and boxs
    // check all number from 1 to 9 whether hidden single values
    for (int pval = 1; pval <= BOARD_SIZE; pval++) {
        occurence= 0 ;
        // check each row
        for(int i = 0 ; i< BOARD_SIZE;i++){
            // check each number for each row whether is hidden single and whether cells have been solved 
            if(is_candidate(p_cells[i],pval)&& p_cells[i]->num_candidates>1){
                occurence++;
            }
        }  
        //occurence is one -> this is a hidden single
        if(occurence==1){
            hidden_single_values[count]=pval;
            count++;
       }
     
    }
     return count;
}
bool is_in_Hidden(HiddenSingle*p_hidden_single, int *size,Cell*p ){
    for(int i = 0 ; i<*size;i++){
        if(p_hidden_single[i].p_cell==p){
            return true;
        }
    }
    return false;
}
// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter) {
    // TEMPORARY to hold hidden _single value

    int hidden_single_values[BOARD_SIZE];
    int count = find_hidden_single_values(p_cells,hidden_single_values);
    for(int i=0 ;i< count ;i++){
            int pval=hidden_single_values[i];
            for(int j = 0 ; j< BOARD_SIZE; j++){
                
            if (is_candidate(p_cells[j],pval)&&p_cells[j]->num_candidates>1 && is_in_Hidden(p_hidden_singles,p_counter,p_cells[j])==false){
                p_hidden_singles[*p_counter].value=pval;
                p_hidden_singles[*p_counter].p_cell=p_cells[j];
                (*p_counter)++;
            }
                
        }
    }
    }

int hidden_singles(SudokuBoard *p_board) {
    
//set candidates 
int solved_counter= 0 ;
HiddenSingle hidden_singles[BOARD_SIZE*BOARD_SIZE];

    for(int i = 0 ; i< BOARD_SIZE;i++){
            find_hidden_single(p_board->p_rows[i],hidden_singles,& solved_counter);
         
       }
       for(int j = 0 ; j< BOARD_SIZE;j++){
            find_hidden_single(p_board->p_cols[j],hidden_singles,&solved_counter);
       }
       for(int k = 0 ; k< BOARD_SIZE;k++){
            find_hidden_single(p_board->p_boxes[k],hidden_singles,&solved_counter);
       }
       for(int  j = 0 ; j<solved_counter;j++){
            HiddenSingle*current_hidden=&hidden_singles[j];
        
            int value=current_hidden->value;
            set_candidates(current_hidden->p_cell, &value, 1);
            
            }
            
        //coppy hidden single to output array
        for(int k = 0 ; k < solved_counter;k++){
            p_board->solved_cells[solved_counter+k]=hidden_singles[k].p_cell;
            }
            return solved_counter ; 

        }







