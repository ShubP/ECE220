/* partners: ykko2, mamir6

We started by writing the set_seed function. We used the return values of sscanf to distinguish between if the inputs are only numbers.
Then, if the return value is 1, we know that the inputs are only numbers. So, we use srand to set up the seed for the rand function using these numbers.
Otherwise, we return 0 to set_seed and display an error message.
Then, we wrote the start_game function. First, we call rand() to get random numbers based on the seed from srand(). 
Then, we apply modulus 8 to get numbers from 0 to 7 only. But, we added 1 to these numbers because the input numbers should be from 1-8.
The same is repeated to generate the other three digits of the solution for the game.
Finally, we wrote the make_guess function. We get the four integers from guess_str and checks if it is a number.
Then, we check if the numbers in the guess_string are between 1 to 8 inclusive. This would mean that the guess_str is valid.
The four numbers in the guess_str are assigned to the respective pointer variables in the function. Then, we initialized the
variable that would counter the number of perfect and mismatched digits. Additionally, we initalized two variables to represent
a paired guess and a paired solution for each digit. First, we count the number of perfect matches and accordingly set the paired
variables for each digit. Then, for each digit that does not have a perfect match, we check for a mismatch. If there is mismatch,
we only set the paired variable for the solution for that digit. This helps avoiding double-counting. After all checks, we print
the number of perfect and mismatched digits for that specific guess. Lastly, the guess number is incremented for use by main.c.
*/


/*			
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */


/*
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */



#include <stdio.h>
#include <stdlib.h>

#include "prog5.h"


/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;


/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the 
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid. 
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */



int set_seed (const char seed_str[])
{


//    Example of how to use sscanf to read a single integer and check for anything other than the integer
//    "int seed" will contain the number typed by the user (if any) and the string "post" will contain anything after the integer
//    The user should enter only an integer, and nothing else, so we will check that only "seed" is read. 
//    We declare
//    int seed;
//    char post[2];
//    The sscanf statement below reads the integer into seed. 
//    sscanf (seed_str, "%d%1s", &seed, post)
//    If there is no integer, seed will not be set. If something else is read after the integer, it will go into "post".
//    The return value of sscanf indicates the number of items read succesfully.
//    When the user has typed in only an integer, only 1 item should be read sucessfully. 
//    Check that the return value is 1 to ensure the user enters only an integer. 
//    Feel free to uncomment these statements, modify them, or delete these comments as necessary. 
//    You may need to change the return statement below




int seed; //initialize the value
char post[2]; //initialize the array post with size 2 to accomodate NULL
int ret_val = sscanf (seed_str, "%d%1s", &seed, post); //store the number of items read and sort them into numbers(seed) and characters(post) only 
    if (ret_val == 1) //check to see the inputs are only numbers 
     {
        srand(seed); //set up the set seed for rand
        return ret_val; //return 1 for set_seed 
    }
    else {
        printf("set_seed: invalid seed\n"); //invalid seed entered by the user since input is not numbers only.
        return 0; //return 0 for set_seed
    }

    
}


/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above. 
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 8)
 *          *two -- the second solution value (between 1 and 8)
 *          *three -- the third solution value (between 1 and 8)
 *          *four -- the fourth solution value (between 1 and 8)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */


void start_game (int* one, int* two, int* three, int* four)
{
    *one= (rand()%8) +1; // generate solution number from 1 to 8 based on the seed 
    *two= (rand()%8) +1; // generate solution number from 1 to 8 based on the seed 
    *three= (rand()%8) +1; // generate solution number from 1 to 8 based on the seed 
    *four= (rand()%8) +1; // generate solution number from 1 to 8 based on the seed 
    guess_number=1; // initialize user's guess number 
    solution1 = *one; // solution fixed to the specific rand() generated number
    solution2 = *two; // solution fixed to the specific rand() generated number
    solution3 = *three; // solution fixed to the specific rand() generated number
    solution4 = *four; // solution fixed to the specific rand() generated number
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str. 
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-8). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 8)
 *          *two -- the second guess value (between 1 and 8)
 *          *three -- the third guess value (between 1 and 8)
 *          *four -- the fourth color value (between 1 and 8)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 8), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess) 
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */



