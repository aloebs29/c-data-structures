#include "dqeue.h"
#include <stdlib.h> // malloc, free
#include "helpers.h" // error_print_and_quit

// To avoid crazy malloc's on dqeue_new_as_array
#define DQEUE_AS_ARRAY_MAX_CAP      32768

// Private function prototypes
dqeue_array_t * dqeue_array_new(int capacity);
void array_enqeue(dqeue_data_t data, int value);
int array_deqeue(dqeue_data_t data);
bool array_is_empty(dqeue_data_t data);
bool array_is_full(dqeue_data_t data);
void array_destroy(dqeue_data_t data);

void list_enqeue(dqeue_data_t data, int value);
int list_deqeue(dqeue_data_t data);
bool list_is_empty(dqeue_data_t data);
bool list_is_full(dqeue_data_t data);
void list_destroy(dqeue_data_t data);

// Public functions
dqeue_t * dqeue_new_as_array(int capacity)
{
  if (capacity < 2)
  {
    error_print_and_quit("dqeue_new_as_array capacity must be greater than 1.");
  }
  if (capacity > DQEUE_AS_ARRAY_MAX_CAP)
  {
    error_print_and_quit("dqeue_new_as_array max capacity exceeded.");
  }

  dqeue_t * qeue = (dqeue_t *)malloc(sizeof(dqeue_t));
  qeue->_data._array = dqeue_array_new(capacity);
  qeue->_enqeue = array_enqeue;
  qeue->_deqeue = array_deqeue;
  qeue->_is_empty = array_is_empty;
  qeue->_is_full = array_is_full;
  qeue->_destroy = array_destroy;

  return qeue;
}

dqeue_t * dqeue_new_as_list()
{
  dqeue_t * qeue = (dqeue_t *)malloc(sizeof(dqeue_t));
  qeue->_data._list = dforward_list_new();
  qeue->_enqeue = list_enqeue;
  qeue->_deqeue = list_deqeue;
  qeue->_is_empty = list_is_empty;
  qeue->_is_full = list_is_full;
  qeue->_destroy = list_destroy;

  return qeue;
}

void dqeue_destroy(dqeue_t * qeue)
{
  qeue->_destroy(qeue->_data);
  free(qeue);
}

void dqeue_enqeue(dqeue_t * qeue, int value)
{
  // Prevent overflow
  if (qeue->_is_full(qeue->_data))
  {
    error_print_and_quit("dqeue_enqeue overflow.");
  }

  qeue->_enqeue(qeue->_data, value);
}

int dqeue_deqeue(dqeue_t * qeue)
{
  // Prevent reading from empty qeue
  if (qeue->_is_empty(qeue->_data))
  {
    error_print_and_quit("dqeue_deqeue from empty qeue.");
  }

  return qeue->_deqeue(qeue->_data);
}

bool dqeue_is_empty(dqeue_t * qeue)
{
  return qeue->_is_empty(qeue->_data);
}

bool dqeue_is_full(dqeue_t * qeue)
{
  return qeue->_is_full(qeue->_data);
}

// Private function definitions
dqeue_array_t * dqeue_array_new(int capacity)
{
  dqeue_array_t * array = (dqeue_array_t *)malloc(sizeof(dqeue_array_t));
  array->_front = (int *)malloc(sizeof(int) * (capacity + 1)); // one unused slot
  array->_read = array->_front;
  array->_write = array->_front;
  array->_back = &array->_front[capacity];

  return array;
}

void array_enqeue(dqeue_data_t data, int value)
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

int array_deqeue(dqeue_data_t data)
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

bool array_is_empty(dqeue_data_t data)
{
  return data._array->_read == data._array->_write;
}

bool array_is_full(dqeue_data_t data)
{
  bool normal_case = ((data._array->_write + 1) == data._array->_read);
  bool edge_case = ((data._array->_write == 
      data._array->_back) && 
      (data._array->_read == data._array->_front));
  
  return normal_case || edge_case;
}

void array_destroy(dqeue_data_t data)
{
  free(data._array->_front);
  free(data._array);
}

void list_enqeue(dqeue_data_t data, int value)
{
  dforward_list_push_back(data._list, value);
}

int list_deqeue(dqeue_data_t data)
{
  return dforward_list_pop_front(data._list);
}

bool list_is_empty(dqeue_data_t data)
{
  return dforward_list_is_empty(data._list);
}

bool list_is_full(dqeue_data_t data)
{
  return false;
}

void list_destroy(dqeue_data_t data)
{
  dforward_list_destroy(data._list);
}