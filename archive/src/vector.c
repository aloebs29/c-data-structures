#include "vector.h"
#include <stdlib.h> // malloc, free
#include "helpers.h" // error_print_and_quit


#define DEFAULT_CAPACITY        16
#define GROWTH_FACTOR           2
#define SHRINK_AT_DIVISOR       4

// Private function prototypes
static int calculate_capacity(int capacity);
static void resize(vector_t *vec, int new_capacity);
// Grows vector if needed to add item
static void secure_capacity_for_add(vector_t *vec);
// Shrinks vector if _size is less than capacity/SHRINK_AT_DIVISOR 
static void reduce_capacity_if_able(vector_t *vec);

// Public functions
vector_t *vector_new(int capacity)
{
  int calculated_cap = calculate_capacity(capacity);

  vector_t *vec = (vector_t *)malloc(sizeof(vector_t));
  vec->_size = 0;
  vec->_capacity = calculated_cap;
  vec->_data = (int *)malloc(sizeof(int) * calculated_cap);

  return vec;
}

void vector_destroy(vector_t *vec)
{
  free(vec->_data);
  free(vec);
}

int vector_capacity(vector_t *vec)
{
  return vec->_capacity;
}

int vector_size(vector_t *vec)
{
  return vec->_size;
}

bool vector_is_empty(vector_t *vec)
{
  return vec->_size == 0;
}

int vector_at(vector_t *vec, int index)
{
  // Validate index
  if ((index < 0) || (index >= vec->_size))
  {
    error_print_and_quit("vector_at index out of range.");
  }

  int * address = (int *)(vec->_data + index);
  return *address;
}

void vector_push(vector_t *vec, int item)
{
  secure_capacity_for_add(vec);
  // Add item, increment _size
  int * address = (int *)(vec->_data + vec->_size);
  *address = item;
  vec->_size++;
}

void vector_insert(vector_t *vec, int index, int item)
{
  // Validate index (we will allow inserting onto the end (essentially a push))
  if ((index < 0) || (index > vec->_size))
  {
    error_print_and_quit("vector_insert index out of range.");
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

void vector_prepend(vector_t *vec, int item)
{
  vector_insert(vec, 0, item);
}

int vector_pop(vector_t *vec)
{
  // Store value to be popped
  int * address = (int *)(vec->_data + vec->_size - 1);
  int value = *address;

  // Reduce size
  vec->_size--;
  reduce_capacity_if_able(vec);

  return value;
}

int vector_delete(vector_t *vec, int index)
{
  // Validate index
  if ((index < 0) || (index >= vec->_size))
  {
    error_print_and_quit("vector_delete index out of range.");
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

int vector_remove(vector_t *vec, int value)
{
  for (int i = 0; i < vec->_size; i ++)
  {
    int * address = (int *)(vec->_data + i);
    if (*address == value) 
    {
      vector_delete(vec, i);
      // This will shift left, so we need to decrement i manually
      i--;
    }
  }

  // No need to reduce capacity since our vector_delete call will do that 
  // internally
}

int vector_find(vector_t *vec, int value)
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
static void resize(vector_t *vec, int new_capacity)
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
static void secure_capacity_for_add(vector_t *vec)
{
  if (vec->_size == vec->_capacity)
  {
    int new_cap = vec->_capacity * GROWTH_FACTOR;
    resize(vec, new_cap);
  }
}
static void reduce_capacity_if_able(vector_t *vec)
{
  if ((vec->_size <= (vec->_capacity / SHRINK_AT_DIVISOR)) &&
      (vec->_capacity != DEFAULT_CAPACITY))
  {
    resize(vec, (vec->_capacity / GROWTH_FACTOR));
  }
}
