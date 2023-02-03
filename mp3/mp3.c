

#include <stdio.h>
#include <stdlib.h>

int main()
{
  int row;                                                                               

  printf("Enter a row index: ");
  scanf("%d",&row);

  if (row==0){
    printf("1\n");
  }
  else{
    unsigned long int result = 1;
    unsigned long int k;
    unsigned long int i;
    for (k = 0; k <= row; k++){
      for (i=1; i <= k; i++){
        result = result * ( row + 1 - i )/(i) ;
      }
      printf("%lu ",result);
      result = 1;
    }
    printf("\n");
  }
  return 0;
}
