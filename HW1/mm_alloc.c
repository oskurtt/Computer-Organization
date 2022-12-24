
#include "matrix.h"
int mm_alloc(matrix* mat)
{

/*
Notes:
  Allocate an array of pointers to a double
  Then, allocate an row/array of doubles and assign each double pointer that row
  Make sure you're allocating using mat->data in the matrix struct
  You can use calloc for allocation and zeroing out all the bytes
  - otherwise, you'd want to manually set all arrays to 0 (good practice)
  If your malloc/calloc returns null, the allocation failed
*/
  int i, j;
  //check if allocation exists
  mat->data = (double **)malloc(mat->rows * sizeof(double *));
  if (mat->data == NULL) {
    printf("mm_alloc: allocation failed.\n");
    exit(-1);
  }
  //check if elemnt is NULL and return error
  for (i = 0; i < mat->rows; i++){
    mat->data[i] = (double *)malloc(mat->cols * sizeof(double));
    if (mat->data[i] == NULL) {
    printf("mm_alloc: allocation failed.\n");
    exit(-1);
  }
  }
//go through each element and set it to 0
  for (i = 0; i < mat->rows; i++){
    for (j = 0; j < mat->cols; j++){
      mat->data[i][j] = 0.0;
    }
  }

  return 0;
}
