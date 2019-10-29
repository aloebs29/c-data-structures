#ifndef _KEY_VAL_LIST_H
#define _KEY_VAL_LIST_H

#include <stdbool.h> // bool

typedef struct {
  char * key;
  int value;
} key_val_pair_t;

typedef struct key_val_node {
  key_val_pair_t * _data;
  struct key_val_node * _next;
} key_val_node_t;

typedef struct {
  key_val_node_t * _head;
  key_val_node_t * _tail;
} key_val_list_t;


// Creates an empty list
key_val_list_t * key_val_list_new();
// Destroys list
void key_val_list_destroy(key_val_list_t * list);
// Creates empty key val pair
key_val_pair_t * key_val_pair_new(const char * key, int value);
// Destroys key val pair
void key_val_pair_destroy(key_val_pair_t * pair);
// Returns true if empty
bool key_val_list_is_empty(key_val_list_t * list);
// Adds item to end of list
void key_val_list_push_back(key_val_list_t * list, const char * key,
    int value);
// Removes item from front and returns value stored there
key_val_pair_t * key_val_list_pop_front(key_val_list_t * list);
// Attempts to get value associated with key
bool key_val_list_try_get(key_val_list_t * list, const char * key, int * out);
// Attempts to update the value associated with key
bool key_val_list_try_update(key_val_list_t * list, const char * key, int value);
// Removes item from list if it exists, returns true if removed
bool key_val_list_remove(key_val_list_t * list, const char * key);

#endif // _KEY_VAL_LIST_H
