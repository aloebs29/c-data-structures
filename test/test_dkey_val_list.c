#include "unity.h"
#include "dkey_val_list.h"

// Force linkage
TEST_FILE("helpers.c");

dkey_val_list_t *list = NULL;

void setUp(void)
{
  list = dkey_val_list_new();
}

void tearDown(void)
{
  dkey_val_list_destroy(list);
}

void test_push_back()
{
  dkey_val_list_push_back(list, "one", 1);
  dkey_val_list_push_back(list, "two", 2);
  dkey_val_list_push_back(list, "three", 3);

  TEST_ASSERT_EQUAL_STRING("one", list->_head->_data->_key);
  TEST_ASSERT_EQUAL(1, list->_head->_data->_value);

  TEST_ASSERT_EQUAL_STRING("two", list->_head->_next->_data->_key);
  TEST_ASSERT_EQUAL(2, list->_head->_next->_data->_value);

  TEST_ASSERT_EQUAL_STRING("three", list->_tail->_data->_key);
  TEST_ASSERT_EQUAL(3, list->_tail->_data->_value);
}

void test_pop_front()
{
  dkey_val_list_push_back(list, "one", 1);
  TEST_ASSERT_EQUAL(1, dkey_val_list_pop_front(list));
  TEST_ASSERT_NULL(list->_head);
  TEST_ASSERT_NULL(list->_tail);

  dkey_val_list_push_back(list, "two", 2);
  dkey_val_list_push_back(list, "three", 3);
  TEST_ASSERT_EQUAL(2, dkey_val_list_pop_front(list));
  TEST_ASSERT_EQUAL(3, dkey_val_list_pop_front(list));
}

void test_try_get()
{
  int out = 42;

  TEST_ASSERT_FALSE(dkey_val_list_try_get(list, "one", &out));
  TEST_ASSERT_EQUAL(42, out);

  dkey_val_list_push_back(list, "one", 1);
  TEST_ASSERT_TRUE(dkey_val_list_try_get(list, "one", &out));
  TEST_ASSERT_EQUAL(1, out);

  dkey_val_list_push_back(list, "two", 2);
  dkey_val_list_push_back(list, "three", 3);
  TEST_ASSERT_TRUE(dkey_val_list_try_get(list, "two", &out));
  TEST_ASSERT_EQUAL(2, out);
  TEST_ASSERT_TRUE(dkey_val_list_try_get(list, "three", &out));
  TEST_ASSERT_EQUAL(3, out);
}

void test_try_update()
{
  // Works on empty list
  TEST_ASSERT_FALSE(dkey_val_list_try_update(list, "one", 1));
  // Success case
  dkey_val_list_push_back(list, "one", 1);
  TEST_ASSERT_TRUE(dkey_val_list_try_update(list, "one", 29));
  int out = 42;
  TEST_ASSERT_TRUE(dkey_val_list_try_get(list, "one", &out));
  TEST_ASSERT_EQUAL(29, out);
  // Failure case
  TEST_ASSERT_FALSE(dkey_val_list_try_update(list, "two", 2));
  dkey_val_list_try_get(list, "one", &out);
  TEST_ASSERT_EQUAL(29, out);
}

void test_remove()
{
  // Removing from empty list works
  TEST_ASSERT_FALSE(dkey_val_list_remove(list, "abc"));

  // Removing only item works
  dkey_val_list_push_back(list, "one", 1);
  TEST_ASSERT_TRUE(dkey_val_list_remove(list, "one"));
  int out = 42;
  TEST_ASSERT_FALSE(dkey_val_list_try_get(list, "one", &out));

  dkey_val_list_push_back(list, "one", 1);
  dkey_val_list_push_back(list, "two", 2);
  dkey_val_list_push_back(list, "three", 3);
  dkey_val_list_push_back(list, "four", 4);
  dkey_val_list_push_back(list, "five", 5);

  // Removing head doesn't throw off list
  TEST_ASSERT_TRUE(dkey_val_list_remove(list, "one"));
  TEST_ASSERT_FALSE(dkey_val_list_try_get(list, "one", &out));
  TEST_ASSERT_TRUE(dkey_val_list_try_get(list, "two", &out));
  TEST_ASSERT_EQUAL(2, out);

  // Removing middle
  TEST_ASSERT_TRUE(dkey_val_list_remove(list, "three"));
  TEST_ASSERT_FALSE(dkey_val_list_try_get(list, "three", &out));

  // Removing tail doesn't throw off list
  TEST_ASSERT_TRUE(dkey_val_list_remove(list, "five"));
  TEST_ASSERT_FALSE(dkey_val_list_try_get(list, "five", &out));
  TEST_ASSERT_TRUE(dkey_val_list_try_get(list, "four", &out));
  TEST_ASSERT_EQUAL(4, out);

  // Removing non-existent value
  TEST_ASSERT_FALSE(dkey_val_list_remove(list, "abc"));
}

void test_is_empty()
{
  TEST_ASSERT_TRUE(dkey_val_list_is_empty(list));

  dkey_val_list_push_back(list, "one", 1);
  TEST_ASSERT_FALSE(dkey_val_list_is_empty(list));

  dkey_val_list_pop_front(list);
  TEST_ASSERT_TRUE(dkey_val_list_is_empty(list));
}
