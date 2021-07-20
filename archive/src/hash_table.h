#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <stdbool.h> // bool
#include <stdint.h> // int types
#include "key_val_list.h" // key_val_list_t, associated functions

typedef struct {
  uint32_t _table_size;
  uint32_t _count;
  key_val_list_t ** _data;
} hash_table_t;

// Creates an empty hash table
hash_table_t * hash_table_new();
// Destroys hash table
void hash_table_destroy(hash_table_t * table);
// Adds key value pair to hash table
void hash_table_add(hash_table_t * table, const char * key, int value);
// Checks if key value pair exists
bool hash_table_exists(hash_table_t * table, const char * key);
// Attempts to get key value pair, returns false if not found
bool hash_table_try_get(hash_table_t * table, const char * key, int * out);
// Removes key value pair from hash table
void hash_table_remove(hash_table_t * table, const char * key);

#endif // _HASH_TABLE_H
