#include "unity.h"
#include "key_val_list.h"

// Force linkage
TEST_FILE("helpers.c");

key_val_list_t *list = NULL;

void setUp(void)
{
  list = key_val_list_new();
}

void tearDown(void)
{
  key_val_list_destroy(list);
}

void test_push_back()
{
  key_val_list_push_back(list, "one", 1);
  key_val_list_push_back(list, "two", 2);
  key_val_list_push_back(list, "three", 3);

  TEST_ASSERT_EQUAL_STRING("one", list->_head->_data->key);
  TEST_ASSERT_EQUAL(1, list->_head->_data->value);

  TEST_ASSERT_EQUAL_STRING("two", list->_head->_next->_data->key);
  TEST_ASSERT_EQUAL(2, list->_head->_next->_data->value);

  TEST_ASSERT_EQUAL_STRING("three", list->_tail->_data->key);
  TEST_ASSERT_EQUAL(3, list->_tail->_data->value);
}

void test_pop_front()
{
  key_val_list_push_back(list, "one", 1);
  key_val_pair_t * pair = key_val_list_pop_front(list);
  TEST_ASSERT_EQUAL(1, pair->value);
  key_val_pair_destroy(pair);
  TEST_ASSERT_NULL(list->_head);
  TEST_ASSERT_NULL(list->_tail);

  key_val_list_push_back(list, "two", 2);
  key_val_list_push_back(list, "three", 3);
  pair = key_val_list_pop_front(list);
  TEST_ASSERT_EQUAL(2, pair->value);
  key_val_pair_destroy(pair);
  pair = key_val_list_pop_front(list);
  TEST_ASSERT_EQUAL(3, pair->value);
  key_val_pair_destroy(pair);
}

void test_try_get()
{
  int out = 42;

  TEST_ASSERT_FALSE(key_val_list_try_get(list, "one", &out));
  TEST_ASSERT_EQUAL(42, out);

  key_val_list_push_back(list, "one", 1);
  TEST_ASSERT_TRUE(key_val_list_try_get(list, "one", &out));
  TEST_ASSERT_EQUAL(1, out);

  key_val_list_push_back(list, "two", 2);
  key_val_list_push_back(list, "three", 3);
  TEST_ASSERT_TRUE(key_val_list_try_get(list, "two", &out));
  TEST_ASSERT_EQUAL(2, out);
  TEST_ASSERT_TRUE(key_val_list_try_get(list, "three", &out));
  TEST_ASSERT_EQUAL(3, out);
}

void test_try_update()
{
  // Works on empty list
  TEST_ASSERT_FALSE(key_val_list_try_update(list, "one", 1));
  // Success case
  key_val_list_push_back(list, "one", 1);
  TEST_ASSERT_TRUE(key_val_list_try_update(list, "one", 29));
  int out = 42;
  TEST_ASSERT_TRUE(key_val_list_try_get(list, "one", &out));
  TEST_ASSERT_EQUAL(29, out);
  // Failure case
  TEST_ASSERT_FALSE(key_val_list_try_update(list, "two", 2));
  key_val_list_try_get(list, "one", &out);
  TEST_ASSERT_EQUAL(29, out);
}

void test_remove()
{
  // Removing from empty list works
  TEST_ASSERT_FALSE(key_val_list_remove(list, "abc"));

  // Removing only item works
  key_val_list_push_back(list, "one", 1);
  TEST_ASSERT_TRUE(key_val_list_remove(list, "one"));
  int out = 42;
  TEST_ASSERT_FALSE(key_val_list_try_get(list, "one", &out));

  key_val_list_push_back(list, "one", 1);
  key_val_list_push_back(list, "two", 2);
  key_val_list_push_back(list, "three", 3);
  key_val_list_push_back(list, "four", 4);
  key_val_list_push_back(list, "five", 5);

  // Removing head doesn't throw off list
  TEST_ASSERT_TRUE(key_val_list_remove(list, "one"));
  TEST_ASSERT_FALSE(key_val_list_try_get(list, "one", &out));
  TEST_ASSERT_TRUE(key_val_list_try_get(list, "two", &out));
  TEST_ASSERT_EQUAL(2, out);

  // Removing middle
  TEST_ASSERT_TRUE(key_val_list_remove(list, "three"));
  TEST_ASSERT_FALSE(key_val_list_try_get(list, "three", &out));

  // Removing tail doesn't throw off list
  TEST_ASSERT_TRUE(key_val_list_remove(list, "five"));
  TEST_ASSERT_FALSE(key_val_list_try_get(list, "five", &out));
  TEST_ASSERT_TRUE(key_val_list_try_get(list, "four", &out));
  TEST_ASSERT_EQUAL(4, out);

  // Removing non-existent value
  TEST_ASSERT_FALSE(key_val_list_remove(list, "abc"));
}

void test_is_empty()
{
  TEST_ASSERT_TRUE(key_val_list_is_empty(list));

  key_val_list_push_back(list, "one", 1);
  TEST_ASSERT_FALSE(key_val_list_is_empty(list));

  key_val_list_pop_front(list);
  TEST_ASSERT_TRUE(key_val_list_is_empty(list));
}
