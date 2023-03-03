/* partners: ykko2, mamir6
In the program, we set up the board by using the 1-D array of the current game board. First, we check for alive cells (1) and dead cells (0).
Then, it goes through the neighboring columns and rows to check if it goes over the boundaries of row and column of the game board except for original
cell. Then, it finds the 1-D location of the specific cell and checks if it is alive or dead. It will increment the live neighbor count and at the end,
it will return the total number of live neighbors. Next, we moved on to the updateBoard function and created a new array to store the updated cells.
Similarily, we went through each row and column and finded the 1D location of the cell. For this board, we had to check for four different rules
for a specific cell. Then, we copied the newboard cells and updated to the original board. Lastly, we moved on to the aliveStable function.
We created a new array of the original board size and went through the rows and columns again. We copied the cells from the original board and find the
1D location of the cell. We updated the board to the next step and if the original board and the same board is not equal, it would return 0.
Otherwise, it would return 1. 


*/

/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
    
    int live = 0; //initialize variable 
    int i,j; //initialize variable 
    for (i=(row-1); i<=(row+1);i++){ //go through the neighboring rows
        if (i < boardRowSize && i >= 0){ //check the row boundaries of the game board
            for (j=(col-1); j<=(col+1);j++){ //go through the neighboring column 
                if (j < boardColSize && j >= 0){ //check the column boundaries of the game board
                    if (i!=row || j!= col){ //skip check for original cell
                        int cell_location = (i*boardColSize) + j; //find the 1-D location of the specific cell
                        if (board[cell_location]==1){ //check if cell is alive or dead 
                            live++;//increment live neighbor count 
                        } 
                    }
                }
            }
        }
    }
    return live; // total number of live neighbors for specific cell
}

/*
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
    int k,l,m, livecount; //initialize variable
    int newboard[boardRowSize * boardColSize]; //create a new array to store the update cells  
    for (k=0;k < boardRowSize; k++){ //go through every rows
        for (l=0;l < boardColSize; l++){ //go through every columns 
            int loc = (k*boardColSize) + l; //find the 1-D location of the specific cell
            livecount = countLiveNeighbor(board, boardRowSize, boardColSize, k, l); //hold the live neighbor cells 
            if (*(board+loc)==0 && (livecount == 3)){ //check if the specific cell is dead and check condition if live neighbors is equal to three 
                newboard[loc] = 1; //the specific cell become alive 
            }
            else if (*(board+loc)==1 && (livecount<2 || livecount >3)){ //check if the specific cell is alive and check if live neighbors is less than 2 or live neighbors is greater than 3
                newboard[loc]=0; //the specific cell become dead 
            }
            else if (*(board+loc)==1 && (livecount==2 || livecount==3) ){//Check if the specific cell is alive and check condition if live neighbors is equal to three or check condition if live neighbors is equal to two 
                newboard[loc] = 1; // the specific cell become alive 
            }
            else{
                newboard[loc]=0; //the specific cell become dead 
            }
        }
    }
    for (m=0; m < (boardRowSize * boardColSize); m++){ // copy the newboard cells 
        *(board+m) = newboard[m]; // update to the original board to the newboard
    }
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */
int aliveStable (int* board, int boardRowSize, int boardColSize) { 
    int sameboard[boardRowSize * boardColSize]; // creating a array of the original game board size 
    int y,z; //initaliaize variable
    for (y=0;y < boardRowSize; y++){ // go through every row
        for (z=0;z < boardColSize; z++){ // go through every column
            int sameloc = y*boardColSize + z; //find the 1-D location of the specific cell
            sameboard[sameloc] = board[sameloc];// copy the cells from the original game board
        }
    }

    updateBoard(sameboard, boardRowSize, boardColSize); // update the board to next step
    
    for (y=0;y < boardRowSize; y++){ //  go through every row
        for (z=0;z < boardColSize; z++){ // go through every column
            int sameloc = y*boardColSize + z; //find the 1-D location of the specific cell
            if (sameboard[sameloc] != board[sameloc]){ // if it not equal then return 0 
                return 0; 
            }
        }
    }
    return 1; //return 1 
}