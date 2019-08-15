#ifndef _DVECTOR_H
#define _DVECTOR_H

#include <stdbool.h> // bool

// Value defines, these would typically be private but we're putting them in
// the header for the test module to access
#define DEFAULT_CAPACITY        16
#define GROWTH_FACTOR           2
#define SHRINK_AT_DIVISOR       4

typedef struct {
  int _size;
  int _capacity;
  int *_data;
} dvector_t;

// Creates a vector with at least the given capacity
dvector_t *dvector_new(int capacity);
// Destroys vector
void dvector_destroy(dvector_t *vec);
// Returns capacity field (we want it treated as private by module users)
int dvector_capacity(dvector_t *vec);
// Returns size field (we want it treated as private by module users)
int dvector_size(dvector_t *vec);
// Returns true if vector has 0 elements
bool dvector_is_empty(dvector_t *vec);
// Returns item at given index, exit failure if out of range
int dvector_at(dvector_t *vec, int index);
// Adds item to end of vector
void dvector_push(dvector_t *vec, int item);
// Inserts item at selected index
void dvector_insert(dvector_t *vec, int index, int item);
// Inserts item at beginning of vector
void dvector_prepend(dvector_t *vec, int item);
// Removes item from end of vector, returns item
int dvector_pop(dvector_t *vec);
// Delete item at index, shifts all other items left
int dvector_delete(dvector_t *vec, int index);
// Removes every item matching the specified value
int dvector_remove(dvector_t *vec, int value);
// Looks for value and returns first index with that value, -1 if not found
int dvector_find(dvector_t *vec, int value);



#endif // _DVECTOR_H