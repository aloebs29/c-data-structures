#include "dforward_list.h"
#include <stdlib.h> // malloc, free
#include "helpers.h" // error_print_and_quit

// Private functions prototypes
static node_t * node_new(int value);
static void node_destroy(node_t * node);
static node_t * node_in_list_at(dforward_list_t * list, int index);

// Public functions
dforward_list_t * dforward_list_new()
{
  dforward_list_t * list = (dforward_list_t *)(malloc(sizeof(dforward_list_t)));
  list->_head = NULL;
  list->_tail = NULL;

  return list;
}

void dforward_list_destroy(dforward_list_t * list)
{
  // Free nodes
  node_t * current = list->_head;
  while (current != NULL)
  {
    node_t * _next = current->_next;
    node_destroy(current);
    current = _next;
  }

  // Free list
  free(list);
}

void dforward_list_push_front(dforward_list_t * list, int value)
{
  node_t * node = node_new(value);

  node->_next = list->_head;
  list->_head = node;

  if (list->_tail == NULL)
  {
    list->_tail = node;
  }
}

bool dforward_list_is_empty(dforward_list_t * list)
{
  return (list->_head == NULL);
}

int dforward_list_size(dforward_list_t * list)
{
  if (dforward_list_is_empty(list))
  {
    return 0;
  }
  node_t * current = list->_head;
  int size = 1;
  while (current != list->_tail)
  {
    current = current->_next;
    size++;
  }

  return size;
}

int dforward_list_at(dforward_list_t * list, int index)
{
  if (dforward_list_is_empty(list))
  {
    error_print_and_quit("dforward_list_at list argument null.");
  }
  if (index < 0)
  {
    error_print_and_quit("dforward_list_at index must be greater than 0.");
  }

  return node_in_list_at(list, index)->_key;
}

void dforward_list_push_back(dforward_list_t * list, int value)
{
  node_t * node = node_new(value);

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

int dforward_list_at_from_end(dforward_list_t * list, int index)
{
  if (dforward_list_is_empty(list))
  {
    error_print_and_quit("dforward_list_at_from_end list argument null.");
  }
  if (index < 0)
  {
    error_print_and_quit("dforward_list_at_from_end index must be greater than 0.");
  }

  // We'll use a pointer as a probe which is "index" elements ahead of our item
  node_t * probe = node_in_list_at(list, index);

  // Now we will walk the list until our probe reaches the end
  node_t * item = list->_head;
  while (probe != list->_tail)
  {
    item = item->_next;
    probe =  probe->_next;
  }
  
  return item->_key;
}

int dforward_list_pop_front(dforward_list_t * list)
{
  if (dforward_list_is_empty(list))
  {
    error_print_and_quit("dforward_list_pop_front list must contain elements.");
  }
  node_t * item = list->_head;
  list->_head = item->_next;

  int val = item->_key;
  node_destroy(item);
  return val;
}

int dforward_list_pop_back(dforward_list_t * list)
{
  if (dforward_list_is_empty(list))
  {
    error_print_and_quit("dforward_list_pop_back list must contain elements.");
  }
  int val = list->_tail->_key;
  
  node_t * current = list->_head;
  while(current->_next != list->_tail)
  {
    current = current->_next;
  }
  current->_next = NULL;
  node_destroy(list->_tail);
  list->_tail = current;

  return val;
}

int dforward_list_front(dforward_list_t * list)
{
  if (dforward_list_is_empty(list))
  {
    error_print_and_quit("dforward_list_front list must contain elements.");
  }
  return list->_head->_key;
}

int dforward_list_back(dforward_list_t * list)
{
  if (dforward_list_is_empty(list))
  {
    error_print_and_quit("dforward_list_back list must contain elements.");
  }
  return list->_tail->_key;
}

void dforward_list_insert(dforward_list_t * list, int index, int value)
{
  if (index < 0)
  {
    error_print_and_quit("dforward_list_insert index must be greater than 0.");
  }

  // Handle special case for index 0
  if (index == 0)
  {
    dforward_list_push_front(list, value);
  }

  node_t * previous_element = node_in_list_at(list, index - 1);
  node_t * new_element = node_new(value);
  new_element->_next = previous_element->_next;
  previous_element->_next = new_element;

  if (list->_tail == previous_element)
  {
    list->_tail = new_element;
  }
}

int dforward_list_delete(dforward_list_t * list, int index)
{
  if (dforward_list_is_empty(list))
  {
    error_print_and_quit("dforward_list_delete list argument null.");
  }
  if (index < 0)
  {
    error_print_and_quit("dforward_list_delete index must be greater than 0.");
  }

  int val;
  // Handle special case for index 0
  if (index == 0)
  {
    val = dforward_list_pop_front(list);
  }
  else
  {
    node_t * previous_element = node_in_list_at(list, index - 1);
    if (previous_element->_next == NULL)
    {
      error_print_and_quit("dforward_list_delete index out of range.");
    }
    node_t * deleted_element = previous_element->_next;
    val = deleted_element->_key;
    previous_element->_next = deleted_element->_next;
    if (list->_tail == deleted_element)
    {
      list->_tail = previous_element;
    }
    node_destroy(deleted_element);
  }

  return val;  
}

void dforward_list_reverse(dforward_list_t * list)
{
  int size = dforward_list_size(list);
  if (size == 0 || size == 1)
  {
    return; // reversed list is identical to current
  }

  node_t * current = list->_head;
  node_t * previous = NULL;
  // Flip head and tail
  list->_head = list->_tail;
  list->_tail = current;
  // Reverse linkages
  while (current != NULL)
  {
    node_t * next = current->_next;
    current->_next = previous;
    previous = current;
    current = next;
  }
}

void dforward_list_remove(dforward_list_t * list, int value)
{
  if (dforward_list_is_empty(list))
  {
    return; // nothing to remove
  }

  node_t * current = list->_head;
  node_t * next = current->_next;
  // Need to handle deleting the head differently
  if (current->_key == value)
  {
    // Need to handle if this is also the tail
    if (list->_tail == current)
    {
      list->_head = NULL;
      list->_tail = NULL;
      node_destroy(current);
    }
    else 
    {
      node_destroy(current);
      list->_head = next;

      current = next;
      next = current->_next;
    }
  }
  while (next != NULL)
  {
    if (next->_key == value)
    {
      current->_next = next->_next;
      // Move tail if needed
      if (list->_tail == next)
      {
        list->_tail = current;
      }
      node_destroy(next);
    }
    else
    {
      current = next;
    }
    next = current->_next;
  }
}

// Private function definitions
static node_t * node_new(int value)
{
  node_t * node = (node_t *)(malloc(sizeof(node_t)));
  node->_key = value;
  node->_next = NULL;
  return node;
}

static void node_destroy(node_t * node)
{
  free(node);
}

static node_t * node_in_list_at(dforward_list_t * list, int index)
{
  node_t * current = list->_head;
  for (int i = 0; i < index; i++)
  {
    // Guard against out of range index
    if (current->_next == NULL)
    {
      error_print_and_quit("List index out of range.");
    }
    current = current->_next;
  }
  
  return current;
}
