#ifndef _DHASH_TABLE_H
#define _DHASH_TABLE_H

#include <stdbool.h> // bool
#include "dkey_val_list.h" // dkey_val_list_t, associated functions

typedef struct {
  int _capacity;
  int _count;
  dkey_val_list_t * _data;
} dhash_table_t;

// Creates an empty hash table
dhash_table_t * dhash_table_new();
// Destroys hash table
void dhash_table_destroy(dhash_table_t * table);
// Adds key value pair to hash table
void dhash_table_add(dhash_table_t * table, const char * key, int value);
// Checks if key value pair exists
bool dhash_table_exists(dhash_table_t * table, const char * key);
// Attempts to get key value pair, returns false if not found
bool dhash_table_try_get(dhash_table_t * table, const char * key, int * out);
// Removes key value pair from hash table
void dhash_table_remove(dhash_table_t * table, const char * key);

#endif // _DHASH_TABLE_H
