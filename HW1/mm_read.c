#include "matrix.h"
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