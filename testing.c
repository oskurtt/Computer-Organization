#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define ROWS 3
#define COLS 2
#define STR_LEN 16
#define ROW_LENGTH 16

char source_of_wisdom[][COLS][STR_LEN] = {{"01", "CompOrg is cool"}, {"02", "DS is cool too"}, {"03", "Dura lex"}};

void mem_dump(void* s_addr, unsigned int length) {
  assert(s_addr >= 0);
  void *ptr = s_addr, *row_start_ptr;
  unsigned int rows = 1 + ((length - 1) / ROW_LENGTH), i, j;

  unsigned int alignment = (long long)s_addr % ROW_LENGTH;
  if (alignment != 0) {
    s_addr -= alignment;
    length += alignment;
  }

  for (i = 0; i < rows; ++i) {
    row_start_ptr = ptr;
    printf("%8llx  ", (long long unsigned int)ptr);
    for (j = 0; j < ROW_LENGTH; ++j) {
      if (j == ROW_LENGTH / 2) {
        printf(" ");
      }
      if (i * ROW_LENGTH + j < length) {
        printf("%02x ", *((unsigned char *)ptr));
        ptr++;
      }
      else {
        printf("   ");
      }
    }
    ptr = row_start_ptr;
    printf(" |");
    for (j = 0; j < ROW_LENGTH && i * ROW_LENGTH + j < length; ++j) {
      if (i * ROW_LENGTH + j < length) {
        if (isprint(*((unsigned char *)ptr))) {
          printf("%c", *((unsigned char *)ptr));
        }
        else {
          printf(".");
        }
        ptr++;
      }
      else {
        printf(" ");
      }
    }
    printf("|\n");
  }
}

int main(int argc, char **argv, char **envp)
{
  int i, j;
  char ***wisdom = (char ***)malloc(ROWS * sizeof(char**));
  for (i = 0; i < ROWS; ++i) {
    wisdom[i] = (char **)malloc(COLS * sizeof(char*));
    for (j = 0; j < COLS; ++j) {
      wisdom[i][j] = (char *)malloc(STR_LEN * sizeof(char));
      strcpy(wisdom[i][j], source_of_wisdom[i][j]);
    }
  }

  printf("Here are some facts:\n");
  for (i = 0; i < ROWS; ++i) {
    for (j = 0; j < COLS; ++j) {
      printf("%20s", wisdom[i][j]);
    }
    printf("\n");
  }

  printf("Let's look at the pointers\n");
  printf("--------------------------\n");
  printf("&wisdom: %p\n", &wisdom);
  printf("wisdom: %p\n", wisdom);
  for (i = 0; i < ROWS; ++i) {
    printf("wisdom[%d]: %p\t", i, wisdom[i]);
  }
  printf("\n");
  for (i = 0; i < ROWS; ++i) {
    for (j = 0; j < COLS; ++j) {
      printf("wisdom[%d][%d]: %p\t", i, j, wisdom[i][j]);
    }
    printf("\n");
  }

  mem_dump(wisdom, ROWS * sizeof(char**));
  printf("\n");
  for (i = 0; i < ROWS; ++i) {
    mem_dump(wisdom[i], COLS * sizeof(char*));
  }
  printf("\n");
  for (i = 0; i < ROWS; ++i) {
    for (j = 0; j < COLS; ++j) {
      mem_dump(wisdom[i][j], STR_LEN * sizeof(char));
    }
    printf("\n");
  }

  for (i = 0; i < ROWS; ++i) {
    for (j = 0; j < COLS; ++j) {
      free(wisdom[i][j]);
    }
    free(wisdom[i]);
  }
  free(wisdom);

  return EXIT_SUCCESS;
}
