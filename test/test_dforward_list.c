#include "unity.h"
#include "dforward_list.h"

// Force linkage
TEST_FILE("helpers.c");

dforward_list_t * list = NULL;

void setUp(void)
{
  list = dforward_list_new();
}

void tearDown(void)
{
  dforward_list_destroy(list);
}

void test_push_front()
{
  dforward_list_push_front(list, 1);
  TEST_ASSERT_EQUAL(1, list->_head->_key);
  dforward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, list->_head->_key);
}

void test_is_empty()
{
  TEST_ASSERT_TRUE(dforward_list_is_empty(list));
  dforward_list_push_front(list, 1);
  TEST_ASSERT_FALSE(dforward_list_is_empty(list));
}

void test_size()
{
  TEST_ASSERT_EQUAL(0, dforward_list_size(list));
  dforward_list_push_front(list, 1);
  TEST_ASSERT_EQUAL(1, dforward_list_size(list));
  dforward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, dforward_list_size(list));
}

void test_at()
{
  dforward_list_push_front(list, 1);
  dforward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, dforward_list_at(list, 0));
  TEST_ASSERT_EQUAL(1, dforward_list_at(list, 1));
}

void test_push_back()
{
  dforward_list_push_back(list, 1);
  dforward_list_push_back(list, 2);
  TEST_ASSERT_EQUAL(1, dforward_list_at(list, 0));
  TEST_ASSERT_EQUAL(2, dforward_list_at(list, 1));
}

void test_at_from_end()
{
  dforward_list_push_front(list, 1);
  dforward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(1, dforward_list_at_from_end(list, 0));
  TEST_ASSERT_EQUAL(2, dforward_list_at_from_end(list, 1));
}

void test_pop_front()
{
  dforward_list_push_front(list, 1);
  dforward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, dforward_list_pop_front(list));
  TEST_ASSERT_EQUAL(1, dforward_list_at(list, 0));
}

void test_pop_back()
{
  dforward_list_push_front(list, 1);
  dforward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(1, dforward_list_pop_back(list));
  TEST_ASSERT_EQUAL(2, dforward_list_at_from_end(list, 0));
}

void test_front()
{
  dforward_list_push_front(list, 1);
  TEST_ASSERT_EQUAL(1, dforward_list_front(list));
  dforward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, dforward_list_front(list));
}

void test_back()
{
  dforward_list_push_back(list, 1);
  TEST_ASSERT_EQUAL(1, dforward_list_back(list));
  dforward_list_push_back(list, 2);
  TEST_ASSERT_EQUAL(2, dforward_list_back(list));
}

void test_insert()
{
  dforward_list_insert(list, 0, 1);
  dforward_list_insert(list, 1, 3);
  dforward_list_insert(list, 2, 4);
  dforward_list_insert(list, 1, 2);

  TEST_ASSERT_EQUAL(1, dforward_list_at(list, 0));
  TEST_ASSERT_EQUAL(2, dforward_list_at(list, 1));
  TEST_ASSERT_EQUAL(3, dforward_list_at(list, 2));
  TEST_ASSERT_EQUAL(4, dforward_list_at(list, 3));
}

void test_delete()
{
  dforward_list_push_back(list, 1);
  dforward_list_push_back(list, 2);
  dforward_list_push_back(list, 3);
  dforward_list_push_back(list, 4);

  // Middle
  TEST_ASSERT_EQUAL(3, dforward_list_delete(list, 2));
  TEST_ASSERT_EQUAL(4, dforward_list_at(list, 2));
  // Tail
  TEST_ASSERT_EQUAL(4, dforward_list_delete(list, 2));
  TEST_ASSERT_EQUAL(2, dforward_list_back(list));
  // Head
  TEST_ASSERT_EQUAL(1, dforward_list_delete(list, 0));
  TEST_ASSERT_EQUAL(2, dforward_list_front(list));
}

void test_reverse()
{
  // Empty
  dforward_list_reverse(list);
  TEST_ASSERT_TRUE(dforward_list_is_empty(list));
  // One element
  dforward_list_push_back(list, 1);
  dforward_list_reverse(list);
  TEST_ASSERT_EQUAL(1, dforward_list_size(list));
  TEST_ASSERT_EQUAL(1, dforward_list_front(list));
  // Multi-element
  dforward_list_push_back(list, 2);
  dforward_list_push_back(list, 3);
  dforward_list_push_back(list, 4);
  dforward_list_reverse(list);
  TEST_ASSERT_EQUAL(4, dforward_list_front(list));
  TEST_ASSERT_EQUAL(3, dforward_list_at(list, 1));
  TEST_ASSERT_EQUAL(2, dforward_list_at(list, 2));
  TEST_ASSERT_EQUAL(1, dforward_list_back(list));
}

void test_remove()
{
  // Empty
  dforward_list_remove(list, 12);
  TEST_ASSERT_TRUE(dforward_list_is_empty(list));

  dforward_list_push_back(list, 1);
  dforward_list_push_back(list, 2);
  dforward_list_push_back(list, 1);
  dforward_list_push_back(list, 3);
  dforward_list_push_back(list, 2);
  // Deleting head
  dforward_list_remove(list, 1);
  TEST_ASSERT_EQUAL(3, dforward_list_size(list));
  TEST_ASSERT_EQUAL(2, dforward_list_front(list));
  TEST_ASSERT_EQUAL(3, dforward_list_at(list, 1));
  TEST_ASSERT_EQUAL(2, dforward_list_back(list));
  // Deleting tail
  dforward_list_remove(list, 2);
  TEST_ASSERT_EQUAL(1, dforward_list_size(list));
  TEST_ASSERT_EQUAL(3, dforward_list_front(list));
  TEST_ASSERT_EQUAL(3, dforward_list_back(list));
  // Deleting entire list
  dforward_list_remove(list, 3);
  TEST_ASSERT_TRUE(dforward_list_is_empty(list));
}
