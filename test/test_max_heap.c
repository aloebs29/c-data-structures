#include <stdbool.h> // bool
#include "unity.h"
#include "max_heap.h"

#define CAPACITY        32

// Force linkage
TEST_FILE("helpers.c");

max_heap_t * heap;

void setUp(void)
{
  heap = max_heap_new(CAPACITY);
}

void tearDown(void)
{
  max_heap_destroy(heap);
}

void test_try_insert()
{
  // Adding first val
  TEST_ASSERT_TRUE(max_heap_try_insert(heap, 1));
  TEST_ASSERT_EQUAL(1, heap->_data[0]);

  // Adding new maxes (will require sifting)
  TEST_ASSERT_TRUE(max_heap_try_insert(heap, 2));
  TEST_ASSERT_TRUE(max_heap_try_insert(heap, 3));
  TEST_ASSERT_TRUE(max_heap_try_insert(heap, 4));
  TEST_ASSERT_TRUE(max_heap_try_insert(heap, 5));

  TEST_ASSERT_EQUAL(5, heap->_data[0]);
  TEST_ASSERT_EQUAL(4, heap->_data[1]);
  TEST_ASSERT_EQUAL(3, heap->_data[4]);
  TEST_ASSERT_EQUAL(2, heap->_data[2]);
  TEST_ASSERT_EQUAL(1, heap->_data[3]);

  // Adding above capacity will return false
  for (int i = 5; i < CAPACITY; i++)
  {
    TEST_ASSERT_TRUE(max_heap_try_insert(heap, i));
  }
  TEST_ASSERT_FALSE(max_heap_try_insert(heap, 42));
}

void test_get_max()
{
  max_heap_try_insert(heap, 1);
  TEST_ASSERT_EQUAL(1, max_heap_get_max(heap));
  max_heap_try_insert(heap, 0);
  TEST_ASSERT_EQUAL(1, max_heap_get_max(heap));
  max_heap_try_insert(heap, 2);
  TEST_ASSERT_EQUAL(2, max_heap_get_max(heap));
}

void test_extract_max()
{
  max_heap_try_insert(heap, 5);
  max_heap_try_insert(heap, 3);
  max_heap_try_insert(heap, 4);
  max_heap_try_insert(heap, 6);
  max_heap_try_insert(heap, 1);
  max_heap_try_insert(heap, 2);

  TEST_ASSERT_EQUAL(6, max_heap_extract_max(heap));
  TEST_ASSERT_EQUAL(5, max_heap_extract_max(heap));
  TEST_ASSERT_EQUAL(4, max_heap_extract_max(heap));
  TEST_ASSERT_EQUAL(3, max_heap_extract_max(heap));
  TEST_ASSERT_EQUAL(2, max_heap_extract_max(heap));
  TEST_ASSERT_EQUAL(1, max_heap_extract_max(heap));
}

void test_is_empty()
{
  TEST_ASSERT_TRUE(max_heap_is_empty(heap));
  max_heap_try_insert(heap, 1);
  TEST_ASSERT_FALSE(max_heap_is_empty(heap));
  max_heap_extract_max(heap);
  TEST_ASSERT_TRUE(max_heap_is_empty(heap));
}

void test_sort()
{
  // Doesn't break @ size 1
  int test1[] = { 3 };
  max_heap_sort(test1, 1);
  TEST_ASSERT_EQUAL(3, test1[0]);

  // General case
  int test2[] = { 5, 3, 6, 1, 2, 99, 42, -3 };
  max_heap_sort(test2, sizeof(test2) / sizeof(*test2));
  TEST_ASSERT_EQUAL(-3, test2[0]);
  TEST_ASSERT_EQUAL(1, test2[1]);
  TEST_ASSERT_EQUAL(2, test2[2]);
  TEST_ASSERT_EQUAL(3, test2[3]);
  TEST_ASSERT_EQUAL(5, test2[4]);
  TEST_ASSERT_EQUAL(6, test2[5]);
  TEST_ASSERT_EQUAL(42, test2[6]);
  TEST_ASSERT_EQUAL(99, test2[7]);
}
