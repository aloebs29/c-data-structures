#include "unity.h"
#include "queue.h"

// Force linkage
TEST_FILE("forward_list.c");
TEST_FILE("helpers.c");

queue_t * queue = NULL;

void setUp(void)
{
}

void tearDown(void)
{
    queue_destroy(queue);
}
// queue as array testing
void test_queue_array_enqueue()
{
    queue = queue_new_as_array(10);

    queue_enqueue(queue, 1);
    queue_enqueue(queue, 2);
    TEST_ASSERT_EQUAL(1, queue->_data._array->_front[0]);
    TEST_ASSERT_EQUAL(2, queue->_data._array->_front[1]);
}

void test_queue_array_dequeue()
{
    queue = queue_new_as_array(10);

    queue_enqueue(queue, 1);
    queue_enqueue(queue, 2);
    TEST_ASSERT_EQUAL(1, queue_dequeue(queue));
    TEST_ASSERT_EQUAL(2, queue_dequeue(queue));
}

void test_queue_array_enqueue_dequeue_wrap()
{
    queue = queue_new_as_array(2);

    queue_enqueue(queue, 1);
    queue_enqueue(queue, 2);
    TEST_ASSERT_EQUAL(1, queue_dequeue(queue));
    TEST_ASSERT_EQUAL(2, queue_dequeue(queue));
    queue_enqueue(queue, 3);
    queue_enqueue(queue, 4);
    TEST_ASSERT_EQUAL(3, queue_dequeue(queue));
    TEST_ASSERT_EQUAL(4, queue_dequeue(queue));
    queue_enqueue(queue, 5);
    queue_enqueue(queue, 6);
    TEST_ASSERT_EQUAL(5, queue_dequeue(queue));
    TEST_ASSERT_EQUAL(6, queue_dequeue(queue));
}

void test_queue_array_is_empty()
{
    queue = queue_new_as_array(2);

    // Make sure new is empty
    TEST_ASSERT_TRUE(queue_is_empty(queue));
    // Normal case (false)
    queue_enqueue(queue, 1);
    TEST_ASSERT_FALSE(queue_is_empty(queue));
    // Normal case (true)
    queue_dequeue(queue);
    TEST_ASSERT_TRUE(queue_is_empty(queue));
    // Edge case (after write wrap)
    queue_enqueue(queue, 2);
    queue_enqueue(queue, 3);
    queue_dequeue(queue);
    queue_dequeue(queue);
    TEST_ASSERT_TRUE(queue_is_empty(queue));
}

void test_queue_array_is_full()
{
    queue = queue_new_as_array(2);

    // Make sure new is not full
    TEST_ASSERT_FALSE(queue_is_full(queue));
    // Normal case (false)
    queue_enqueue(queue, 1);
    TEST_ASSERT_FALSE(queue_is_full(queue));
    // Normal case (true)
    queue_enqueue(queue, 2);
    TEST_ASSERT_TRUE(queue_is_full(queue));
    // Edge case (after write wrap)
    queue_dequeue(queue);
    queue_dequeue(queue);
    queue_enqueue(queue, 3);
    queue_enqueue(queue, 4);
    TEST_ASSERT_TRUE(queue_is_full(queue));
}

// queue as singly-linked list testing
void test_queue_list_enqueue()
{
    queue = queue_new_as_list();

    queue_enqueue(queue, 1);
    queue_enqueue(queue, 2);
    TEST_ASSERT_EQUAL(1, queue->_data._list->_head->_key);
    TEST_ASSERT_EQUAL(2, queue->_data._list->_head->_next->_key);
}

void test_queue_list_dequeue()
{
    queue = queue_new_as_list();

    queue_enqueue(queue, 1);
    queue_enqueue(queue, 2);
    TEST_ASSERT_EQUAL(1, queue_dequeue(queue));
    TEST_ASSERT_EQUAL(2, queue_dequeue(queue));
}

void test_queue_list_is_empty()
{
    queue = queue_new_as_list();

    TEST_ASSERT_TRUE(queue_is_empty(queue));
    queue_enqueue(queue, 1);
    TEST_ASSERT_FALSE(queue_is_empty(queue));
    queue_dequeue(queue);
    TEST_ASSERT_TRUE(queue_is_empty(queue));
}

void test_queue_list_is_full()
{
    queue = queue_new_as_list();

    TEST_ASSERT_FALSE(queue_is_full(queue));
    queue_enqueue(queue, 1);
    TEST_ASSERT_FALSE(queue_is_full(queue));
}

