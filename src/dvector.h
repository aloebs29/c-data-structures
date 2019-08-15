#ifndef DVECTOR_H
#define DVECTOR_H

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
} dvector;

// Creates a vector with at least the given capacity
dvector *dvector_new(int capacity);
// Destroys vector
void dvector_destroy(dvector *vec);
// Returns capacity field (we want it treated as private by module users)
int dvector_capacity(dvector *vec);
// Returns size field (we want it treated as private by module users)
int dvector_size(dvector *vec);
// Returns true if vector has 0 elements
bool dvector_is_empty(dvector *vec);
// Returns item at given index, exit failure if out of range
int dvector_at(dvector *vec, int index);
// Adds item to end of vector
void dvector_push(dvector *vec, int item);
// Inserts item at selected index
void dvector_insert(dvector *vec, int index, int item);
// Inserts item at beginning of vector
void dvector_prepend(dvector *vec, int item);
// Removes item from end of vector, returns item
int dvector_pop(dvector *vec);
// Delete item at index, shifts all other items left
int dvector_delete(dvector *vec, int index);
// Removes every item matching the specified value
int dvector_remove(dvector *vec, int value);
// Looks for value and returns first index with that value, -1 if not found
int dvector_find(dvector *vec, int value);



#endif // DVECTOR_H