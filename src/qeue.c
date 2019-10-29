#include "qeue.h"
#include <stdlib.h> // malloc, free
#include "helpers.h" // error_print_and_quit

// To avoid crazy malloc's on qeue_new_as_array
#define QEUE_AS_ARRAY_MAX_CAP      32768

// Private function prototypes
qeue_array_t * qeue_array_new(int capacity);
void array_enqeue(qeue_data_t data, int value);
int array_deqeue(qeue_data_t data);
bool array_is_empty(qeue_data_t data);
bool array_is_full(qeue_data_t data);
void array_destroy(qeue_data_t data);

void list_enqeue(qeue_data_t data, int value);
int list_deqeue(qeue_data_t data);
bool list_is_empty(qeue_data_t data);
bool list_is_full(qeue_data_t data);
void list_destroy(qeue_data_t data);

// Public functions
qeue_t * qeue_new_as_array(int capacity)
{
  if (capacity < 2)
  {
    error_print_and_quit("qeue_new_as_array capacity must be greater than 1.");
  }
  if (capacity > QEUE_AS_ARRAY_MAX_CAP)
  {
    error_print_and_quit("qeue_new_as_array max capacity exceeded.");
  }

  qeue_t * qeue = (qeue_t *)malloc(sizeof(qeue_t));
  qeue->_data._array = qeue_array_new(capacity);
  qeue->_enqeue = array_enqeue;
  qeue->_deqeue = array_deqeue;
  qeue->_is_empty = array_is_empty;
  qeue->_is_full = array_is_full;
  qeue->_destroy = array_destroy;

  return qeue;
}

qeue_t * qeue_new_as_list()
{
  qeue_t * qeue = (qeue_t *)malloc(sizeof(qeue_t));
  qeue->_data._list = forward_list_new();
  qeue->_enqeue = list_enqeue;
  qeue->_deqeue = list_deqeue;
  qeue->_is_empty = list_is_empty;
  qeue->_is_full = list_is_full;
  qeue->_destroy = list_destroy;

  return qeue;
}

void qeue_destroy(qeue_t * qeue)
{
  qeue->_destroy(qeue->_data);
  free(qeue);
}

void qeue_enqeue(qeue_t * qeue, int value)
{
  // Prevent overflow
  if (qeue->_is_full(qeue->_data))
  {
    error_print_and_quit("qeue_enqeue overflow.");
  }

  qeue->_enqeue(qeue->_data, value);
}

int qeue_deqeue(qeue_t * qeue)
{
  // Prevent reading from empty qeue
  if (qeue->_is_empty(qeue->_data))
  {
    error_print_and_quit("qeue_deqeue from empty qeue.");
  }

  return qeue->_deqeue(qeue->_data);
}

bool qeue_is_empty(qeue_t * qeue)
{
  return qeue->_is_empty(qeue->_data);
}

bool qeue_is_full(qeue_t * qeue)
{
  return qeue->_is_full(qeue->_data);
}

// Private function definitions
qeue_array_t * qeue_array_new(int capacity)
{
  qeue_array_t * array = (qeue_array_t *)malloc(sizeof(qeue_array_t));
  array->_front = (int *)malloc(sizeof(int) * (capacity + 1)); // one unused slot
  array->_read = array->_front;
  array->_write = array->_front;
  array->_back = &array->_front[capacity];

  return array;
}

void array_enqeue(qeue_data_t data, int value)
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

int array_deqeue(qeue_data_t data)
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

bool array_is_empty(qeue_data_t data)
{
  return data._array->_read == data._array->_write;
}

bool array_is_full(qeue_data_t data)
{
  bool normal_case = ((data._array->_write + 1) == data._array->_read);
  bool edge_case = ((data._array->_write == 
      data._array->_back) && 
      (data._array->_read == data._array->_front));
  
  return normal_case || edge_case;
}

void array_destroy(qeue_data_t data)
{
  free(data._array->_front);
  free(data._array);
}

void list_enqeue(qeue_data_t data, int value)
{
  forward_list_push_back(data._list, value);
}

int list_deqeue(qeue_data_t data)
{
  return forward_list_pop_front(data._list);
}

bool list_is_empty(qeue_data_t data)
{
  return forward_list_is_empty(data._list);
}

bool list_is_full(qeue_data_t data)
{
  return false;
}

void list_destroy(qeue_data_t data)
{
  forward_list_destroy(data._list);
}