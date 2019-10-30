#include "hash_table.h"
#include <stdlib.h> // malloc
#include <string.h> // memset

// Value definitions
#define DEFAULT_TABLE_SIZE          64

#define GROW_AT_LOAD_FACTOR         8
#define SHRINK_AT_LOAD_DIVISOR      8

#define GROWTH_FACTOR               32 // should be larger than grow at load factor
#define SHRINK_FACTOR               4

// Private function prototypes
static hash_table_t * new_table(int table_size);
static uint32_t hash(const char * key, uint32_t table_size);
static void upsize_if_needed(hash_table_t * table);
static void downsize_if_needed(hash_table_t * table);
static void rehash_table(hash_table_t * table, uint32_t new_table_size);

// Public functions
hash_table_t * hash_table_new()
{
  return new_table(DEFAULT_TABLE_SIZE);
}

void hash_table_destroy(hash_table_t * table)
{
  // Free any allocated lists
  for (int i = 0; i < table->_table_size; i++)
  {
    if (table->_data[i])
    {
      key_val_list_destroy(table->_data[i]);
    }
  }
  
  free(table->_data);
  free(table);
}

void hash_table_add(hash_table_t * table, const char * key, int value)
{
  uint32_t slot = hash(key, table->_table_size);
  if (!table->_data[slot])
  {
    table->_data[slot] = key_val_list_new();
  }
  
  // If key exists, update, else add
  if (!key_val_list_try_update(table->_data[slot], key, value))
  {
    key_val_list_push_back(table->_data[slot], key, value);
    table->_count++;
    upsize_if_needed(table);
  }
}

bool hash_table_exists(hash_table_t * table, const char * key)
{
  int ignored;
  return hash_table_try_get(table, key, &ignored);
}

bool hash_table_try_get(hash_table_t * table, const char * key, int * out)
{
  uint32_t slot = hash(key, table->_table_size);
  if (!table->_data[slot])
  {
    return false;
  }
  else
  {
    return key_val_list_try_get(table->_data[slot], key, out);
  }
}

void hash_table_remove(hash_table_t * table, const char * key)
{
  uint32_t slot = hash(key, table->_table_size);
  if (table->_data[slot])
  {
    if (key_val_list_remove(table->_data[slot], key))
    {
      table->_count--;
      downsize_if_needed(table);
    }
  }
}

// Private function definitions
static hash_table_t * new_table(int table_size)
{
  hash_table_t * table = (hash_table_t *)(malloc(sizeof(hash_table_t)));
  table->_count = 0;
  table->_table_size = table_size;

  // Malloc data and set to null
  table->_data = (key_val_list_t **)(malloc(sizeof(key_val_list_t *) *
      table_size));
  for (int i = 0; i < table_size; i++)
  {
    table->_data[i] = NULL;
  }

  return table;
}

static uint32_t hash(const char * key, uint32_t table_size)
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

static void upsize_if_needed(hash_table_t * table)
{
  int load_factor = table->_count / table->_table_size;
  if (load_factor >= GROW_AT_LOAD_FACTOR)
  {
    rehash_table(table, table->_table_size * GROWTH_FACTOR);
  }
}

static void downsize_if_needed(hash_table_t * table)
{
  int load_divisor = table->_table_size / table->_count; // inverse of load factor
  if ((load_divisor >= SHRINK_AT_LOAD_DIVISOR) &&
      (table->_table_size > DEFAULT_TABLE_SIZE)) // default will serve as the minimum
  {
    rehash_table(table, table->_table_size / SHRINK_FACTOR);
  }
}

static void rehash_table(hash_table_t * table, uint32_t new_table_size)
{
  // Malloc new data and set to null
  key_val_list_t ** new_data = (key_val_list_t **)
      (malloc(sizeof(key_val_list_t *) * new_table_size));
  for (int i = 0; i < new_table_size; i++)
  {
    new_data[i] = NULL;
  }

  // Iterate through old data
  for (int i = 0; i < table->_table_size; i++)
  {
    if (table->_data[i])
    {
      if (!key_val_list_is_empty(table->_data[i]))
      {
        key_val_pair_t * current = key_val_list_pop_front(table->_data[i]);
        while (current)
        {
          // Add to new table
          uint32_t slot = hash(current->key, new_table_size);
          if (!new_data[slot])
          {
            new_data[slot] = key_val_list_new();
          }
          key_val_list_push_back(new_data[slot], current->key, current->value);

          key_val_pair_destroy(current);
          current = key_val_list_pop_front(table->_data[i]);
        }
      }

      key_val_list_destroy(table->_data[i]);
    }
  }

  free(table->_data);
  table->_data = new_data;
  table->_table_size = new_table_size;
}