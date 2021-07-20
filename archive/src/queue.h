#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdbool.h> // bool
#include "forward_list.h" // forward_list_*

typedef struct {
  int * _front;
  int * _back;
  int * _read;
  int * _write;
} queue_array_t;

typedef union {
  queue_array_t * _array;
  forward_list_t * _list;
} queue_data_t;

typedef struct {
  queue_data_t _data;
  void (*_enqueue)(queue_data_t data, int value);
  int (*_dequeue)(queue_data_t data);
  bool (*_is_empty)(queue_data_t data);
  bool (*_is_full)(queue_data_t data);
  void (*_destroy)(queue_data_t data);
} queue_t;

// Creates new queue with underlying array structure
queue_t * queue_new_as_array(int capacity);
// Creates new queue with underlying singly-linked list structure
queue_t * queue_new_as_list();
// Destroys queue
void queue_destroy(queue_t * queue);
// Adds item to back of queue
void queue_enqueue(queue_t * queue, int value);
// Removes and returns item from front of queue
int queue_dequeue(queue_t * queue);
// Returns true if queue has 0 elements
bool queue_is_empty(queue_t * queue);
// Returns true if queue is full
bool queue_is_full(queue_t * queue);

#endif // _QUEUE_H
