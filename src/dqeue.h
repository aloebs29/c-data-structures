#ifndef _DQEUE_H
#define _DQEUE_H

#include <stdbool.h> // bool
#include "dforward_list.h" // dforward_list_*

typedef struct {
  int * _front;
  int * _back;
  int * _read;
  int * _write;
} dqeue_array_t;

typedef union {
  dqeue_array_t * _array;
  dforward_list_t * _list;
} dqeue_data_t;

typedef struct {
  dqeue_data_t _data;
  void (*_enqeue)(dqeue_data_t data, int value);
  int (*_deqeue)(dqeue_data_t data);
  bool (*_is_empty)(dqeue_data_t data);
  bool (*_is_full)(dqeue_data_t data);
  void (*_destroy)(dqeue_data_t data);
} dqeue_t;

// Creates new qeue with underlying array structure
dqeue_t * dqeue_new_as_array(int capacity);
// Creates new qeue with underlying singly-linked list structure
dqeue_t * dqeue_new_as_list();
// Destroys qeue
void dqeue_destroy(dqeue_t * qeue);
// Adds item to back of qeue
void dqeue_enqeue(dqeue_t * qeue, int value);
// Removes and returns item from front of qeue
int dqeue_deqeue(dqeue_t * qeue);
// Returns true if qeue has 0 elements
bool dqeue_is_empty(dqeue_t * qeue);
// Returns true if qeue is full
bool dqeue_is_full(dqeue_t * qeue);

#endif // _DQEUE_H
