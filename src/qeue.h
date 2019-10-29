#ifndef _QEUE_H
#define _QEUE_H

#include <stdbool.h> // bool
#include "forward_list.h" // forward_list_*

typedef struct {
  int * _front;
  int * _back;
  int * _read;
  int * _write;
} qeue_array_t;

typedef union {
  qeue_array_t * _array;
  forward_list_t * _list;
} qeue_data_t;

typedef struct {
  qeue_data_t _data;
  void (*_enqeue)(qeue_data_t data, int value);
  int (*_deqeue)(qeue_data_t data);
  bool (*_is_empty)(qeue_data_t data);
  bool (*_is_full)(qeue_data_t data);
  void (*_destroy)(qeue_data_t data);
} qeue_t;

// Creates new qeue with underlying array structure
qeue_t * qeue_new_as_array(int capacity);
// Creates new qeue with underlying singly-linked list structure
qeue_t * qeue_new_as_list();
// Destroys qeue
void qeue_destroy(qeue_t * qeue);
// Adds item to back of qeue
void qeue_enqeue(qeue_t * qeue, int value);
// Removes and returns item from front of qeue
int qeue_deqeue(qeue_t * qeue);
// Returns true if qeue has 0 elements
bool qeue_is_empty(qeue_t * qeue);
// Returns true if qeue is full
bool qeue_is_full(qeue_t * qeue);

#endif // _QEUE_H
