#ifndef _DKEY_VAL_LIST_H
#define _DKEY_VAL_LIST_H

#include <stdbool.h> // bool

typedef struct {
  char * _key;
  int _value;
} key_val_pair_t;

typedef struct key_val_node {
  key_val_pair_t * _data;
  struct key_val_node * _next;
} key_val_node_t;

typedef struct {
  key_val_node_t * _head;
  key_val_node_t * _tail;
} dkey_val_list_t;


// Creates an empty list
dkey_val_list_t * dkey_val_list_new();
// Destroys list
void dkey_val_list_destroy(dkey_val_list_t * list);
// Returns true if empty
bool dkey_val_list_is_empty(dkey_val_list_t * list);
// Adds item to end of list
void dkey_val_list_push_back(dkey_val_list_t * list, const char * key,
    int value);
// Attempts to get value associated with key
bool dkey_val_list_try_get(dkey_val_list_t * list, const char * key, int * out);
// Attempts to update the value associated with key
bool dkey_val_list_try_update(dkey_val_list_t * list, const char * key, int value);
// Removes item from list if it exists
void dkey_val_list_remove(dkey_val_list_t * list, const char * key);

#endif // _DKEY_VAL_LIST_H
