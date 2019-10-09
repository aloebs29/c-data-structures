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
TEST_FILE("helpers.c");
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

  TEST_ASSERT_EQUAL(1, table->_data[39]->_head->_data->value);
  TEST_ASSERT_EQUAL(2, table->_data[31]->_head->_data->value);
  TEST_ASSERT_EQUAL(3, table->_data[29]->_head->_data->value);
  TEST_ASSERT_EQUAL(4, table->_data[33]->_head->_data->value);
  TEST_ASSERT_EQUAL(5, table->_data[19]->_head->_data->value);
  TEST_ASSERT_EQUAL(6, table->_data[19]->_tail->_data->value);
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
  // Based on GROW_AT_SCALE_FACTOR and default size, we need 512 entries to resize
  char buffer[16];
  for (int i = 0; i < 511; i++)
  {
    sprintf(buffer, "test_%d", i);
    dhash_table_add(table, buffer, i);
  }

  // Make sure it didn't grow yet
  TEST_ASSERT_EQUAL(64, table->_table_size);
  dhash_table_add(table, "test_511", 511);

  // Make sure it grew
  TEST_ASSERT_EQUAL(2048, table->_table_size); // based on growth factor

  // Test random values
  int out = 42;
  dhash_table_try_get(table, "test_511", &out);
  TEST_ASSERT_EQUAL(511, out);
  dhash_table_try_get(table, "test_51", &out);
  TEST_ASSERT_EQUAL(51, out);
  dhash_table_try_get(table, "test_232", &out);
  TEST_ASSERT_EQUAL(232, out);
  dhash_table_try_get(table, "test_0", &out);
  TEST_ASSERT_EQUAL(0, out);

  // Make sure adding new value works
  dhash_table_add(table, "one", 1);
  dhash_table_try_get(table, "one", &out);
  TEST_ASSERT_EQUAL(1, out);
}