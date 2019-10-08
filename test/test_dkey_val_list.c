#include "unity.h"
#include "dkey_val_list.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_push_back()
{
    dkey_val_list_t * list = dkey_val_list_new();

    dkey_val_list_push_back(list, "one", 1);
    dkey_val_list_push_back(list, "two", 2);
    dkey_val_list_push_back(list, "three", 3);

    TEST_ASSERT_EQUAL_STRING("one", list->_head->_data->_key);
    TEST_ASSERT_EQUAL(1, list->_head->_data->_value);

    TEST_ASSERT_EQUAL_STRING("two", list->_head->_next->_data->_key);
    TEST_ASSERT_EQUAL(2, list->_head->_next->_data->_value);

    TEST_ASSERT_EQUAL_STRING("three", list->_tail->_data->_key);
    TEST_ASSERT_EQUAL(3, list->_tail->_data->_value);

    dkey_val_list_destroy(list);
}

void test_is_empty()
{
    dkey_val_list_t * list = dkey_val_list_new();

    TEST_ASSERT_TRUE(dkey_val_list_is_empty(list));

    dkey_val_list_push_back(list, "one", 1);
    TEST_ASSERT_FALSE(dkey_val_list_is_empty(list));

    dkey_val_list_destroy(list);
}

void test_try_get()
{
    dkey_val_list_t * list = dkey_val_list_new();

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

    dkey_val_list_destroy(list);
}

void test_try_update()
{
    dkey_val_list_t * list = dkey_val_list_new();

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

    dkey_val_list_destroy(list);
}

void test_remove()
{
    dkey_val_list_t * list = dkey_val_list_new();

    dkey_val_list_push_back(list, "one", 1);
    // Removing only item works
    int out = 42;
    dkey_val_list_remove(list, "one");
    TEST_ASSERT_FALSE(dkey_val_list_try_get(list, "one", &out));

    dkey_val_list_push_back(list, "one", 1);
    dkey_val_list_push_back(list, "two", 2);
    dkey_val_list_push_back(list, "three", 3);
    dkey_val_list_push_back(list, "four", 4);
    dkey_val_list_push_back(list, "five", 5);

    // Removing head doesn't throw off list
    dkey_val_list_remove(list, "one");
    TEST_ASSERT_FALSE(dkey_val_list_try_get(list, "one", &out));
    TEST_ASSERT_TRUE(dkey_val_list_try_get(list, "two", &out));
    TEST_ASSERT_EQUAL(2, out);

    // Removing middle
    dkey_val_list_remove(list, "three");
    TEST_ASSERT_FALSE(dkey_val_list_try_get(list, "three", &out));

    // Removing tail doesn't throw off list
    dkey_val_list_remove(list, "five");
    TEST_ASSERT_FALSE(dkey_val_list_try_get(list, "five", &out));
    TEST_ASSERT_TRUE(dkey_val_list_try_get(list, "four", &out));
    TEST_ASSERT_EQUAL(4, out);

    dkey_val_list_destroy(list);
}

// // Creates an empty list
// dkey_val_list_t * dkey_val_list_new();
// // Destroys list
// void dkey_val_list_destroy(dkey_val_list_t * list);
// // Returns true if empty
// bool dkey_val_list_is_empty(dkey_val_list_t * list);
// // Adds item to end of list
// void dkey_val_list_push_back(dkey_val_list_t * list, const char * key,
//     int value);
// // Attempts to get value associated with key
// bool dkey_val_list_try_get(dkey_val_list_t * list, const char * key, int * out);
// // Removes item from list if it exists
// void dkey_val_list_remove(dkey_val_list_t * list, const char * key);