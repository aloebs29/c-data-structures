#include "unity.h"
#include "dqeue.h"

// Force linkage
TEST_FILE("dforward_list.c");
TEST_FILE("helpers.c");

dqeue_t * qeue = NULL;

void setUp(void)
{
}

void tearDown(void)
{
    dqeue_destroy(qeue);
}
// Qeue as array testing
void test_qeue_array_enqeue()
{
    qeue = dqeue_new_as_array(10);

    dqeue_enqeue(qeue, 1);
    dqeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, qeue->_data._array->_front[0]);
    TEST_ASSERT_EQUAL(2, qeue->_data._array->_front[1]);
}

void test_qeue_array_deqeue()
{
    qeue = dqeue_new_as_array(10);

    dqeue_enqeue(qeue, 1);
    dqeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, dqeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(2, dqeue_deqeue(qeue));
}

void test_qeue_array_enqeue_deqeue_wrap()
{
    qeue = dqeue_new_as_array(2);

    dqeue_enqeue(qeue, 1);
    dqeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, dqeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(2, dqeue_deqeue(qeue));
    dqeue_enqeue(qeue, 3);
    dqeue_enqeue(qeue, 4);
    TEST_ASSERT_EQUAL(3, dqeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(4, dqeue_deqeue(qeue));
    dqeue_enqeue(qeue, 5);
    dqeue_enqeue(qeue, 6);
    TEST_ASSERT_EQUAL(5, dqeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(6, dqeue_deqeue(qeue));
}

void test_qeue_array_is_empty()
{
    qeue = dqeue_new_as_array(2);

    // Make sure new is empty
    TEST_ASSERT_TRUE(dqeue_is_empty(qeue));
    // Normal case (false)
    dqeue_enqeue(qeue, 1);
    TEST_ASSERT_FALSE(dqeue_is_empty(qeue));
    // Normal case (true)
    dqeue_deqeue(qeue);
    TEST_ASSERT_TRUE(dqeue_is_empty(qeue));
    // Edge case (after write wrap)
    dqeue_enqeue(qeue, 2);
    dqeue_enqeue(qeue, 3);
    dqeue_deqeue(qeue);
    dqeue_deqeue(qeue);
    TEST_ASSERT_TRUE(dqeue_is_empty(qeue));
}

void test_qeue_array_is_full()
{
    qeue = dqeue_new_as_array(2);

    // Make sure new is not full
    TEST_ASSERT_FALSE(dqeue_is_full(qeue));
    // Normal case (false)
    dqeue_enqeue(qeue, 1);
    TEST_ASSERT_FALSE(dqeue_is_full(qeue));
    // Normal case (true)
    dqeue_enqeue(qeue, 2);
    TEST_ASSERT_TRUE(dqeue_is_full(qeue));
    // Edge case (after write wrap)
    dqeue_deqeue(qeue);
    dqeue_deqeue(qeue);
    dqeue_enqeue(qeue, 3);
    dqeue_enqeue(qeue, 4);
    TEST_ASSERT_TRUE(dqeue_is_full(qeue));
}

// Qeue as singly-linked list testing
void test_qeue_list_enqeue()
{
    qeue = dqeue_new_as_list();

    dqeue_enqeue(qeue, 1);
    dqeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, qeue->_data._list->_head->_key);
    TEST_ASSERT_EQUAL(2, qeue->_data._list->_head->_next->_key);
}

void test_qeue_list_deqeue()
{
    qeue = dqeue_new_as_list();

    dqeue_enqeue(qeue, 1);
    dqeue_enqeue(qeue, 2);
    TEST_ASSERT_EQUAL(1, dqeue_deqeue(qeue));
    TEST_ASSERT_EQUAL(2, dqeue_deqeue(qeue));
}

void test_qeue_list_is_empty()
{
    qeue = dqeue_new_as_list();

    TEST_ASSERT_TRUE(dqeue_is_empty(qeue));
    dqeue_enqeue(qeue, 1);
    TEST_ASSERT_FALSE(dqeue_is_empty(qeue));
    dqeue_deqeue(qeue);
    TEST_ASSERT_TRUE(dqeue_is_empty(qeue));
}

void test_qeue_list_is_full()
{
    qeue = dqeue_new_as_list();

    TEST_ASSERT_FALSE(dqeue_is_full(qeue));
    dqeue_enqeue(qeue, 1);
    TEST_ASSERT_FALSE(dqeue_is_full(qeue));
}

