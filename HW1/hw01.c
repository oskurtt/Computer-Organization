/* 
Assignment 1: NxM Matrix Multiplication 
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

/******************************************************************************/
/* Function Calls */
/******************************************************************************/

typedef struct{
  double** data;
  int rows;
  int cols;
} matrix; 

int mm_alloc(matrix* mat);
int mm_free (matrix* mat);
int mm_print(matrix* mat);
int mm_read (char* filename, matrix* mat);
matrix* mm_matrix_mult(matrix* mat1, matrix* mat2);


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

int mm_read(char* filename, matrix* mat) 
{
/*
Notes:
  Use fopen to open the matrix file, then use fgets or getline to read lines
  The first line of the matrix contains integers for "#rows #columns"
  - Use these to allocate your arrays for matrix data
  Each subsequent line contains a single data value
  - You can use sscanf to parse each matrix entry
  - First value is data[0][0], next is data[0][1] ... data[#rows-1][#columns-1]
*/
  int i, j;
  FILE *file;
  file = fopen(filename, "r");
//check if file exists
  if(!file){
    printf("mm_read: failed to open file.\n");
    exit(-1);
  }
  //do a quick peek through the content and check if there are contents
  fseek (file, 0, SEEK_END);
  if (ftell(file) == 0){
    printf("mm read: failed to read from file.\n");
    exit(-1);
  }
  //reopen the file because fseek will make you reach end of file
  file = fopen(filename, "r");

  //get the first line for the number of rows and columns
  char line[256];
  if (sscanf(fgets(line, sizeof(line), file), "%d %d", &mat->rows, &mat->cols) == 1){
    printf("mm_read: failed to read matrix dimensions.");
    exit(-1);
  }
  //allocate the matrix
  mm_alloc(mat);
  
  //get data for the matrices and check whether or not there are enough values to read
  for (i = 0; i < mat->rows; i++){
    for (j = 0; j < mat->cols; j++){
      if (fgets(line, sizeof(line), file) != NULL){
        sscanf(line, "%lf", &mat->data[i][j]);
      }
      //if NULL is returned, that means there is no value
      else if (fgets(line, sizeof(line), file) == NULL){
        printf("mm_read: failed to read matrix values.");
        exit(-1);
        if (sscanf(line, "%lf", &mat->data[i][j]) != 1){
          printf("mm_read: failed to read matrix values.");
          exit(-1);
        }
      }
    }
  }

  fclose(file);

  
  return 0;
}

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

int main()
{
  /*
   * you must keep this function AS IS. 
   * you cannot modify it!
   */
  char filename_A[256];
  char filename_B[256];
  matrix* A = (matrix*)malloc(sizeof(matrix));
  matrix* B = (matrix*)malloc(sizeof(matrix));
  matrix* C = NULL;

  printf("Please enter the matrix A data file name: ");
  if (1 != scanf("%s", filename_A)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_A, A);
  mm_print(A);
  
  printf("Please enter the matrix B data file name: ");
  if (1 != scanf("%s", filename_B)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_B, B);
  mm_print(B);
  
  C = mm_matrix_mult(A, B);
  mm_print(C);

  mm_free(A);
  mm_free(B);
  mm_free(C);
  free(A);
  free(B);
  free(C);
  
  return 0;
}
