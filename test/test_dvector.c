#include <stdbool.h> // bool
#include "unity.h"
#include "dvector.h"

// Helper function
void push_vec_range(dvector_t *vec, int start_val, int end_val)
{
  for (int i = start_val; i < end_val; i++)
  {
    dvector_push(vec, i);
  }
}
// Tests for functions used by other tests..
void test_new()
{
  dvector_t * vec = dvector_new(0);
  TEST_ASSERT_EQUAL(DEFAULT_CAPACITY, dvector_capacity(vec));
  dvector_destroy(vec);
}

void test_at_and_push() // these two are inseperably reliant on eachother
{
  dvector_t * vec = dvector_new(0);
  push_vec_range(vec, 0, 12);
  TEST_ASSERT_EQUAL(6, dvector_at(vec, 6));
  dvector_destroy(vec);
}

void test_pop()
{
  dvector_t * vec = dvector_new(0);
  push_vec_range(vec, 0, 12);
  // Verify correct return
  TEST_ASSERT_EQUAL(11, dvector_pop(vec));
  // Verify size modified
  TEST_ASSERT_EQUAL(11, dvector_size(vec));
  dvector_destroy(vec);
}

// // All other tests in same order as header
void test_size()
{
  dvector_t * vec = dvector_new(0);
  TEST_ASSERT_EQUAL(0, dvector_size(vec));
  push_vec_range(vec, 0, 12);
  TEST_ASSERT_EQUAL(12, dvector_size(vec));
  dvector_destroy(vec);
}

void test_capacity()
{
  // Default capacity
  dvector_t * vec = dvector_new(0);
  TEST_ASSERT_EQUAL(DEFAULT_CAPACITY, dvector_capacity(vec));
  dvector_destroy(vec);

  // Second capacity
  vec = dvector_new(DEFAULT_CAPACITY + 1);
  TEST_ASSERT_EQUAL(DEFAULT_CAPACITY * GROWTH_FACTOR, dvector_capacity(vec));
  dvector_destroy(vec);

  // Third capacity
  vec = dvector_new((DEFAULT_CAPACITY * GROWTH_FACTOR) + 1);
  TEST_ASSERT_EQUAL((DEFAULT_CAPACITY * GROWTH_FACTOR * GROWTH_FACTOR),
      dvector_capacity(vec));
  dvector_destroy(vec);
}

void test_is_empty()
{
  dvector_t * vec = dvector_new(0);
  TEST_ASSERT_EQUAL(true, dvector_is_empty(vec));
  dvector_destroy(vec);
}

void test_insert()
{
  dvector_t * vec = dvector_new(0);
  push_vec_range(vec, 0, 12);
  dvector_insert(vec, 3, 100);
  // Verify item is where we wanted it
  TEST_ASSERT_EQUAL(100, dvector_at(vec, 3));
  // Verify items were right shifted
  TEST_ASSERT_EQUAL(11, dvector_at(vec, 12));
  dvector_destroy(vec);
}

void test_prepend()
{
  dvector_t * vec = dvector_new(0);
  push_vec_range(vec, 0, 12);
  dvector_prepend(vec, 100);
  // Verify item is where we wanted it
  TEST_ASSERT_EQUAL(100, dvector_at(vec, 0));
  // Verify items were right shifted
  TEST_ASSERT_EQUAL(10, dvector_at(vec, 11));
  dvector_destroy(vec);
}

void test_delete()
{
  dvector_t * vec = dvector_new(0);
  push_vec_range(vec, 0, 12);
  // Verify return value
  TEST_ASSERT_EQUAL(5, dvector_delete(vec, 5));
  // Verify left shift at adjacent index
  TEST_ASSERT_EQUAL(6, dvector_at(vec, 5));
  // Verify size update
  TEST_ASSERT_EQUAL(11, dvector_size(vec));
  // Verify left shift of end index
  TEST_ASSERT_EQUAL(11, dvector_at(vec, 10));
  dvector_destroy(vec);
}

void test_remove()
{
  dvector_t * vec = dvector_new(0);
  push_vec_range(vec, 0, 12);
  dvector_insert(vec, 10, 7);
  dvector_prepend(vec, 7);

  dvector_remove(vec, 7);
  // Verify size update
  TEST_ASSERT_EQUAL(11, dvector_size(vec));
  // Verify first removal
  TEST_ASSERT_EQUAL(0, dvector_at(vec, 0));
  // Verify second removal
  TEST_ASSERT_EQUAL(11, dvector_at(vec, 10));
  // Verify third removal
  TEST_ASSERT_EQUAL(8, dvector_at(vec, 7));
  dvector_destroy(vec);
}

void test_resize()
{
  dvector_t * vec = dvector_new(0);
  push_vec_range(vec, 0, DEFAULT_CAPACITY + 1);
  // Verify capacity grew
  TEST_ASSERT_EQUAL(DEFAULT_CAPACITY * GROWTH_FACTOR, dvector_capacity(vec));
  // Verify data filled into extra capacity
  TEST_ASSERT_EQUAL(DEFAULT_CAPACITY, dvector_at(vec, DEFAULT_CAPACITY));
  
  // Remove items from front until vector should shrink
  for (int i = DEFAULT_CAPACITY; 
      i >= ((DEFAULT_CAPACITY * GROWTH_FACTOR) / SHRINK_AT_DIVISOR);
      i--)
  {
    dvector_delete(vec, 0); 
  }
  // Verify capacity shrunk
  TEST_ASSERT_EQUAL(DEFAULT_CAPACITY, dvector_capacity(vec));

  dvector_destroy(vec);
}