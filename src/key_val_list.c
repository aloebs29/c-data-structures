#include "key_val_list.h"
#include <stdlib.h> // malloc, free, NULL
#include <string.h> // strlen, strcpy, strcmp

// Private function prototypes
key_val_node_t * key_val_node_new(const char * key, int value);
void key_val_node_destroy(key_val_node_t * node);

// Public functions
key_val_list_t * key_val_list_new()
{
  key_val_list_t * list = (key_val_list_t *)(malloc(sizeof(key_val_list_t)));
  list->_head = NULL;
  list->_tail = NULL;

  return list;
}

void key_val_list_destroy(key_val_list_t * list)
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

key_val_pair_t * key_val_pair_new(const char * key, int value)
{
  key_val_pair_t * pair = (key_val_pair_t *)(malloc(sizeof(key_val_pair_t)));

  pair->key = (char *)(malloc(strlen(key) + 1));
  strcpy(pair->key, key);

  pair->value = value;

  return pair;
}

void key_val_pair_destroy(key_val_pair_t *pair)
{
  free(pair->key);
  free(pair);
}

bool key_val_list_is_empty(key_val_list_t * list)
{
  return (list->_head == NULL);
}

void key_val_list_push_back(key_val_list_t * list, const char * key,
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

key_val_pair_t * key_val_list_pop_front(key_val_list_t * list)
{
  if (key_val_list_is_empty(list))
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

  key_val_pair_t * pair = key_val_pair_new(item->_data->key, 
      item->_data->value);

  key_val_node_destroy(item);
  return pair;
}

bool key_val_list_try_get(key_val_list_t * list, const char * key, int * out)
{
  if (key_val_list_is_empty(list))
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

bool key_val_list_try_update(key_val_list_t * list, const char * key, int value)
{
  if (key_val_list_is_empty(list))
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

bool key_val_list_remove(key_val_list_t * list, const char *key)
{
  if (key_val_list_is_empty(list))
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

  node->_data = key_val_pair_new(key, value);
  node->_next = NULL;
  return node;
}

void key_val_node_destroy(key_val_node_t * node)
{
  key_val_pair_destroy(node->_data);
  free(node);
}