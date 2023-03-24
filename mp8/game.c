/* partners: shubbp2, ykko2
For this MP, we first create the make_game function which creates an instance in the game structure with the rows and cols parameters entered.
The rows and cols variables in game are set to the rows and cols provided to the function. The score is initialized to 0 since it is a new game. All
cells in the game instance are initialized as empty cells(-1). The remake_game function is written in the same way, but deferenced once because of 
the double struct. We also write the get_cell function which is used in other sections of the code. The get_cell function checks the bounds of the given row and col
and returns NULL if the invalid coordinates are entered. Otherwise it points to the address calculated using the row-major formula.

For the sliding functions, we use a algorithm of first sliding all exisiting cells in the given direction. Then, we check for any possible combining tiles and combine
them. A combination creates empty cells in the algorithm which has to be adjusted. Hence, we slide the cells again in the given direction which gives us the desired result.
For example, for the initial sliding for move_w, we find the first available empty cell above the cell and accordingly change it. Since a change has occured, the move 
is set as valid. Then for the combining cells, we go through every possible row combination from the top. If a combination needs to be made, the cell values are added 
and stored in the upper cell of the combination. The lower cell is changed to an empty cell. We then skip the next combination in order to prevent combining of the same cell
again. Since empty cells are created, we adjust it again using the same initial method of finding available cells. This algorithm is applied in every direction, but the loop moves 
in the direction for rows and columns accordingly.

Lastly, for legal_move_check we first check if there are any empty cells in the game since that indicates a possible move. If not, we first create a new game instance and then copy
the cell values from the current game to the new instance. We then apply the four move functions on the new instance and check if any of them return a valid move. Accordingly, 
the function returns if a valid move exists while preventing any changes to the current game.
*/

#include "game.h"


game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    int i,j; //Initialize variables
    mygame -> rows = rows; // set row in my game 
    mygame -> cols = cols; // set column in my game 
    mygame -> score = 0; // set score in my game 
    for(i = 0; i < mygame -> rows; i++) // iterate through every row 
    {
      for(j = 0; j < mygame -> cols; j++) // iterate through every column  
      {
        (mygame->cells[i*(mygame -> cols)+j]) = -1;//initialize every element in the mygame to -1
      }
    }

    return mygame;

    // return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	//YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
    (*_cur_game_ptr)->rows = new_rows; // set row in _cur_game_ptr
    (*_cur_game_ptr)->cols = new_cols; // set column in _cur_game_ptr
    (*_cur_game_ptr)->score = 0; // set score in _cur_game_ptr

   int i,j;
   for(i = 0; i < (*_cur_game_ptr)->rows; i++) // iterate through every row in cur_game_ptr
   {
     for(j = 0; j < (*_cur_game_ptr)->cols; j++) // iterate through every column in cur_game_ptr
     {
       (*_cur_game_ptr)->cells[i*((*_cur_game_ptr) -> cols)+j] = -1;//initialize every element in the cur_game_ptr to -1
     }
   }
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    if(row < 0 || col < 0 || row > cur_game->rows || col > cur_game->cols)//check if coordinate exist 
    {
      return NULL; //return null
    }
    else{
      return &cur_game->cells[row*(cur_game->cols) + col];//return the pointer to the corresponding cells 
    }
   
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    int i , j; //Initialize variables
    int check=0; //Initialize variable
    for (j=0;j<cur_game ->cols; j++){ //check through every columns
        for(i=0;i<cur_game->rows; i++){ //check through every rows
            int cur_row = *get_cell(cur_game, i, j); // calculate the cell value at the current i,j
            if (cur_row != -1){ //check if the cell is empty
                int available = i-1; // initialize available
                for (available= i-1; available >=0;available--){ // loop for finding the first available empty cell above the current cell
                    if (*get_cell(cur_game, available, j)!= -1){ // break the loop if a filled cell is found above
                        break; // break
                    }
                }
                available++; //adjust available for the loop extra count
                if (*get_cell(cur_game, available, j) == -1){ // check if the target cell is empty
                    *get_cell(cur_game, available, j) = *get_cell(cur_game, i, j); // set the target cell as the current cell
                    *get_cell(cur_game, i, j) = -1; // set the current cell as empty
                    check = 1; // set as a valid move
                }
            }
        }
        int k; // initialize variable
        for(k=1;k<cur_game->rows; k++){ // go through every combination of cells
            int a; // initialize variable
            for (a=k-1; a<k+1;a++){ // go through the cell in the given combination
                if ((*get_cell(cur_game, a, j) == *get_cell(cur_game, a+1, j)) && (*get_cell(cur_game, a, j) != -1) && (*get_cell(cur_game, a+1, j) != -1)){ // check if combination has equal and non-empty cells
                    *get_cell(cur_game, a, j) += *get_cell(cur_game, a+1, j); // combine the value of cells and store in the upper cell
                    *get_cell(cur_game, a+1, j) = -1; // set the lower cell as empty
                    k++; // increment k to skip to next combination                
                    cur_game -> score += cur_game -> cells[(a)*(cur_game -> cols)+j]; // calculate the score
                    check = 1; // set as a valid move
                }
            }
        }
        for(i=0;i<cur_game->rows; i++){ // adjust all cells upward in case a combination has been made
            int cur_row = *get_cell(cur_game, i, j);
            if (cur_row != -1){
                int available = i-1;
                for (available= i-1; available >=0;available--){
                    if (*get_cell(cur_game, available, j)!= -1){
                        break;
                    }
                }
                available++;
                if (*get_cell(cur_game, available, j) == -1){
                    *get_cell(cur_game, available, j) = *get_cell(cur_game, i, j);
                    *get_cell(cur_game, i, j) = -1;
                    check = 1;
                }
            }
        }        
    }
    return check; // return if it's a valid move
};

