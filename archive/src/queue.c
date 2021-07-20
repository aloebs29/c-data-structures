#include "queue.h"
#include <stdlib.h> // malloc, free
#include "helpers.h" // error_print_and_quit

// To avoid crazy malloc's on queue_new_as_array
#define queue_AS_ARRAY_MAX_CAP      32768

// Private function prototypes
queue_array_t * queue_array_new(int capacity);
void array_enqueue(queue_data_t data, int value);
int array_dequeue(queue_data_t data);
bool array_is_empty(queue_data_t data);
bool array_is_full(queue_data_t data);
void array_destroy(queue_data_t data);

void list_enqueue(queue_data_t data, int value);
int list_dequeue(queue_data_t data);
bool list_is_empty(queue_data_t data);
bool list_is_full(queue_data_t data);
void list_destroy(queue_data_t data);

// Public functions
queue_t * queue_new_as_array(int capacity)
{
  if (capacity < 2)
  {
    error_print_and_quit("queue_new_as_array capacity must be greater than 1.");
  }
  if (capacity > queue_AS_ARRAY_MAX_CAP)
  {
    error_print_and_quit("queue_new_as_array max capacity exceeded.");
  }

  queue_t * queue = (queue_t *)malloc(sizeof(queue_t));
  queue->_data._array = queue_array_new(capacity);
  queue->_enqueue = array_enqueue;
  queue->_dequeue = array_dequeue;
  queue->_is_empty = array_is_empty;
  queue->_is_full = array_is_full;
  queue->_destroy = array_destroy;

  return queue;
}

queue_t * queue_new_as_list()
{
  queue_t * queue = (queue_t *)malloc(sizeof(queue_t));
  queue->_data._list = forward_list_new();
  queue->_enqueue = list_enqueue;
  queue->_dequeue = list_dequeue;
  queue->_is_empty = list_is_empty;
  queue->_is_full = list_is_full;
  queue->_destroy = list_destroy;

  return queue;
}

void queue_destroy(queue_t * queue)
{
  queue->_destroy(queue->_data);
  free(queue);
}

void queue_enqueue(queue_t * queue, int value)
{
  // Prevent overflow
  if (queue->_is_full(queue->_data))
  {
    error_print_and_quit("queue_enqueue overflow.");
  }

  queue->_enqueue(queue->_data, value);
}

int queue_dequeue(queue_t * queue)
{
  // Prevent reading from empty queue
  if (queue->_is_empty(queue->_data))
  {
    error_print_and_quit("queue_dequeue from empty queue.");
  }

  return queue->_dequeue(queue->_data);
}

bool queue_is_empty(queue_t * queue)
{
  return queue->_is_empty(queue->_data);
}

bool queue_is_full(queue_t * queue)
{
  return queue->_is_full(queue->_data);
}

// Private function definitions
queue_array_t * queue_array_new(int capacity)
{
  queue_array_t * array = (queue_array_t *)malloc(sizeof(queue_array_t));
  array->_front = (int *)malloc(sizeof(int) * (capacity + 1)); // one unused slot
  array->_read = array->_front;
  array->_write = array->_front;
  array->_back = &array->_front[capacity];

  return array;
}

void array_enqueue(queue_data_t data, int value)
{
  *data._array->_write = value;

  // Circular increment
  if (data._array->_write == data._array->_back)
  {
    data._array->_write = data._array->_front; 
  }
  else 
  {
    data._array->_write++;
  }
}

int array_dequeue(queue_data_t data)
{
  int val = *data._array->_read;

  // Circular increment
  if (data._array->_read == data._array->_back)
  {
    data._array->_read = data._array->_front; 
  }
  else 
  {
    data._array->_read++;
  }

  return val;
}

bool array_is_empty(queue_data_t data)
{
  return data._array->_read == data._array->_write;
}

bool array_is_full(queue_data_t data)
{
  bool normal_case = ((data._array->_write + 1) == data._array->_read);
  bool edge_case = ((data._array->_write == 
      data._array->_back) && 
      (data._array->_read == data._array->_front));
  
  return normal_case || edge_case;
}

void array_destroy(queue_data_t data)
{
  free(data._array->_front);
  free(data._array);
}

void list_enqueue(queue_data_t data, int value)
{
  forward_list_push_back(data._list, value);
}

int list_dequeue(queue_data_t data)
{
  return forward_list_pop_front(data._list);
}

bool list_is_empty(queue_data_t data)
{
  return forward_list_is_empty(data._list);
}

bool list_is_full(queue_data_t data)
{
  return false;
}

void list_destroy(queue_data_t data)
{
  forward_list_destroy(data._list);
}