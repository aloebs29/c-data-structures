#include "dkey_val_list.h"
#include <stdlib.h> // malloc, free, NULL
#include <string.h> // strlen, strcpy, strcmp

// Private function prototypes
key_val_node_t * key_val_node_new(const char * key, int value);
void key_val_node_destroy(key_val_node_t * node);

// Public functions
dkey_val_list_t * dkey_val_list_new()
{
  dkey_val_list_t * list = (dkey_val_list_t *)(malloc(sizeof(dkey_val_list_t)));
  list->_head = NULL;
  list->_tail = NULL;

  return list;
}

void dkey_val_list_destroy(dkey_val_list_t * list)
{
  // Free nodes
  key_val_node_t * current = list->_head;
  while (current != NULL)
  {
    key_val_node_t * next = current->_next;
    key_val_node_destroy(current);
    current = next;
  }

  free(list);
}

dkey_val_pair_t * dkey_val_pair_new(const char * key, int value)
{
  dkey_val_pair_t * pair = (dkey_val_pair_t *)(malloc(sizeof(dkey_val_pair_t)));

  pair->key = (char *)(malloc(strlen(key) + 1));
  strcpy(pair->key, key);

  pair->value = value;

  return pair;
}

void dkey_val_pair_destroy(dkey_val_pair_t *pair)
{
  free(pair->key);
  free(pair);
}

bool dkey_val_list_is_empty(dkey_val_list_t * list)
{
  return (list->_head == NULL);
}

void dkey_val_list_push_back(dkey_val_list_t * list, const char * key,
    int value)
{
  key_val_node_t * node = key_val_node_new(key, value);

  if (list->_tail == NULL)
  {
    list->_head = node;
    list->_tail = node;
  }
  else
  {
    list->_tail->_next = node;
    list->_tail = node;
  }
}

dkey_val_pair_t * dkey_val_list_pop_front(dkey_val_list_t * list)
{
  if (dkey_val_list_is_empty(list))
  {
    return NULL;
  }
  key_val_node_t * item = list->_head;
  if (item->_next == NULL)
  {
    list->_head = NULL;
    list->_tail = NULL;
  }
  else
  {
    list->_head = item->_next;
  }

  dkey_val_pair_t * pair = dkey_val_pair_new(item->_data->key, 
      item->_data->value);

  key_val_node_destroy(item);
  return pair;
}

bool dkey_val_list_try_get(dkey_val_list_t * list, const char * key, int * out)
{
  if (dkey_val_list_is_empty(list))
  {
    return false;
  }

  key_val_node_t *current = list->_head;
  key_val_node_t *next = current->_next;

  // Check head
  if (strcmp(current->_data->key, key) == 0)
  {
    *out = current->_data->value;
    return true;
  }
  // Check rest of list
  while (next != NULL)
  {
    if (strcmp(next->_data->key, key) == 0)
    {
      *out = next->_data->value;
      return true;
    }
    current = next;
    next = current->_next;
  }

  // If we've reached this point, there was no match
  return false;
}

bool dkey_val_list_try_update(dkey_val_list_t * list, const char * key, int value)
{
  if (dkey_val_list_is_empty(list))
  {
    return false;
  }

  key_val_node_t *current = list->_head;
  key_val_node_t *next = current->_next;

  // Check head
  if (strcmp(current->_data->key, key) == 0)
  {
    current->_data->value = value;
    return true;
  }
  // Check rest of list
  while (next != NULL)
  {
    if (strcmp(next->_data->key, key) == 0)
    {
      next->_data->value = value;
      return true;
    }
    current = next;
    next = current->_next;
  }

  // If we've reached this point, there was no match
  return false;
}

bool dkey_val_list_remove(dkey_val_list_t * list, const char *key)
{
  if (dkey_val_list_is_empty(list))
  {
    return false;
  }

  key_val_node_t *current = list->_head;
  key_val_node_t *next = current->_next;

  // Need to handle deleting the head differently
  if (strcmp(current->_data->key, key) == 0)
  {
    // Need to handle if this is also the tail
    if (list->_tail == current)
    {
      list->_head = NULL;
      list->_tail = NULL;
    }
    else
    {
      list->_head = next;
    }
    key_val_node_destroy(current);
    return true;
  }

  // Wasn't the head, start searching rest of list
  while (next != NULL)
  {
    if (strcmp(next->_data->key, key) == 0)
    {
      current->_next = next->_next;
      // Move tail if needed
      if (list->_tail == next)
      {
        list->_tail = current;
      }
      key_val_node_destroy(next);
      return true;
    }
    current = next;
    next = current->_next;
  }

  // Didn't find the value
  return false;
}


// Private function definitions
key_val_node_t * key_val_node_new(const char * key, int value)
{
  key_val_node_t * node = (key_val_node_t *)(malloc(sizeof(key_val_node_t)));

  node->_data = dkey_val_pair_new(key, value);
  node->_next = NULL;
  return node;
}

void key_val_node_destroy(key_val_node_t * node)
{
  dkey_val_pair_destroy(node->_data);
  free(node);
}