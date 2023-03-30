#include <stdio.h>
#include <stdlib.h>
#include "maze.h"


/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    maze_t * mymaze = (maze_t*) malloc(sizeof(maze_t)); //reserve space for memory 
    FILE *file = fopen (fileName, "r"); //create an instance that stores maze file
    int rows, cols; // intialize variable 
    fscanf (file, "%d %d\n" , &cols, &rows); // read the stored size of the maze from the file 
    mymaze -> width = cols; // assign the width using cols 
    mymaze -> height= rows;  // assign the height using rows
    mymaze -> cells = (char **)calloc(rows, sizeof(char *)); //reserve space for each rows
    int i; // intialize variable 
    for (i=0; i<rows; i++) // go through every row 
    {
        mymaze -> cells[i] = (char *)calloc (cols, sizeof(char));//reserve space for each cols
    }
    int k, l;// intialize variable 
    for ( k=0; k< rows; k++)// go through every row
    {
        for (l=0; l< cols+1; l++)// go through every cols
        {
            char cell = fgetc(file); //get the next charcter 
            if (cell == 'S') //cell is found with 'S'
            {
                mymaze -> startRow = k;//assign the row value of 'S'
                mymaze -> startColumn = l;//assign the col value of 'S'

            }
            else if (cell == 'E')//cell is found with 'E'
            {
                mymaze -> endRow = k;//assign the row value of 'E'
                mymaze -> endColumn = l;//assign the col value of 'E'
                
            }
            if (cell != '\n') // if cell not S or E or next line charcter 
            {
                mymaze -> cells[k][l] = cell; //assign the same character 
            }
        }
    }
    fclose(file); // close the files
    return mymaze; //return the created maze 
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    int i;// intialize variable 
    for(i=0; i<maze -> height; i++ ){ // go through every rows in the maze 
        free(maze -> cells[i]); //remove the allocation memory 
    }
    free(maze->cells); // remove all the cells in the maze 
    free(maze); // remove allocation in the maze 
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    int i,j;// intialize variable 
    for(i=0; i<maze -> width; i++){ // go through every cols in the maze
        for(j=0; j<maze -> height; j++){// go through every rows in the maze
            printf("%c",maze -> cells[i][j]); //assign cells of each rows and cols of the maze 
        }
        printf("\n"); //print next line 
    }
}
/*
 * solveMazeDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.
    if ((col <0) || (row <0) || (col > maze -> width) || (row > maze ->height)){ //if row, col outside bounds of the maze
        return 0;//return false
    }
    if (maze->cells[row][col] == 'E'){// if row, col is the end of the maze 
        maze->cells[maze->startRow][maze->startColumn] = 'S'; //replace the starting position asterisk with S once 'E' is found
        return 1;//return true
    }
    if (maze->cells[row][col] != ' ' && maze->cells[row][col] != 'S'){ // if row,col is not an empty cell and it is not the start
        return 0;//return false
    }
    if(maze->cells[row][col] == ' ' || maze->cells[row][col] == 'S'){// if row,col is an empty cell
        maze->cells[row][col] = '*';//set row,col as part of solution
    }
    if (solveMazeDFS(maze, col-1,row) == 1 ){ //recursively check for cells to the left
        return 1;//return true
    }
    if (solveMazeDFS(maze, col+1,row) == 1 ){//recursively check for cells to the right
        return 1;//return true
    }
    if (solveMazeDFS(maze, col,row-1) == 1 ){//recursively check for cells above
        return 1;//return true
    }
    if (solveMazeDFS(maze, col,row+1) == 1 ){//recursively check for cells below
        return 1;//return true
    }
    if(maze->cells[row][col] != 'S'){//if the cell is not the start
       maze->cells[row][col] = '~';// set cell as visited but not solution cell
    }
    return 0;//return false
}