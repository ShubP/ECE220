/*
partners: ykko2, mamir6

The first error was that the semicolon was missing in the return line (second to the last line from main.c). The effect of this error is that the file wouldn't compile.
In addition, we had to change both return value in the is_prime function. The effect of the return value is that it would return 1 if the number is not prime and 0 if it is prime when it should be the opposite. 
The second error was in the is_prime(j) if statement. Instead of the module sign, we were supposed to have the division sign because we had to check whether the provided number is evenly divisible by any number between 2 and one less than the provided number. 
The effect of this error was that it was printing out the incorrect program output. Lastly, the last error was that we had to add "ret=1" and "break" for the is_prime(k) if statement. 
The effect without the "break" statement is that it would print the last value twice when it should be printed only once. Also, without the "ret=1" statement, it would return the value 0 when it should return the value 1. 
*/

#include <stdlib.h>
#include <stdio.h>


/*
 * is_prime: determines whether the provided number is prime or not
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number)
{
    int i;
    if (number == 1) {return 0;}
    for (i = 2; i < number; i++) { //for each number smaller than it
        if (number % i == 0) { //check if the remainder is 0
            return 0;
        }
    }
    return 1;
}


/*
 * print_semiprimes: prints all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b)
{
    int i, j, k;
    int ret = 0;
    for (i = a; i <=b; i++) { //for each item in interval
        //check if semiprime
        for (j = 2; j <= i; j++) {
            if (i%j == 0) {
                if (is_prime(j)) {
                    k = i/j;
                    if (is_prime(k)) {
                        printf("%d ", i);
		                ret=1; 
			            break;
                    }
                }
            }
        }

    }
    printf("\n");
    return ret;

}
