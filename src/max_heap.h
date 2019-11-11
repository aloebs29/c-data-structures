#ifndef _MAX_HEAP_H
#define _MAX_HEAP_H

#include <stdbool.h> // bool

typedef struct {
  int * _data;
  int _capacity;
  int _size;
} max_heap_t;

// Creates empty max heap with the given capacity
max_heap_t * max_heap_new(int capacity);
// Destroys heap
void max_heap_destroy(max_heap_t * heap);
// Returns true if heap is empty
bool max_heap_is_empty(max_heap_t * heap);
// Adds value to heap if heap has capacity (returns true), returns false
// if heap does not have capacity for insert
bool max_heap_try_insert(max_heap_t * heap, int value);
// Returns the max value, without removing
int max_heap_get_max(max_heap_t * heap);
// Returns the max value, removes from heap
int max_heap_extract_max(max_heap_t * heap);
// Creates max heap from array of elements
max_heap_t * max_heapify(int * arr, int len);
// Sorts array
int * max_heap_sort(int * arr, int len);

#endif // _MAX_HEAP_H
