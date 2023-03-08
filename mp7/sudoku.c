#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------
/* partners: ykko2, mamir6
First, we defined the three helper functions that check if the value is already there in a specific row, column, or 3x3.
To check for the row, we looped through all the columns while keeping the row constant. If a specific cell in the row is equal
to the given value, you return 1. To check for the column, we looped through all the rows while keeping the column constant. 
If a specific cell in the column is equal to the given value, you return 1.  Lastly, to check within the 3x3, we first locate
the specific 3x3 square and accordingly set the starting and the ending value of the loop. Then, every location within the 
3x3 is checked for a repeating value. We return 1, if there is a repeating value in the 3x3. Finally, we use these helper 
functions to define is_Val_valid. This function calls the three helper functions for a the given cell location and checks 
if the val can be added to the cell. If it can, it returns 0.

We use the above defined functions within the solve_sudoku function. In solve_sudoku, we go through the 9x9 board using 
the given pseudocode. We check first for an empty cell row by row in the board. If an empty cell is found, we loop through 
1 to 9 and accordingly call  is_val_valid to check if the cell can be filled with that number. If not, the cell if filled 
with 0 and kept as an empty cell. If the cell value is replaced, then we check if the board is still incomplete by calling
solve_sudoku recursively. This fills up all the empty cells. The function returns 1 when board is complete.
*/

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);
  
  // BEG TODO
  int col; //intilaize the variable 
  for(col=0; col<9; col++){ //loop through the column  
    if(sudoku[i][col]== val){ //check if number is already there 
      return 1; //return `1 (number is there already)
    }
  }
  return 0; //return 0
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  int row; //initialize the variable 
  for(row=0; row<9; row++){ // loop through the rows
    if(sudoku[row][j]== val){ // check if number is already there in the column
      return 1; //return 1 (number is there already)
    }
  }
  return 0; //return 0
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9);
  
  // BEG TODO
  int row; //initialize 3x3 row variable
  int col; //initialize 3x3 column variable
  if(i<3){ //if the specific cell is in the first 3 rows
    row = 0; //loop through 3x3 from row 0
  }
  else if(i<6 && i>2){ //if the specific cell in the next 3 rows
    row = 3; //loop through 3x3 from row 3
  }
  else if(i<9 && i>5){ //if the specific cell in the next 3 rows
    row = 6; //loop through 3x3 from row 6
  }
  if(j<3){ //if the specific cell in the first 3 cols
    col = 0; //loop through 3x3 from col 0
  }
  else if(j<6 && j>2){ //if the specific cell in the next 3 cols
    col = 3; //loop through 3x3 from col 3
  }
  else if(j<9 && j>5){ //if the specific cell in the next 3 cols
    col = 6; //loop through 3x3 from col 6
  }
  int row_max = row+3; //set the 3x3 row bound as starting row +3
  int col_max = col+3; //set the 3x3 col bound as starting col +3
  for(int k= row;k<row_max;k++){ //loop through 3x3 rows
    for(int l= col;l<col_max;l++){ //loop through 3x3 cols
      if(sudoku[k][l]==val){ // if value is repeated
        return 1; //return 1
      }
    }
  }
  
  return 0; //return 0 (value is not found in 3x3)
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) { 

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  if((is_val_in_row(val,i,sudoku))||(is_val_in_col(val,j,sudoku))|| (is_val_in_3x3_zone(val,i,j,sudoku))){ //check if val is repeated in the column, row, or 3x3
    return 0; //return 0
  }
  return 1; //return 1 (value can be filled)
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int i,j,number; // initialize variable
  for(i=0;i<9;i++){ // loop through entire sudoku row
    for(j=0;j<9;j++){ // loop through entire sudoku col
      if(sudoku[i][j]==0){ // check for the empty cell
        for(number=1;number<=9;number++){ // for an empty cell, check which number is valid to be filled
          if(is_val_valid(number,i,j,sudoku)){ // check if specific number is valid in the cell
            sudoku[i][j]= number; // fill the cell with the valid number
            if(solve_sudoku(sudoku)==1){ // recursively check if the board is completed
              return 1; // return 1
            }
            sudoku[i][j]=0; // fill the empty cell with the zero in case there is no valid value
          }
        }
      return 0; // return 0
      }
    }
  }
  return 1; // return 1 (board is complete) 
  
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}