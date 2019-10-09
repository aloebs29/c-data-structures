#include "unity.h"
#include "dhash_table.h"

/*----------------------------------
  Collision found by SE user Ian Boyd 
  https://softwareengineering.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed

  Hashes for table size 64 (default):
    Hash of "one" is: 39
    Hash of "two" is: 31
    Hash of "three" is: 29
    Hash of "four" is: 33
    Hash of "heliotropes" is: 19
    Hash of "neurospora" is: 19
  ---------------------------*/

// Force linkage
TEST_FILE("dkey_val_list.c");

dhash_table_t * table = NULL;

void setUp(void)
{
  table = dhash_table_new();
}

void tearDown(void)
{
  dhash_table_destroy(table);
}

void test_add()
{
  TEST_ASSERT_EQUAL(0, table->_count);
  dhash_table_add(table, "one", 1);
  TEST_ASSERT_EQUAL(1, table->_count);
  dhash_table_add(table, "two", 2);
  dhash_table_add(table, "three", 3);
  dhash_table_add(table, "four", 4);
  dhash_table_add(table, "heliotropes", 5);
  dhash_table_add(table, "neurospora", 6);
  TEST_ASSERT_EQUAL(6, table->_count);

  TEST_ASSERT_EQUAL(1, table->_data[39]->_head->_data->_value);
  TEST_ASSERT_EQUAL(2, table->_data[31]->_head->_data->_value);
  TEST_ASSERT_EQUAL(3, table->_data[29]->_head->_data->_value);
  TEST_ASSERT_EQUAL(4, table->_data[33]->_head->_data->_value);
  TEST_ASSERT_EQUAL(5, table->_data[19]->_head->_data->_value);
  TEST_ASSERT_EQUAL(6, table->_data[19]->_tail->_data->_value);
}

void test_exists()
{
  // Test on empty
  TEST_ASSERT_FALSE(dhash_table_exists(table, "abc"));

  dhash_table_add(table, "one", 1);
  TEST_ASSERT_TRUE(dhash_table_exists(table, "one"));
  TEST_ASSERT_FALSE(dhash_table_exists(table, "two"));

  // Test on collision
  dhash_table_add(table, "heliotropes", 5);
  TEST_ASSERT_TRUE(dhash_table_exists(table, "heliotropes"));
  TEST_ASSERT_FALSE(dhash_table_exists(table, "neurospora"));
  dhash_table_add(table, "neurospora", 6);
  TEST_ASSERT_TRUE(dhash_table_exists(table, "heliotropes"));
  TEST_ASSERT_TRUE(dhash_table_exists(table, "neurospora"));
}

void test_try_get()
{
  // Test on empty
  int out = 42;
  TEST_ASSERT_FALSE(dhash_table_try_get(table, "abc", &out));

  dhash_table_add(table, "one", 1);
  TEST_ASSERT_TRUE(dhash_table_try_get(table, "one", &out));
  TEST_ASSERT_EQUAL(1, out);
  TEST_ASSERT_FALSE(dhash_table_try_get(table, "two", &out));

  // Test on collision
  dhash_table_add(table, "heliotropes", 5);
  TEST_ASSERT_TRUE(dhash_table_try_get(table, "heliotropes", &out));
  TEST_ASSERT_EQUAL(5, out);
  TEST_ASSERT_FALSE(dhash_table_try_get(table, "neurospora", &out));
  dhash_table_add(table, "neurospora", 6);
  TEST_ASSERT_TRUE(dhash_table_try_get(table, "heliotropes", &out));
  TEST_ASSERT_EQUAL(5, out);
  TEST_ASSERT_TRUE(dhash_table_try_get(table, "neurospora", &out));
  TEST_ASSERT_EQUAL(6, out);
}

void test_remove()
{
  dhash_table_add(table, "one", 1);
  dhash_table_add(table, "two", 2);
  dhash_table_add(table, "three", 3);
  dhash_table_add(table, "four", 4);
  dhash_table_add(table, "heliotropes", 5);
  dhash_table_add(table, "neurospora", 6);

  dhash_table_remove(table, "one");
  TEST_ASSERT_FALSE(dhash_table_exists(table, "one"));
  TEST_ASSERT_EQUAL(5, table->_count);
  dhash_table_remove(table, "heliotropes");
  TEST_ASSERT_FALSE(dhash_table_exists(table, "heliotropes"));
  TEST_ASSERT_TRUE(dhash_table_exists(table, "neurospora"));
  TEST_ASSERT_EQUAL(4, table->_count);
}

void test_resize()
{
  TEST_IGNORE_MESSAGE("Need to implement hash table resize.");
}