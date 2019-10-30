#include "unity.h"
#include <stdlib.h> // free
#include "bst.h"

// Force linkage
TEST_FILE("helpers.c");

static bst_t * tree;

void setUp(void)
{
  tree = bst_new();
}

void tearDown(void)
{
  bst_destroy(tree);
}

void test_insert()
{
  bst_insert(tree, 3);
  TEST_ASSERT_EQUAL(3, tree->_root->_key);
  TEST_ASSERT_EQUAL(1, tree->_root->_count);
  bst_insert(tree, 3);
  TEST_ASSERT_EQUAL(2, tree->_root->_count);

  bst_insert(tree, 1);
  bst_insert(tree, 0);
  bst_insert(tree, 2);
  TEST_ASSERT_EQUAL(1, tree->_root->_left->_key);
  TEST_ASSERT_EQUAL(0, tree->_root->_left->_left->_key);
  TEST_ASSERT_EQUAL(2, tree->_root->_left->_right->_key);


  bst_insert(tree, 5);
  bst_insert(tree, 4);
  bst_insert(tree, 6);
  TEST_ASSERT_EQUAL(5, tree->_root->_right->_key);
  TEST_ASSERT_EQUAL(4, tree->_root->_right->_left->_key);
  TEST_ASSERT_EQUAL(6, tree->_root->_right->_right->_key);
}

void test_get_count()
{
  TEST_ASSERT_EQUAL(0, bst_get_count(tree));

  bst_insert(tree, 3);
  TEST_ASSERT_EQUAL(1, bst_get_count(tree));
  bst_insert(tree, 3);
  TEST_ASSERT_EQUAL(2, bst_get_count(tree));

  bst_insert(tree, 1007);
  bst_insert(tree, 23);
  bst_insert(tree, -55);
  bst_insert(tree, 2);
  TEST_ASSERT_EQUAL(6, bst_get_count(tree));
}

void test_get_node_count()
{
  TEST_ASSERT_EQUAL(0, bst_get_node_count(tree));

  bst_insert(tree, 3);
  TEST_ASSERT_EQUAL(1, bst_get_node_count(tree));
  bst_insert(tree, 3);
  TEST_ASSERT_EQUAL(1, bst_get_node_count(tree));

  bst_insert(tree, 1007);
  bst_insert(tree, 23);
  bst_insert(tree, -55);
  bst_insert(tree, 2);
  TEST_ASSERT_EQUAL(5, bst_get_node_count(tree));
}

void test_is_in_tree()
{
  TEST_ASSERT_FALSE(bst_is_in_tree(tree, 5));

  bst_insert(tree, 5);
  TEST_ASSERT_TRUE(bst_is_in_tree(tree, 5));
  TEST_ASSERT_FALSE(bst_is_in_tree(tree, -5));

  bst_insert(tree, -3);
  bst_insert(tree, 2);
  bst_insert(tree, 1);
  bst_insert(tree, -32);
  bst_insert(tree, -3);
  bst_insert(tree, 45);
  bst_insert(tree, 44);
  TEST_ASSERT_TRUE(bst_is_in_tree(tree, -3));
  TEST_ASSERT_TRUE(bst_is_in_tree(tree, 2));
  TEST_ASSERT_TRUE(bst_is_in_tree(tree, 1));
  TEST_ASSERT_TRUE(bst_is_in_tree(tree, -32));
  TEST_ASSERT_TRUE(bst_is_in_tree(tree, -3));
  TEST_ASSERT_TRUE(bst_is_in_tree(tree, 45));
  TEST_ASSERT_TRUE(bst_is_in_tree(tree, 44));
  TEST_ASSERT_FALSE(bst_is_in_tree(tree, -42));
  TEST_ASSERT_FALSE(bst_is_in_tree(tree, -62));
  TEST_ASSERT_FALSE(bst_is_in_tree(tree, 31));
}

void test_get_height()
{
  TEST_ASSERT_EQUAL(0, bst_get_height(tree));

  bst_insert(tree, 42);
  TEST_ASSERT_EQUAL(1, bst_get_height(tree));

  bst_insert(tree, 43);
  TEST_ASSERT_EQUAL(2, bst_get_height(tree));
  bst_insert(tree, 30);
  bst_insert(tree, 39);
  bst_insert(tree, 39);
  bst_insert(tree, 39);
  TEST_ASSERT_EQUAL(3, bst_get_height(tree));
  bst_insert(tree, 45);
  bst_insert(tree, 46);
  bst_insert(tree, 47);
  TEST_ASSERT_EQUAL(5, bst_get_height(tree));
  bst_insert(tree, 38);
  bst_insert(tree, 37);
  bst_insert(tree, 36);
  TEST_ASSERT_EQUAL(6, bst_get_height(tree));
}

