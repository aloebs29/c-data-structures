#include "dvector.h"
#include <stdlib.h> // malloc, free


// Private function prototypes
static int calculate_capacity(int capacity);
static void resize(dvector *vec, int new_capacity);
// Grows vector if needed to add item
static void secure_capacity_for_add(dvector *vec);
// Shrinks vector if _size is less than capacity/SHRINK_AT_DIVISOR 
static void reduce_capacity_if_able(dvector *vec);

// Public functions
dvector *dvector_new(int capacity)
{
  int calculated_cap = calculate_capacity(capacity);

  dvector *vec = (dvector *)malloc(sizeof(dvector));
  vec->_size = 0;
  vec->_capacity = calculated_cap;
  vec->_data = (int *)malloc(sizeof(int) * calculated_cap);

  return vec;
}

void dvector_destroy(dvector *vec)
{
  free(vec->_data);
  free(vec);
}

int dvector_capacity(dvector *vec)
{
  return vec->_capacity;
}

int dvector_size(dvector *vec)
{
  return vec->_size;
}

bool dvector_is_empty(dvector *vec)
{
  return vec->_size == 0;
}

int dvector_at(dvector *vec, int index)
{
  // Validate index
  if ((index < 0) || (index >= vec->_size))
  {
    exit(EXIT_FAILURE);
  }

  int * address = (int *)(vec->_data + index);
  return *address;
}

void dvector_push(dvector *vec, int item)
{
  secure_capacity_for_add(vec);
  // Add item, increment _size
  int * address = (int *)(vec->_data + vec->_size);
  *address = item;
  vec->_size++;
}

void dvector_insert(dvector *vec, int index, int item)
{
  // Validate index (we will allow inserting onto the end (essentially a push))
  if ((index < 0) || (index > vec->_size))
  {
    exit(EXIT_FAILURE);
  }

  secure_capacity_for_add(vec);
  // Shift items right
  for (int i = vec->_size; i > index; i--)
  {
    int * address = (int *)(vec->_data + i);
    *address = *((int *)(address - 1));
  }
  // Increment _size
  vec->_size++;
  // Add item at index
  int * address = (int *)(vec->_data + index);
  *address = item;
}

void dvector_prepend(dvector *vec, int item)
{
  dvector_insert(vec, 0, item);
}

int dvector_pop(dvector *vec)
{
  // Store value to be popped
  int * address = (int *)(vec->_data + vec->_size - 1);
  int value = *address;

  // Reduce size
  vec->_size--;
  reduce_capacity_if_able(vec);

  return value;
}

int dvector_delete(dvector *vec, int index)
{
  // Validate index
  if ((index < 0) || (index >= vec->_size))
  {
    exit(EXIT_FAILURE);
  }
  // We need to return the item we delete
  int * item_to_delete_address = (int *)(vec->_data + index);
  int item_to_delete_value = *item_to_delete_address;
  // Shift items left
  for (int i = index; i < vec->_size; i ++)
  {
    int * address = (int *)(vec->_data + i);
    *address = *((int *)(address + 1));
  }
  // Reduce size
  vec->_size--;
  reduce_capacity_if_able(vec);

  return item_to_delete_value;
}

int dvector_remove(dvector *vec, int value)
{
  for (int i = 0; i < vec->_size; i ++)
  {
    int * address = (int *)(vec->_data + i);
    if (*address == value) 
    {
      dvector_delete(vec, i);
      // This will shift left, so we need to decrement i manually
      i--;
    }
  }

  // No need to reduce capacity since our dvector_delete call will do that 
  // internally
}

int dvector_find(dvector *vec, int value)
{
  for (int i = 0; i < vec->_size; i ++)
  {
    int * address = (int *)(vec->_data + i);
    if (*address == value) 
    {
      return i;
    }
  }

  // Did not find value
  return -1;
}


// Private function definitions
static int calculate_capacity(int capacity)
{
  int calculated_cap = DEFAULT_CAPACITY;
  while (calculated_cap < capacity) {
    calculated_cap *= GROWTH_FACTOR;
  }
  return calculated_cap;
}
static void resize(dvector *vec, int new_capacity)
{
  int * new_data = (int *)malloc(sizeof(int) * new_capacity);
  for (int i = 0; i < vec->_size; i ++)
  {
    int * source_address = (int *)(vec->_data + i);
    int * dest_address = (int *)(new_data + i);
    *dest_address = *source_address;
  }
  free(vec->_data);
  vec->_data = new_data;
  vec->_capacity = new_capacity;
}
static void secure_capacity_for_add(dvector *vec)
{
  if (vec->_size == vec->_capacity)
  {
    int new_cap = vec->_capacity * GROWTH_FACTOR;
    resize(vec, new_cap);
  }
}
static void reduce_capacity_if_able(dvector *vec)
{
  if ((vec->_size <= (vec->_capacity / SHRINK_AT_DIVISOR)) &&
      (vec->_capacity != DEFAULT_CAPACITY))
  {
    resize(vec, (vec->_capacity / GROWTH_FACTOR));
  }
}