#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  printf("Data structures implemented:\n");
  printf("\t- Dynamic array (vector_t).\n");
  printf("\t- Singly-linked list (forward_list_t).\n");
  printf("\t- queue with array or singly-linked list backing (queue_t).\n");
  printf("\t- Singly-linked key/value list for use in hash table (key_val_list_t).\n");
  printf("\t- Hash table (hash_table_t).\n");
  printf("\t- Binary search tree (bst_t).\n");
  printf("\t- Max heap (max_heap_t).\n");

  printf("\nOther:\n");
  printf("\t- Sorting:\n");
  printf("\t\t- Heap sort (heap_sort)\n");
  printf("\t\t- Quick sort (quick_sort)\n");
  printf("\t\t- Merge sort (merge_sort)\n");

  printf("\nGo to root project folder, and run \"ceedling test:all\" to run tests.\n");
  printf("Press ENTER key to exit.\n");
  getchar();

  return EXIT_SUCCESS;
}