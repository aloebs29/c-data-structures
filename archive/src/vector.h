/**
 * @file		{stem}.h
 * @author		{author}
 * @brief		{brief}
 *
 */

#ifndef VECTOR_H_
#define VECTOR_H_


typedef struct {
  int _size;
  int _capacity;
  int *_data;
} vector_t;

// Creates a vector with at least the given capacity
vector_t *vector_new(int capacity);
// Destroys vector
void vector_destroy(vector_t *vec);
// Returns capacity field (we want it treated as private by module users)
int vector_capacity(vector_t *vec);
// Returns size field (we want it treated as private by module users)
int vector_size(vector_t *vec);
// Returns true if vector has 0 elements
bool vector_is_empty(vector_t *vec);
// Returns item at given index, exit failure if out of range
int vector_at(vector_t *vec, int index);
// Adds item to end of vector
void vector_push(vector_t *vec, int item);
// Inserts item at selected index
void vector_insert(vector_t *vec, int index, int item);
// Inserts item at beginning of vector
void vector_prepend(vector_t *vec, int item);
// Removes item from end of vector, returns item
int vector_pop(vector_t *vec);
// Delete item at index, shifts all other items left
int vector_delete(vector_t *vec, int index);
// Removes every item matching the specified value
int vector_remove(vector_t *vec, int value);
// Looks for value and returns first index with that value, -1 if not found
int vector_find(vector_t *vec, int value);



#endif // VECTOR_H_
