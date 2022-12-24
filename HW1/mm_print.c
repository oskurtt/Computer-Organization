#include "matrix.h"
int mm_print(matrix* mat)
{
/*
Notes:
  Print all the data values in your matrix
  Go row by row
  At the end of each row, print a newline
  Between each data value, insert a tab
  Use 2 digits of precision after the decimal (use "%10.2lf\t" as format string)
  See output.txt for expected output formatting
*/
//basic printing with for loops
  int i, j;
  printf("\n/******** START of 2-D Matrix **********************************/\n");
  for (i = 0; i < mat->rows; i++){
    for (j = 0; j < mat->cols; j++){
      printf("%10.2lf\t", mat->data[i][j]);
    }
    printf("\n");
  }
  printf("/******** END of 2-D Matrix ************************************/\n");
  return 0;
}