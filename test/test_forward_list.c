#include "unity.h"
#include "forward_list.h"

// Force linkage
TEST_FILE("helpers.c");

forward_list_t * list = NULL;

void setUp(void)
{
  list = forward_list_new();
}

void tearDown(void)
{
  forward_list_destroy(list);
}

void test_push_front()
{
  forward_list_push_front(list, 1);
  TEST_ASSERT_EQUAL(1, list->_head->_key);
  forward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, list->_head->_key);
}

void test_is_empty()
{
  TEST_ASSERT_TRUE(forward_list_is_empty(list));
  forward_list_push_front(list, 1);
  TEST_ASSERT_FALSE(forward_list_is_empty(list));
}

void test_size()
{
  TEST_ASSERT_EQUAL(0, forward_list_size(list));
  forward_list_push_front(list, 1);
  TEST_ASSERT_EQUAL(1, forward_list_size(list));
  forward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, forward_list_size(list));
}

void test_at()
{
  forward_list_push_front(list, 1);
  forward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, forward_list_at(list, 0));
  TEST_ASSERT_EQUAL(1, forward_list_at(list, 1));
}

void test_push_back()
{
  forward_list_push_back(list, 1);
  forward_list_push_back(list, 2);
  TEST_ASSERT_EQUAL(1, forward_list_at(list, 0));
  TEST_ASSERT_EQUAL(2, forward_list_at(list, 1));
}

void test_at_from_end()
{
  forward_list_push_front(list, 1);
  forward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(1, forward_list_at_from_end(list, 0));
  TEST_ASSERT_EQUAL(2, forward_list_at_from_end(list, 1));
}

void test_pop_front()
{
  forward_list_push_front(list, 1);
  forward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, forward_list_pop_front(list));
  TEST_ASSERT_EQUAL(1, forward_list_at(list, 0));
}

void test_pop_back()
{
  forward_list_push_front(list, 1);
  forward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(1, forward_list_pop_back(list));
  TEST_ASSERT_EQUAL(2, forward_list_at_from_end(list, 0));
}

void test_front()
{
  forward_list_push_front(list, 1);
  TEST_ASSERT_EQUAL(1, forward_list_front(list));
  forward_list_push_front(list, 2);
  TEST_ASSERT_EQUAL(2, forward_list_front(list));
}

void test_back()
{
  forward_list_push_back(list, 1);
  TEST_ASSERT_EQUAL(1, forward_list_back(list));
  forward_list_push_back(list, 2);
  TEST_ASSERT_EQUAL(2, forward_list_back(list));
}

void test_insert()
{
  forward_list_insert(list, 0, 1);
  forward_list_insert(list, 1, 3);
  forward_list_insert(list, 2, 4);
  forward_list_insert(list, 1, 2);

  TEST_ASSERT_EQUAL(1, forward_list_at(list, 0));
  TEST_ASSERT_EQUAL(2, forward_list_at(list, 1));
  TEST_ASSERT_EQUAL(3, forward_list_at(list, 2));
  TEST_ASSERT_EQUAL(4, forward_list_at(list, 3));
}

void test_delete()
{
  forward_list_push_back(list, 1);
  forward_list_push_back(list, 2);
  forward_list_push_back(list, 3);
  forward_list_push_back(list, 4);

  // Middle
  TEST_ASSERT_EQUAL(3, forward_list_delete(list, 2));
  TEST_ASSERT_EQUAL(4, forward_list_at(list, 2));
  // Tail
  TEST_ASSERT_EQUAL(4, forward_list_delete(list, 2));
  TEST_ASSERT_EQUAL(2, forward_list_back(list));
  // Head
  TEST_ASSERT_EQUAL(1, forward_list_delete(list, 0));
  TEST_ASSERT_EQUAL(2, forward_list_front(list));
}

void test_reverse()
{
  // Empty
  forward_list_reverse(list);
  TEST_ASSERT_TRUE(forward_list_is_empty(list));
  // One element
  forward_list_push_back(list, 1);
  forward_list_reverse(list);
  TEST_ASSERT_EQUAL(1, forward_list_size(list));
  TEST_ASSERT_EQUAL(1, forward_list_front(list));
  // Multi-element
  forward_list_push_back(list, 2);
  forward_list_push_back(list, 3);
  forward_list_push_back(list, 4);
  forward_list_reverse(list);
  TEST_ASSERT_EQUAL(4, forward_list_front(list));
  TEST_ASSERT_EQUAL(3, forward_list_at(list, 1));
  TEST_ASSERT_EQUAL(2, forward_list_at(list, 2));
  TEST_ASSERT_EQUAL(1, forward_list_back(list));
}

void test_remove()
{
  // Empty
  forward_list_remove(list, 12);
  TEST_ASSERT_TRUE(forward_list_is_empty(list));

  forward_list_push_back(list, 1);
  forward_list_push_back(list, 2);
  forward_list_push_back(list, 1);
  forward_list_push_back(list, 3);
  forward_list_push_back(list, 2);
  // Deleting head
  forward_list_remove(list, 1);
  TEST_ASSERT_EQUAL(3, forward_list_size(list));
  TEST_ASSERT_EQUAL(2, forward_list_front(list));
  TEST_ASSERT_EQUAL(3, forward_list_at(list, 1));
  TEST_ASSERT_EQUAL(2, forward_list_back(list));
  // Deleting tail
  forward_list_remove(list, 2);
  TEST_ASSERT_EQUAL(1, forward_list_size(list));
  TEST_ASSERT_EQUAL(3, forward_list_front(list));
  TEST_ASSERT_EQUAL(3, forward_list_back(list));
  // Deleting entire list
  forward_list_remove(list, 3);
  TEST_ASSERT_TRUE(forward_list_is_empty(list));
}
