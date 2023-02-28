
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
    
    int live = 0;
    int i,j;
    for (i=(row-1); i<=(row+1);i++){
        if (i < boardRowSize && i >= 0){
            for (j=(col-1); j<=(col+1);j++){
                if (j < boardColSize && j >= 0){
                    if (i!=row || j!= col){
                        int cell_location = (i*boardColSize) + j;
                        if (board[cell_location]==1){
                            live++;
                        } 
                    }
                }
            }
        }
    }
    return live;
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
    int k,l,m, livecount;
    int newboard[boardRowSize * boardColSize];
    for (k=0;k < boardRowSize; k++){
        for (l=0;l < boardColSize; l++){
            int loc = (k*boardColSize) + l;
            livecount = countLiveNeighbor(board, boardRowSize, boardColSize, k, l);
            if (*(board+loc)==0 && (livecount == 3)){
                newboard[loc] = 1;
            }
            else if (*(board+loc)==1 && (livecount<2 || livecount >3)){
                newboard[loc]=0;
            }
            else if (*(board+loc)==1 && (livecount==2 || livecount==3) ){
                newboard[loc] = 1;
            }
            else{
                newboard[loc]=0;
            }
        }
    }
    for (m=0; m < (boardRowSize * boardColSize); m++){
        *(board+m) = newboard[m];
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
    int sameboard[boardRowSize * boardColSize];
    int y,z;
    for (y=0;y < boardRowSize; y++){
        for (z=0;z < boardColSize; z++){
            int sameloc = y*boardColSize + z;
            sameboard[sameloc] = board[sameloc];
        }
    }

    updateBoard(sameboard, boardRowSize, boardColSize);
    
    for (y=0;y < boardRowSize; y++){
        for (z=0;z < boardColSize; z++){
            int sameloc = y*boardColSize + z;
            if (sameboard[sameloc] != board[sameloc]){
                return 0;
            }
        }
    }
    return 1;
}