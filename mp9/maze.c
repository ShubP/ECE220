/* partners: ykko2, mamir6
In MP9, we start by reserving space for the instance in maze_t that we are creating using malloc. Next we use the input file from the function parameter to create a FILE instance that stores the input maze.txt .
Then we used fscanf to read for the first two numbers from the file and accordingly store them in variables. These variables are then used to assign width and height in the instance created. These variables are 
then used to reserve memory for number of rows in the maze. Then using a loop, we reserve memory for each column in a given row. Once memory is allocated, we start assigning the cells in our maze_t instance. If a 'S' or 'E'
is found in the loop iterations, we assign the start and end variables for row and column in the maze_t instance. For any other cell, other than newline, we assign it to cells as it is found in the text file.
We then close the file, and return the created maze_t instance.

For destroyMaze, we simply reverse our allocation process and use free to remove any memory allocation for the number of rows, everything in cells, and the maze_t structure instance itself. For printMaze, we loop 
through every row and column in the maze instance and print the corresponding character in cells location given by the loop variables. If the loop reaches the end of a column, we print a newline character and move to next row.

Lastly, for solveMazeDFS, we first check for the bounds of the given cell (row,col) and return 0 if out of bounds. Next, as told in the lab discussion, we switch steps 2 and 3 in the given algorithm and check for 'E'. 
If 'E' is found, then we make sure that we replace the * at the start position with 'S' since our modification to the algorithm changes the 'S' character to an asterisk as well. Then, we return 1. We then check if the cell
is not an empty cell(' ') or 'S'. If so, we return false. Then, we assign the cell to an '*' if it is the start or an empty cell. We then recursively check for cells to left, right, above and below to continue the solution path.
Once there is no continuing solution path, we backtrack till an alternative path is possible, and mark the current checked cells with '~' while not including them in the solution path. Incase no alternative paths are found,
we return 0 to indicate an unsolvable maze. This recursion and backtracking helps us find a suitable solution path in the maze while indicating paths that were checked but did not lead to 'E' in the maze cells.
*/

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
    maze_t * mymaze = (maze_t*) malloc(sizeof(maze_t)); //reserve space in memory for maze_t instance 
    FILE *file = fopen (fileName, "r"); //create an FILE instance that stores the input maze file
    int rows, cols; // intialize variables
    fscanf (file, "%d %d\n" , &cols, &rows); // read the stored size of the maze from the input file 
    mymaze -> width = cols; // assign the maze width using cols
    mymaze -> height= rows;  // assign the maze height using rows
    mymaze -> cells = (char **)calloc(rows, sizeof(char *)); //reserve space for the number of rows pointed by double pointer
    int i; // intialize variable 
    for (i=0; i<rows; i++) // go through every row in the maze
    {
        mymaze -> cells[i] = (char *)calloc (cols, sizeof(char));//reserve space for each column for each row in the maze
    }
    int j, k;// intialize variables to loop through maze characters
    for ( j=0; j< rows; j++)// go through every row in maze
    {
        for (k=0; k<= cols; k++)// go through every col and newline in maze
        {
            char cell = fgetc(file); //get the next charcter 
            if (cell == 'S') //cell is found with 'S'
            {
                mymaze -> startRow = j;//assign the row value of 'S' as start of maze position
                mymaze -> startColumn = k;//assign the col value of 'S' as start of maze position

            }
            else if (cell == 'E')//cell is found with 'E'
            {
                mymaze -> endRow = j;//assign the row value of 'E' as the ending of maze
                mymaze -> endColumn = k;//assign the col value of 'E' as the ending of maze
                
            }
            if (cell != '\n') // if cell is not 'S' or 'E' or newline character ('%' or ' ')
            {
                mymaze -> cells[j][k] = cell; //assign the same character in the maze instance's cells
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
        free(maze -> cells[i]); //reverse allocation and remove it for each row in cells
    }
    free(maze->cells); // remove allocation for cells in maze instance
    free(maze); // remove allocation for maze structure
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

    int i,j;// intialize variables
    for(i=0; i<maze -> height; i++){ // go through every row in the maze
        for(j=0; j<maze -> width; j++){// go through every column in the maze
            printf("%c",maze -> cells[i][j]); //print the char stored in cells for each position(i,j)
            if (j == (maze-> width)-1) {
                printf("\n"); //print next line at the end of each column
            }
        }
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
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
    if(maze->cells[row][col] == ' ' || maze->cells[row][col] == 'S'){// if row,col is an empty cell or the starting cell
        maze->cells[row][col] = '*';//set row,col as part of solution, starting cell will be reverted to 'S' once 'E' is found
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
    if(maze->cells[row][col] != 'S'){//if the cell is not the start and cannot continue chain of solution
       maze->cells[row][col] = '~';// set cell as visited but not solution cell and move to alternative path checks
    }
    return 0;//return false
}