int move_s(game * cur_game) //
{
    int i , j; // initialize variable
    int check=0; // initialize variable
    for (j=0;j<cur_game ->cols; j++){ // check through every columns
        for(i=cur_game->rows -1;i>=0; i--){ // check through every rows
            int cur_row = *get_cell(cur_game, i, j); //calculate the cell value at the current i,j
            if (cur_row != -1){// check if cell is empty
                int available; // initialize available
                for (available= i+1; available < cur_game->rows;available++){ // loop for finding the first available empty cell below the current cell
                    if (*get_cell(cur_game, available, j)!= -1){ // break the loop if a filled cell is found below
                        break; // break
                    }
                }
                available--; // adjust available for the loop reduced count
                if (*get_cell(cur_game, available, j) == -1){ // check if the target cell is empty
                    *get_cell(cur_game, available, j) = *get_cell(cur_game, i, j); // set the target cell as the current cell
                    *get_cell(cur_game, i, j) = -1; //set the current cell as empty
                    check = 1; // set as a valid move
                }
            }
        }
        int k; // initialize variable
        for(k=(cur_game->rows)-1;k>0; k--){ // go through every combination of cells
            int a; // initialize variable
            for (a=k-1; a<k;a++){ // go through the cell in the given combination
                if ((*get_cell(cur_game, a, j) == *get_cell(cur_game, a+1, j)) && (*get_cell(cur_game, a, j) != -1) && (*get_cell(cur_game, a+1, j) != -1)){  // check if combiation has equal and non-empty cells
                    *get_cell(cur_game, a+1, j) += *get_cell(cur_game, a, j); // combine the value of cells and store in the lower cell
                    *get_cell(cur_game, a, j) = -1; // set the upper cell as empty
                    
                    k--; // decrement k to skip to next combination             
                    cur_game -> score += cur_game -> cells[(a+1)*(cur_game -> cols)+j]; // calculate the score
                    check = 1; // set as a valid move
                }
            }
        }
        for(i=cur_game->rows -1;i>=0; i--){// adjust all cells below in case a combination has been made
            int cur_row = *get_cell(cur_game, i, j);
            if (cur_row != -1){
                int available;
                for (available= i+1; available < cur_game->rows;available++){
                    if (*get_cell(cur_game, available, j)!= -1){
                        break;
                    }
                }
                available--;
                if (*get_cell(cur_game, available, j) == -1){
                    *get_cell(cur_game, available, j) = *get_cell(cur_game, i, j);
                    *get_cell(cur_game, i, j) = -1;
                    check = 1;
                }
            }
        }
    }
    return check; // return if it's a valid move
};

int move_a(game * cur_game) 
{
    int i , j; // initialize variable
    int check=0; //initialize variable
    for(i=0;i<cur_game->rows; i++){ // check through every rows
        for (j=0;j<cur_game ->cols; j++){ // check through every columns
            int cur_col = *get_cell(cur_game, i, j); //calculate the cell value at the current i,j
            if (cur_col != -1){//check if the cell is empty
                int available = j-1; // initialize available
                for (available= j-1; available >=0;available--){ //loop for finding the first available empty cell to the left of the current cell
                    if (*get_cell(cur_game,i, available)!= -1){ //break the loop if a filled cell is found to the left
                        break; // break
                    }
                }
                available++; //adjust available for the loop extra count
                if (*get_cell(cur_game, i,available) == -1){ //check if the target cell is empty
                    *get_cell(cur_game, i,available) = *get_cell(cur_game, i, j); // set the target cell as the current cell
                    *get_cell(cur_game, i, j) = -1; // set the current cell as empty
                    check = 1; // set as a valid move
                }
            }
        }
        int k; //initialize variable
        for(k=1;k<cur_game->cols; k++){ // go through every combination of cells
            int a; // initialize variable
            for (a=k-1; a<k+1;a++){ // go through the cell in the given combination
                if ((*get_cell(cur_game, i, a) == *get_cell(cur_game, i,a+1)) && (*get_cell(cur_game, i, a) != -1) && (*get_cell(cur_game,i, a+1) != -1)){//check if the combination has equal and non-empty cells
                    *get_cell(cur_game, i,a) += *get_cell(cur_game, i, a+1); // combine the values of cells and store in the left cell
                    *get_cell(cur_game, i, a+1) = -1; // set the right cell as empty
                    k++;// increment k to skip to next combination                 
                    cur_game -> score += cur_game -> cells[i*(cur_game -> cols)+(a)]; //calculate the score
                    check = 1; //set as a valid move
                }
            }
        }
        for (j=0;j<cur_game ->cols; j++){//adjust all cells leftward in case a combination has been made
            int cur_col = *get_cell(cur_game, i, j);
            if (cur_col != -1){
                int available = j-1;
                for (available= j-1; available >=0;available--){
                    if (*get_cell(cur_game,i, available)!= -1){
                        break;
                    }
                }
                available++;
                if (*get_cell(cur_game, i,available) == -1){
                    *get_cell(cur_game, i,available) = *get_cell(cur_game, i, j);
                    *get_cell(cur_game, i, j) = -1;
                    check = 1;
                }
            }
        }
    }
    return check; //return if it's a valid move
};

