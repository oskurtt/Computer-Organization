#include "matrix.h"

matrix* mm_matrix_mult(matrix* mat1, matrix* mat2)
{
/*
Notes:
  First check that it is actually possible to multiply the two inputs
  If so, construct a result matrix
  Then perform the multiplication
  Then return a pointer to your result matrix
*/
  int i, j, x;
  double calc = 0.0;
  //allocate size of the new matrix
  matrix* result_matrix = (matrix*)malloc(sizeof(matrix));
  result_matrix->rows = mat1->rows;
  result_matrix->cols = mat2->cols;
  //allocate contents of the matrix
  mm_alloc(result_matrix);
  //check if dimesions of matrices are mismatchec
  if (mat1->cols != mat2->rows){
    printf("mm_matrix_mult: dimension mismatch between matrices.\n");
    exit(-1);
  }
  //calculate matrices
  for (i = 0; i < result_matrix->rows; i++){
    for (j = 0; j < result_matrix->cols; j++){
      for (x = 0; x < mat1->cols; x ++){
        calc += (mat1->data[i][x] * mat2->data[x][j]);
      } 
      result_matrix->data[i][j] = calc;
      calc = 0;
    }
  }
  return result_matrix;
}
