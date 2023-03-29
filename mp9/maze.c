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
    maze_t * mymaze = (maze_t*) malloc(sizeof(maze_t)); 
    FILE *file = fopen (fileName, "r");
    int rows, cols;
    fscanf (file, "%d %d\n" , &cols, &rows );
    mymaze -> width = cols;
    mymaze -> height= rows; 
    mymaze -> cells = (char **)calloc(rows, sizeof(char *));
    int i;
    for (i=0; i<rows; i++)
    {
        mymaze -> cells[i] = (char *)calloc (cols, sizeof(char));
    }
    int k, l;
    for ( k=0; k< rows; k++)
    {
        for (l=0; l< cols+1; l++)
        {
            char cell = fgetc(file);
            if (cell == 'S')
            {
                mymaze -> startRow = k;
                mymaze -> startColumn = l;

            }
            else if (cell == 'E')
            {
                mymaze -> endRow = k;
                mymaze -> endColumn = l;
                
            }
            if (cell != '\n')
            {
                mymaze -> cells[k][l] = cell;
            }
        }
    }
    fclose(file);
    return mymaze;
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
    int i;
    for(i=0; i<maze -> height; i++ ){
        free(maze -> cells[i]);
    }
    free(maze->cells);
    free(maze);
    return;
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
    int i,j;
    for(i=0; i<maze -> width; i++){
        for(j=0; j<maze -> height; j++){
            printf("%c",maze -> cells[i][j]);
        }
        printf("\n");
    }
    return;
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
    if ((col <0) || (row <0) || (col > maze -> width) || (row > maze ->height)){
        return 0;
    }
    if (maze->cells[row][col] == 'E'){
        return 1;
    }
    if (maze->cells[row][col] != ' ' && maze->cells[row][col] != 'S'){
        return 0;
    }
    if(maze->cells[row][col] != 'S'){
        maze->cells[row][col] = '*';
    }
    if (solveMazeDFS(maze, col-1,row) == 1){
        return 1;
    }
    if (solveMazeDFS(maze, col+1,row) == 1){
        return 1;
    }
    if (solveMazeDFS(maze, col,row-1) == 1){
        return 1;
    }
    if (solveMazeDFS(maze, col,row+1) == 1){
        return 1;
    }
    if(maze->cells[row][col] != 'S'){
       maze->cells[row][col] = '~'; 
    }
    return 0;
}