int make_guess (const char guess_str[], int* one, int* two, int* three, int* four)
{
int w,x,y,z; //initialize the values
char post[2]; //initialize the array post with size 2 to accomodate NULL
int ret_val = sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post); //store the number of items read and sort them in numbers(w,x,y,z) and characters(post) only
    if (ret_val == 4) { //check if there are only 4 numbers read by sscanf
        if((w >=1 && w <=8) && (x >=1 && x <=8) && (y >=1 && y <=8) && (z >=1 && z <=8)){ //check if the guess numbers are between 1 to 8
            *one = w; //set up the first digit in the guess 
            *two = x; //set up the second digit in the guess 
            *three = y; //set up the third digit in the guess 
            *four = z; //set up the fourth digit in the guess 
            int perfect = 0; //initialized the counter for number of perfect matches
            int mismatch = 0; //initialized the counter for number of misplaced matches
            int pairedg1 = 0; //initialized the paired variable guess number 1
            int paireds1 = 0; //initialized the paired variable solution number 1
            int pairedg2 = 0; //initialized the paired variable guess number 2
            int paireds2 = 0; //initialized the paired variable solution number 
            int pairedg3 = 0; //initialized the paired variable guess number 3
            int paireds3 = 0; //initialized the paired variable solution number 3
            int pairedg4 = 0; //initialized the paired variable guess number 4
            int paireds4 = 0; //initialized the paired variable solution number 4
            if (*one == solution1) { // check if guess and solution number 1 match
                perfect ++; // increment perfect match counter
                pairedg1 = 1; // set paired guess 1 variable
                paireds1 = 1; // set paired solution 1 variable
            }
            if (*two == solution2) { // check if guess and solution number 2 match
                perfect ++; // increment perfect match counter
                pairedg2 = 1; // set paired guess 2 variable
                paireds2 = 1; // set paired solution 2 variable
            }
            if (*three == solution3) { // check if guess and solution number 3 match
                perfect ++; // increment perfect match counter
                pairedg3 = 1; // set paired guess 3 variable
                paireds3 = 1; // set paired solution 3 variable
            }
            if (*four == solution4) { // check if guess and solution number 4 match
                perfect ++; // increment perfect match counter
                pairedg4 = 1; // set paired guess 4 variable
                paireds4 = 1; // set paired solution 4 variable
            }
            if (*one == solution2 && pairedg1 == 0 && paireds2 == 0) { // check for misplaced matches with unpaired solution numbers
                 mismatch ++;   // increment misplaced match counter
                 paireds2 = 1; // set paired solution 2 variable
            }
            else if (*one == solution3 && pairedg1 == 0 && paireds3 == 0) {
                 mismatch ++; // increment misplaced match counter
                paireds3 = 1; // set paired solution 3 variable
            }
            else if (*one == solution4 && pairedg1 == 0 && paireds4 == 0) {
                 mismatch ++;  // increment misplaced match counter
                paireds4 = 1; // set paired solution 4 variable
            }
            if (*two == solution1 && pairedg2 == 0 && paireds1 == 0) { // check for misplaced matches with unpaired solution numbers
                 mismatch ++; // increment misplaced match counter
                paireds1 = 1; // set paired solution 1 variable
            }
            else if (*two == solution3 && pairedg2 == 0 && paireds3 == 0) {
                 mismatch ++; // increment misplaced match counter
                paireds3 = 1; // set paired solution 3 variable
            }
            else if (*two == solution4 && pairedg2 == 0 && paireds4 == 0) {
                 mismatch ++; // increment misplaced match counter
                paireds4 = 1; // set paired solution 4 variable
            }
            if (*three == solution2 && pairedg3 == 0 && paireds2 == 0) { // check for misplaced matches with unpaired solution numbers
                 mismatch ++; // increment misplaced match counter
                paireds2 = 1; // set paired solution 2 variable
            }
            else if (*three == solution1 && pairedg3 == 0 && paireds1 == 0) {
                 mismatch ++; // increment misplaced match counter
                paireds1 = 1; // set paired solution 1 variable
            }
            else if (*three == solution4 && pairedg3 == 0 && paireds4 == 0) {
                 mismatch ++; // increment misplaced match counter
                paireds4 = 1; // set paired solution 4 variable
            }
            if (*four == solution2 && pairedg4 == 0 && paireds2 == 0) { // check for misplaced matches with unpaired solution numbers
                 mismatch ++; // increment misplaced match counter
                paireds2 = 1; // set paired solution 2 variable
            }
            else if (*four == solution3 && pairedg4 == 0 && paireds3 == 0) {
                 mismatch ++; // increment misplaced match counter
                paireds3 = 1; // set paired solution 3 variable
            }
            else if (*four == solution1 && pairedg4 == 0 && paireds1 == 0) {
                 mismatch ++; // increment misplaced match counter
                paireds1 = 1; // set paired solution 1 variable
            }
            printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n",guess_number, perfect, mismatch); // print perfect and misplaced matches
            guess_number ++; // increment guess number for use in main.c
            return 1; // return 1 for make_guess function
        }
        else{
             printf("make_guess: invalid guess\n"); // print invalid guess since numbers are not between 1 and 8
             return 0; // return 0 for make_guess function
        }
    }
    else {
        printf("make_guess: invalid guess\n"); // print invalid guess since guess input format is not valid
        return 0; // return 0 for make_guess function
    }

//  One thing you will need to read four integers from the string guess_str, using a process
//  similar to set_seed
//  The statement, given char post[2]; and four integers w,x,y,z,
//  sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post)
//  will read four integers from guess_str into the integers and read anything else present into post
//  The return value of sscanf indicates the number of items sucessfully read from the string.
//  You should check that exactly four integers were sucessfully read.
//  You should then check if the 4 integers are between 1-8. If so, it is a valid guess
//  Otherwise, it is invalid.  
//  Feel free to use this sscanf statement, delete these comments, and modify the return statement as needed

}


