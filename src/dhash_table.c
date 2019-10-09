#include "dhash_table.h"
#include <stdlib.h> // malloc
#include <string.h> // memset

// Value definitions
#define DEFAULT_TABLE_SIZE          64

#define GROW_AT_LOAD_FACTOR         8
#define SHRINK_AT_LOAD_DIVISOR      8

#define GROWTH_FACTOR               32 // should be larger than grow at load factor
#define SHRINK_FACTOR               4

// Private function prototypes
static dhash_table_t * new_table(int table_size);
static uint32_t hash(const char * key, uint32_t table_size);
static void upsize_if_needed(dhash_table_t * table);
static void downsize_if_needed(dhash_table_t * table);

// Public functions
dhash_table_t * dhash_table_new()
{
  return new_table(DEFAULT_TABLE_SIZE);
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
static dhash_table_t * new_table(int table_size)
{
  dhash_table_t * table = (dhash_table_t *)(malloc(sizeof(dhash_table_t)));
  table->_count = 0;
  table->_table_size = table_size;

  // Malloc data and set to null
  table->_data = (dkey_val_list_t **)(malloc(sizeof(dkey_val_list_t *) *
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

static void upsize_if_needed(dhash_table_t * table)
{
  int load_factor = table->_count / table->_table_size;
  if (load_factor >= GROW_AT_LOAD_FACTOR)
  {
    dhash_table_t * grown_table = new_table(table->_table_size * GROWTH_FACTOR);

    // Rehash
    for (int i = 0; i < table->_table_size; i++)
    {
      if (table->_data[i] != NULL)
      {
        //key_val_pair_t * current = dkey_val_list_pop
      }
    }
  }
}

static void downsize_if_needed(dhash_table_t * table)
{

}