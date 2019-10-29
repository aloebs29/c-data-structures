#include "unity.h"
#include "qeue.h"

// Force linkage
TEST_FILE("forward_list.c");
TEST_FILE("helpers.c");

qeue_t * qeue = NULL;

void setUp(void)
{
}

void tearDown(void)
{
    qeue_destroy(qeue);
}
// Qeue as array testing
void test_qeue_array_enqeue()
{
    qeue = qeue_new_as_array(10);

    qeue_enqeue(qeue, 1);
    qeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, qeue->_data._array->_front[0]);
    TEST_ASSERT_EQUAL(2, qeue->_data._array->_front[1]);
}

void test_qeue_array_deqeue()
{
    qeue = qeue_new_as_array(10);

    qeue_enqeue(qeue, 1);
    qeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, qeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(2, qeue_deqeue(qeue));
}

void test_qeue_array_enqeue_deqeue_wrap()
{
    qeue = qeue_new_as_array(2);

    qeue_enqeue(qeue, 1);
    qeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, qeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(2, qeue_deqeue(qeue));
    qeue_enqeue(qeue, 3);
    qeue_enqeue(qeue, 4);
    TEST_ASSERT_EQUAL(3, qeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(4, qeue_deqeue(qeue));
    qeue_enqeue(qeue, 5);
    qeue_enqeue(qeue, 6);
    TEST_ASSERT_EQUAL(5, qeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(6, qeue_deqeue(qeue));
}

void test_qeue_array_is_empty()
{
    qeue = qeue_new_as_array(2);

    // Make sure new is empty
    TEST_ASSERT_TRUE(qeue_is_empty(qeue));
    // Normal case (false)
    qeue_enqeue(qeue, 1);
    TEST_ASSERT_FALSE(qeue_is_empty(qeue));
    // Normal case (true)
    qeue_deqeue(qeue);
    TEST_ASSERT_TRUE(qeue_is_empty(qeue));
    // Edge case (after write wrap)
    qeue_enqeue(qeue, 2);
    qeue_enqeue(qeue, 3);
    qeue_deqeue(qeue);
    qeue_deqeue(qeue);
    TEST_ASSERT_TRUE(qeue_is_empty(qeue));
}

void test_qeue_array_is_full()
{
    qeue = qeue_new_as_array(2);

    // Make sure new is not full
    TEST_ASSERT_FALSE(qeue_is_full(qeue));
    // Normal case (false)
    qeue_enqeue(qeue, 1);
    TEST_ASSERT_FALSE(qeue_is_full(qeue));
    // Normal case (true)
    qeue_enqeue(qeue, 2);
    TEST_ASSERT_TRUE(qeue_is_full(qeue));
    // Edge case (after write wrap)
    qeue_deqeue(qeue);
    qeue_deqeue(qeue);
    qeue_enqeue(qeue, 3);
    qeue_enqeue(qeue, 4);
    TEST_ASSERT_TRUE(qeue_is_full(qeue));
}

// Qeue as singly-linked list testing
void test_qeue_list_enqeue()
{
    qeue = qeue_new_as_list();

    qeue_enqeue(qeue, 1);
    qeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, qeue->_data._list->_head->_key);
    TEST_ASSERT_EQUAL(2, qeue->_data._list->_head->_next->_key);
}

void test_qeue_list_deqeue()
{
    qeue = qeue_new_as_list();

    qeue_enqeue(qeue, 1);
    qeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, qeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(2, qeue_deqeue(qeue));
}

void test_qeue_list_is_empty()
{
    qeue = qeue_new_as_list();

    TEST_ASSERT_TRUE(qeue_is_empty(qeue));
    qeue_enqeue(qeue, 1);
    TEST_ASSERT_FALSE(qeue_is_empty(qeue));
    qeue_deqeue(qeue);
    TEST_ASSERT_TRUE(qeue_is_empty(qeue));
}

void test_qeue_list_is_full()
{
    qeue = qeue_new_as_list();

    TEST_ASSERT_FALSE(qeue_is_full(qeue));
    qeue_enqeue(qeue, 1);
    TEST_ASSERT_FALSE(qeue_is_full(qeue));
}

