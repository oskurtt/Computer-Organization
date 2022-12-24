#include "matrix.h"
int mm_free(matrix* mat)
{
/*
Notes:
  Reverse the allocation process, but you'll be calling free() instead
*/
//for loop through matrix and free everthing
  int i;
  for (i = 0; i < mat->rows; i++){
    free(mat->data[i]);
  }
  free(mat->data);
  return 0;
}