int move_d(game * cur_game)
{ //slide to the right
    int i , j; //initialize variable
    int check=0;// initialize variable
    for(i=0;i<cur_game->rows; i++){ // check through every rows
        for (j=cur_game ->cols -1;j>=0; j--){ //check through every columns
            int cur_col = *get_cell(cur_game, i, j); //calculate the cell value at the current i,j
            if (cur_col != -1){//check if the cell is empty
                int available;//initialize variable
                for (available= j+1; available < cur_game->cols;available++){// loop for finding the first available empty cells right to the current cell
                    if (*get_cell(cur_game, i,available)!= -1){ //break the loop if a filled cell is found to the right
                        break;//break
                    }
                }
                available--; //adjust available for the loop reduced count
                if (*get_cell(cur_game, i,available) == -1){ //check if the target cell is empty
                    *get_cell(cur_game, i,available) = *get_cell(cur_game, i, j);// set the target cell as the current cell
                    *get_cell(cur_game, i, j) = -1; //set the current cell is empty
                    check = 1; // set as a valid move
                }
            }
        }
        int k; // initialize variable
        for(k=(cur_game->cols) -1;k>0; k--){ // go through every combination of cells
            int a; // initialize variable
            for (a=k-1; a<k;a++){ // go through the cell in the given combination
                if ((*get_cell(cur_game, i, a) == *get_cell(cur_game, i,a+1)) && (*get_cell(cur_game, i, a) != -1) && (*get_cell(cur_game,i, a+1) != -1)){ //check if combination has equal and non-empty cells
                    *get_cell(cur_game, i,a+1) += *get_cell(cur_game, i, a);//combine the value of cells and store in the right cell
                    *get_cell(cur_game, i, a) = -1;//set the left cell as empty
                    k--; //decrement k to skip to next combination                
                    cur_game -> score += cur_game -> cells[i*(cur_game -> cols)+(a+1)]; //calculate the score
                    check = 1; // set as a valid move
                }
            }
        }
        for (j=cur_game ->cols -1;j>=0; j--){ // adjust all cells upward in case a combination has been made
            int cur_col = *get_cell(cur_game, i, j);
            if (cur_col != -1){
                int available;
                for (available= j+1; available < cur_game->cols;available++){
                    if (*get_cell(cur_game, i,available)!= -1){
                        break;
                    }
                }
                available--;
                if (*get_cell(cur_game, i,available) == -1){
                    *get_cell(cur_game, i,available) = *get_cell(cur_game, i, j);
                    *get_cell(cur_game, i, j) = -1;
                    check = 1;
                }
            }
        }
    }
    return check; // return if it's a valid move
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE
    int i,j; //initialize variable
    for(i = 0; i < cur_game -> rows; i++) // go through every row
    {
      for(j = 0; j < cur_game -> cols; j++) // go through every column
      {
        if(*get_cell(cur_game, i,j) == -1) // check if there is any empty cell
        {
          return 1;//return 1 since empty cell indicates possible move
        }
      }
    }

    // checking for legal moves incase the board is fully filled
    game *new = make_game(cur_game -> rows, cur_game -> cols); // make a new game structure
    for(i = 0; i < cur_game -> rows; i++) // go through every row
    {
      for(j = 0; j < cur_game -> cols; j++) // go through every column
      {
          *get_cell(new, i,j) = *get_cell(cur_game, i,j); // copy all the existing cells into new
      }
    }

    if((move_w(new) == 0) && (move_s(new) == 0) && (move_a(new) == 0) && (move_d(new) == 0)){ //check if any individual move on new_game is valid
      return 0;//return 0 if any is invalid
    }
    else{
      return 1;//return 1 if any is valid
    }
    
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
