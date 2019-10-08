#include <stdio.h>
#include <stdlib.h>
#include "dvector.h"

int main(int argc, char **argv)
{
  printf("Data structures implemented:\n");
  printf("\t- Dynamic array (dvector_t).\n");
  printf("\t- Singly-linked list (dforward_list_t).\n");
  printf("\t- Qeue with array or singly-linked list backing (dqeue_t).\n");
  printf("\t- Singly-linked key/value list for use in hash table (dkey_val_list_t).\n");

  printf("\nGo to root project folder, and run \"ceedling test:all\" to run tests.\n");
  printf("Press ENTER key to exit.\n");
  getchar();

  return EXIT_SUCCESS;
}