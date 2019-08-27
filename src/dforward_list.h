#ifndef _DFORWARD_LIST_H
#define _DFORWARD_LIST_H

#include <stdbool.h> // bool

typedef struct node {
  int _key;
  struct node *_next;
} node_t;

typedef struct {
  node_t * _head;
  node_t * _tail;
} dforward_list_t;

// Creates an empty forward list
dforward_list_t * dforward_list_new();
// Destroys list
void dforward_list_destroy(dforward_list_t * list);
// Adds item to front of list
void dforward_list_push_front(dforward_list_t * list, int value);
// Returns true if empty
bool dforward_list_is_empty(dforward_list_t * list);
// Returns number of elements in list
int dforward_list_size(dforward_list_t * list);
// Returns value of item at index
int dforward_list_at(dforward_list_t * list, int index);
// Adds item to end of list
void dforward_list_push_back(dforward_list_t * list, int value);
// Returns value of item at index from end
int dforward_list_at_from_end(dforward_list_t * list, int index);
// Removes item from front and returns value stored there
int dforward_list_pop_front(dforward_list_t * list);
// Removes item from end and returns value stored there
int dforward_list_pop_back(dforward_list_t * list);
// Returns item at the front of the list
int dforward_list_front(dforward_list_t * list);
// Returns item at the end of the list
int dforward_list_back(dforward_list_t * list);
// Inserts item at index
void dforward_list_insert(dforward_list_t * list, int index, int value);
// Deletes node at given index and returns value stored there
int dforward_list_delete(dforward_list_t * list, int index);
// Reverses list
void dforward_list_reverse(dforward_list_t * list);
// Removes all items in list with value
void dforward_list_remove(dforward_list_t * list, int value);

#endif // _DFORWARD_LIST_H
