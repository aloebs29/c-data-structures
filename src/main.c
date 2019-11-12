#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  printf("Data structures implemented:\n");
  printf("\t- Dynamic array (vector_t).\n");
  printf("\t- Singly-linked list (forward_list_t).\n");
  printf("\t- Qeue with array or singly-linked list backing (qeue_t).\n");
  printf("\t- Singly-linked key/value list for use in hash table (key_val_list_t).\n");
  printf("\t- Hash table (hash_table_t).\n");
  printf("\t- Binary search tree (bst_t).");
  printf("\t- Max heap (max_heap_t).");

  printf("\nGo to root project folder, and run \"ceedling test:all\" to run tests.\n");
  printf("Press ENTER key to exit.\n");
  getchar();

  return EXIT_SUCCESS;
}