void test_get_min()
{
  bst_insert(tree, 42);
  TEST_ASSERT_EQUAL(42, bst_get_min(tree));

  bst_insert(tree, 43);
  TEST_ASSERT_EQUAL(42, bst_get_min(tree));

  bst_insert(tree, 37);
  bst_insert(tree, 38);
  bst_insert(tree, 39);
  TEST_ASSERT_EQUAL(37, bst_get_min(tree));
}

void test_get_max()
{
  bst_insert(tree, 42);
  TEST_ASSERT_EQUAL(42, bst_get_max(tree));

  bst_insert(tree, 41);
  TEST_ASSERT_EQUAL(42, bst_get_max(tree));

  bst_insert(tree, 46);
  bst_insert(tree, 45);
  bst_insert(tree, 44);
  TEST_ASSERT_EQUAL(46, bst_get_max(tree));
}

void test_flatten()
{
  bst_insert(tree, 42);
  int * flattened = bst_flatten(tree);
  TEST_ASSERT_EQUAL(42, flattened[0]);
  free(flattened);

  bst_insert(tree, -372);
  bst_insert(tree, 41);
  bst_insert(tree, -333);
  bst_insert(tree, 45);
  bst_insert(tree, 43);
  bst_insert(tree, 42);

  flattened = bst_flatten(tree);
  TEST_ASSERT_EQUAL(-372, flattened[0]);
  TEST_ASSERT_EQUAL(-333, flattened[1]);
  TEST_ASSERT_EQUAL(41, flattened[2]);
  TEST_ASSERT_EQUAL(42, flattened[3]);
  TEST_ASSERT_EQUAL(42, flattened[4]);
  TEST_ASSERT_EQUAL(43, flattened[5]);
  TEST_ASSERT_EQUAL(45, flattened[6]);
  free(flattened);
}

void test_remove()
{
  bst_insert(tree, 42);
  bst_insert(tree, 42);
  bst_insert(tree, 43);

  // Test with removing 1 count
  bst_remove(tree, 42);
  TEST_ASSERT_EQUAL(2, bst_get_count(tree));
  int * flattened = bst_flatten(tree);
  TEST_ASSERT_EQUAL(43, flattened[1]);
  free(flattened);

  // Test with removing root
  bst_remove(tree, 42);
  TEST_ASSERT_EQUAL(1, bst_get_count(tree));
  flattened = bst_flatten(tree);
  TEST_ASSERT_EQUAL(43, flattened[0]);
  free(flattened);

  // Test with removing root, without children
  bst_remove(tree, 43);
  TEST_ASSERT_EQUAL(0, bst_get_count(tree));

  bst_insert(tree, 42);
  bst_insert(tree, 10);
  bst_insert(tree, 1);
  bst_insert(tree, 5);
  bst_insert(tree, 8);
  bst_insert(tree, 100);
  bst_insert(tree, 99);
  bst_insert(tree, 101);

  // Test removing leaf
  bst_remove(tree, 8);
  TEST_ASSERT_EQUAL(7, bst_get_count(tree));
  flattened = bst_flatten(tree);
  TEST_ASSERT_EQUAL(5, flattened[1]);
  TEST_ASSERT_EQUAL(10, flattened[2]);
  free(flattened);

  // Test removing node with 1 child
  bst_remove(tree, 1);
  TEST_ASSERT_EQUAL(6, bst_get_count(tree));
  flattened = bst_flatten(tree);
  TEST_ASSERT_EQUAL(5, flattened[0]);
  TEST_ASSERT_EQUAL(10, flattened[1]);
  free(flattened);

  // Test removing node with two children
  bst_remove(tree, 100);
  TEST_ASSERT_EQUAL(5, bst_get_count(tree));
  flattened = bst_flatten(tree);
  TEST_ASSERT_EQUAL(99, flattened[3]);
  TEST_ASSERT_EQUAL(101, flattened[4]);
  free(flattened);

  // Test removing root with two children
  bst_remove(tree, 42);
  TEST_ASSERT_EQUAL(4, bst_get_count(tree));
  flattened = bst_flatten(tree);
  TEST_ASSERT_EQUAL(10, flattened[1]);
  TEST_ASSERT_EQUAL(99, flattened[2]);
  free(flattened);
}