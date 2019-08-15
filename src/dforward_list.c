#include "dforward_list.h"
#include <stdlib.h> // malloc, free

dforward_list_t * dforward_list_new()
{
  dforward_list_t * list = (dforward_list_t *)(malloc(sizeof(dforward_list_t)));
  list->head = NULL;
  list->tail = NULL;

  return list;
}

void dforward_list_destroy(dforward_list_t * list)
{
  // Free nodes
  node_t * current = list->head;
  while (current != NULL)
  {
    node_t * next = current->next;
    free(current);
    current = next;
  }

  // Free list
  free(list);
}

