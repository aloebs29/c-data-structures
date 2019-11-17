#include "unity.h"
#include "sort.h"

// Force linkage
TEST_FILE("max_heap.c");
TEST_FILE("helpers.c");

static int arr1[2];
static int arr2[12];
static int arr3[7];

// Helper function prototypes
static void setup_arr1();
static void setup_arr2();
static void setup_arr3();

static void check_against_arr1(void (*sort)(int * arr, int len));
static void check_against_arr2(void (*sort)(int * arr, int len));
static void check_against_arr3(void (*sort)(int * arr, int len));

// Ceedling special functions
void setUp(void)
{
  setup_arr1();
  setup_arr2();
  setup_arr3();    
}

void tearDown(void)
{
}

// Test functions
void test_quick_sort()
{
  check_against_arr1(quick_sort);
  check_against_arr2(quick_sort);
  check_against_arr3(quick_sort);
}

void test_heap_sort()
{
  check_against_arr1(heap_sort);
  check_against_arr2(heap_sort);
  check_against_arr3(heap_sort);
}

void test_merge_sort()
{
  check_against_arr1(merge_sort);
  check_against_arr2(merge_sort);
  check_against_arr3(merge_sort);
}

// Helper function definitions
static void setup_arr1()
{
  arr1[0] = 1;
  arr1[1] = 0;
}

static void setup_arr2()
{
  arr2[0] = 53;
  arr2[1] = 42;
  arr2[2] = 11;
  arr2[3] = 97;
  arr2[4] = -3;
  arr2[5] = 54;
  arr2[6] = 22;
  arr2[7] = 5;
  arr2[8] = 6;
  arr2[9] = 2;
  arr2[10] = 990;
  arr2[11] = 101;
}

static void setup_arr3()
{
  arr3[0] = -33;
  arr3[1] = -33;
  arr3[2] = -4;
  arr3[3] = -5;
  arr3[4] = -21;
  arr3[5] = -33;
  arr3[6] = -32768;
}

static void check_against_arr1(void (*sort)(int * arr, int len))
{
  sort(arr1, sizeof(arr1) / sizeof(*arr1));

  TEST_ASSERT_EQUAL(0, arr1[0]);
  TEST_ASSERT_EQUAL(1, arr1[1]);
}

static void check_against_arr2(void (*sort)(int * arr, int len))
{
  sort(arr2, sizeof(arr2) / sizeof(*arr2));

  TEST_ASSERT_EQUAL(-3, arr2[0]);
  TEST_ASSERT_EQUAL(2, arr2[1]);
  TEST_ASSERT_EQUAL(5, arr2[2]);
  TEST_ASSERT_EQUAL(6, arr2[3]);
  TEST_ASSERT_EQUAL(11, arr2[4]);
  TEST_ASSERT_EQUAL(22, arr2[5]);
  TEST_ASSERT_EQUAL(42, arr2[6]);
  TEST_ASSERT_EQUAL(53, arr2[7]);
  TEST_ASSERT_EQUAL(54, arr2[8]);
  TEST_ASSERT_EQUAL(97, arr2[9]);
  TEST_ASSERT_EQUAL(101, arr2[10]);
  TEST_ASSERT_EQUAL(990, arr2[11]);
}

static void check_against_arr3(void (*sort)(int * arr, int len))
{
  sort(arr3, sizeof(arr3) / sizeof(*arr3));

  TEST_ASSERT_EQUAL(-32768, arr3[0]);
  TEST_ASSERT_EQUAL(-33, arr3[1]);
  TEST_ASSERT_EQUAL(-33, arr3[2]);
  TEST_ASSERT_EQUAL(-33, arr3[3]);
  TEST_ASSERT_EQUAL(-21, arr3[4]);
  TEST_ASSERT_EQUAL(-5, arr3[5]);
  TEST_ASSERT_EQUAL(-4, arr3[6]);
}