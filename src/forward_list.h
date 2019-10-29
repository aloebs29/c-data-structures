#ifndef _FORWARD_LIST_H
#define _FORWARD_LIST_H

#include <stdbool.h> // bool

typedef struct node {
  int _key;
  struct node *_next;
} node_t;

typedef struct {
  node_t * _head;
  node_t * _tail;
} forward_list_t;

// Creates an empty forward list
forward_list_t * forward_list_new();
// Destroys list
void forward_list_destroy(forward_list_t * list);
// Adds item to front of list
void forward_list_push_front(forward_list_t * list, int value);
// Returns true if empty
bool forward_list_is_empty(forward_list_t * list);
// Returns number of elements in list
int forward_list_size(forward_list_t * list);
// Returns value of item at index
int forward_list_at(forward_list_t * list, int index);
// Adds item to end of list
void forward_list_push_back(forward_list_t * list, int value);
// Returns value of item at index from end
int forward_list_at_from_end(forward_list_t * list, int index);
// Removes item from front and returns value stored there
int forward_list_pop_front(forward_list_t * list);
// Removes item from end and returns value stored there
int forward_list_pop_back(forward_list_t * list);
// Returns item at the front of the list
int forward_list_front(forward_list_t * list);
// Returns item at the end of the list
int forward_list_back(forward_list_t * list);
// Inserts item at index
void forward_list_insert(forward_list_t * list, int index, int value);
// Deletes node at given index and returns value stored there
int forward_list_delete(forward_list_t * list, int index);
// Reverses list
void forward_list_reverse(forward_list_t * list);
// Removes all items in list with value
void forward_list_remove(forward_list_t * list, int value);

#endif // _FORWARD_LIST_H
