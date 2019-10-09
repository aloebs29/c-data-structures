#include "dhash_table.h"
#include <stdlib.h> // malloc
#include <string.h> // memset

// Value definitions
#define DEFAULT_TABLE_SIZE          64

#define GROW_AT_LOAD_FACTOR         10.0f  // Chaining isn't terrible at high load factors
#define SHRINK_AT_LOAD_FACTOR       0.1f

#define GROWTH_FACTOR               32 // should be larger than grow at load factor
#define SHRINK_FACTOR               4

// Private function prototypes
uint32_t hash(const char * key, uint32_t table_size);

// Public functions
dhash_table_t * dhash_table_new()
{
  dhash_table_t * table = (dhash_table_t *)(malloc(sizeof(dhash_table_t)));
  table->_count = 0;
  table->_table_size = DEFAULT_TABLE_SIZE;

  // Malloc data and set to null
  table->_data = (dkey_val_list_t **)(malloc(sizeof(dkey_val_list_t *) *
      table->_table_size));
  for (int i = 0; i < table->_table_size; i++)
  {
    table->_data[i] = NULL;
  }

  return table;
}

void dhash_table_destroy(dhash_table_t * table)
{
  // Free any allocated lists
  for (int i = 0; i < table->_table_size; i++)
  {
    if (table->_data[i] != NULL)
    {
      dkey_val_list_destroy(table->_data[i]);
    }
  }
  
  free(table->_data);
  free(table);
}

void dhash_table_add(dhash_table_t * table, const char * key, int value)
{
  uint32_t slot = hash(key, table->_table_size);
  if (table->_data[slot] == NULL)
  {
    table->_data[slot] = dkey_val_list_new();
  }
  
  // If key exists, update, else add
  if (!dkey_val_list_try_update(table->_data[slot], key, value))
  {
    dkey_val_list_push_back(table->_data[slot], key, value);
    table->_count++;
  }

  // TODO: Check for upsize
}

bool dhash_table_exists(dhash_table_t * table, const char * key)
{
  int ignored;
  return dhash_table_try_get(table, key, &ignored);
}

bool dhash_table_try_get(dhash_table_t * table, const char * key, int * out)
{
  uint32_t slot = hash(key, table->_table_size);
  if (table->_data[slot] == NULL)
  {
    return false;
  }
  else
  {
    return dkey_val_list_try_get(table->_data[slot], key, out);
  }
}

void dhash_table_remove(dhash_table_t * table, const char * key)
{
  uint32_t slot = hash(key, table->_table_size);
  if (table->_data[slot] != NULL)
  {
    if (dkey_val_list_remove(table->_data[slot], key))
    {
      table->_count--;
      // TODO: Check for downsize
    }
  }
}

// Private function definitions
uint32_t hash(const char * key, uint32_t table_size)
{
  // Using djb2
  uint32_t hash = 5381;
  int c;
  while (c = *key++)
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % table_size;
}