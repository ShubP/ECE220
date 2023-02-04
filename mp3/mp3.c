/* partners: shubbp2, ykko2 */

/* This C program inputs a index row from the user, and prints the binomial coefficients as found in Pascal's triangle. 
The given equation calculates the coefficient using a for loop. The final result is printed to the display. */

#include <stdio.h>
#include <stdlib.h>

int main()
{
  int row;	

  printf("Enter a row index: ");	
  scanf("%d",&row);	
 

if (row ==0){	/*Case for index '0' */
	printf("1 ");	/*Print '1' for case '0'*/
}
else{
	unsigned long int result=1;	/* initialize coefficient variable */
	int k; /* initialize coefficient counter */
	int i; /* initialize equation counter */
	for (k=0; k<= row; k++){	/*loop for each coefficient */
		for (i=1; i<=k; i++){	/* loop for coefficient equation */
			result =result*(row + 1-i)/(i);	/*calculate using the equation */
		}
		printf("%lu ", result);	/* print the coefficient */
		result =1; 	/* reset coeffient variable */
	}
}
	printf("\n");	/* print next line */
  return 0;
}
