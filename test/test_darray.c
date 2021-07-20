#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "darray.h"
#include "array.h"

// NOTE (aloebs): These are defined in the uut's implementation file. It feels gross copying them, 
// but I don't want to put them in the uut's header.
#define DEFAULT_CAPACITY        16
#define GROWTH_FACTOR           2
#define SHRINK_AT_DIVISOR       4

#define ASSERT_GOOD_RET(expr) assert_int_equal(0, expr)
#define ASSERT_BAD_RET(expr) assert_int_not_equal(0, expr)
#define ASSERT_SIZE_EQUAL(d, expected) assert_int_equal(expected, (d)->_size)
#define ASSERT_VALUE_AT_INDEX(d, index, expected) assert_int_equal(expected, ((int *)(d)->_data)[index])

int setup(void **state)
{
    struct darray *d = darray_new(sizeof(int));
    if (!d) return -1;

    *state = d;
    return 0;
}

int teardown(void **state)
{
    darray_destroy(*state);
    return 0;
}

void test_new(void **state) //// HALFLING_NO_SETUP
{
    struct darray *d = darray_new(sizeof(int));

    assert_non_null(d);
    assert_non_null(d->_data);
    assert_int_equal(sizeof(int), d->_item_size);
    ASSERT_SIZE_EQUAL(d, 0);
    assert_int_equal(DEFAULT_CAPACITY * sizeof(int), d->_capacity);
}

void test_extend(void **state)
{
    struct darray *d = *state;
    // test edge case
    ASSERT_GOOD_RET(darray_extend(d, NULL, 0));
    ASSERT_SIZE_EQUAL(d, 0);

    // when empty
    int ta[] = { 1, 2, 3, 4, 5, 6, 7 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta) - 3));
    ASSERT_SIZE_EQUAL(d, (ARRAY_COUNT(ta) - 3) * sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, 1);
    ASSERT_VALUE_AT_INDEX(d, 1, 2);
    ASSERT_VALUE_AT_INDEX(d, 2, 3);
    ASSERT_VALUE_AT_INDEX(d, 3, 4);

    // when non-empty
    ASSERT_GOOD_RET(darray_extend(d, &ta[ARRAY_COUNT(ta) - 3], 3));
    ASSERT_SIZE_EQUAL(d, ARRAY_COUNT(ta) * sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, 1);
    ASSERT_VALUE_AT_INDEX(d, 1, 2);
    ASSERT_VALUE_AT_INDEX(d, 2, 3);
    ASSERT_VALUE_AT_INDEX(d, 3, 4);
    ASSERT_VALUE_AT_INDEX(d, 4, 5);
    ASSERT_VALUE_AT_INDEX(d, 5, 6);
    ASSERT_VALUE_AT_INDEX(d, 6, 7);
}

void test_append(void **state)
{
    struct darray *d = *state;

    // when empty
    int td = 42;
    ASSERT_GOOD_RET(darray_append(d, &td));
    ASSERT_SIZE_EQUAL(d, sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, td);

    // when non-empty
    int td1 = 55;
    ASSERT_GOOD_RET(darray_append(d, &td1));
    ASSERT_SIZE_EQUAL(d, 2 * sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, td);
    ASSERT_VALUE_AT_INDEX(d, 1, td1);
}

void test_insert(void **state)
{
    struct darray *d = *state;

    // OOB
    int td = 77;
    ASSERT_BAD_RET(darray_insert(d, &td, 1));

    // empty
    ASSERT_GOOD_RET(darray_insert(d, &td, 0));
    ASSERT_SIZE_EQUAL(d, sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, td);

    // after
    int td1 = 32, td2 = 45, td3 = 51;
    ASSERT_GOOD_RET(darray_insert(d, &td1, 1));
    ASSERT_SIZE_EQUAL(d, 2 * sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, td);
    ASSERT_VALUE_AT_INDEX(d, 1, td1);
    // middle
    ASSERT_GOOD_RET(darray_insert(d, &td2, 1));
    ASSERT_SIZE_EQUAL(d, 3 * sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, td);
    ASSERT_VALUE_AT_INDEX(d, 1, td2);
    ASSERT_VALUE_AT_INDEX(d, 2, td1);
    // before
    ASSERT_GOOD_RET(darray_insert(d, &td3, 0));
    ASSERT_SIZE_EQUAL(d, 4 * sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, td3);
    ASSERT_VALUE_AT_INDEX(d, 1, td);
    ASSERT_VALUE_AT_INDEX(d, 2, td2);
    ASSERT_VALUE_AT_INDEX(d, 3, td1);
}

void test_pop(void **state)
{
    struct darray *d = *state;

    // empty
    darray_pop(d);
    ASSERT_SIZE_EQUAL(d, 0);

    // normal case
    int ta[] = { 1, 2, 3, 4, 5 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));

    darray_pop(d);
    ASSERT_SIZE_EQUAL(d, sizeof(int) * (ARRAY_COUNT(ta) - 1));
    ASSERT_VALUE_AT_INDEX(d, 0, ta[0]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta[1]);
    ASSERT_VALUE_AT_INDEX(d, 2, ta[2]);
    ASSERT_VALUE_AT_INDEX(d, 3, ta[3]);

    darray_pop(d);
    ASSERT_SIZE_EQUAL(d, sizeof(int) * (ARRAY_COUNT(ta) - 2));
    ASSERT_VALUE_AT_INDEX(d, 0, ta[0]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta[1]);
    ASSERT_VALUE_AT_INDEX(d, 2, ta[2]);
}

void test_remove(void **state)
{
    struct darray *d = *state;

    // empty
    darray_remove(d, 0);
    ASSERT_SIZE_EQUAL(d, 0);

    // normal case
    int ta[] = { 1, 2, 3, 4, 5 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));

    // front
    darray_remove(d, 0);
    ASSERT_SIZE_EQUAL(d, sizeof(int) * (ARRAY_COUNT(ta) - 1));
    ASSERT_VALUE_AT_INDEX(d, 0, ta[1]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta[2]);
    ASSERT_VALUE_AT_INDEX(d, 2, ta[3]);
    ASSERT_VALUE_AT_INDEX(d, 3, ta[4]);

    // middle
    darray_remove(d, 1);
    ASSERT_SIZE_EQUAL(d, sizeof(int) * (ARRAY_COUNT(ta) - 2));
    ASSERT_VALUE_AT_INDEX(d, 0, ta[1]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta[3]);
    ASSERT_VALUE_AT_INDEX(d, 2, ta[4]);

    // end
    darray_remove(d, 2);
    ASSERT_SIZE_EQUAL(d, sizeof(int) * (ARRAY_COUNT(ta) - 3));
    ASSERT_VALUE_AT_INDEX(d, 0, ta[1]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta[3]);

    // OOB
    darray_remove(d, 2);
    ASSERT_SIZE_EQUAL(d, sizeof(int) * (ARRAY_COUNT(ta) - 3));
    ASSERT_VALUE_AT_INDEX(d, 0, ta[1]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta[3]);
}

void test_clear(void **state)
{
    struct darray *d = *state;

    // empty
    darray_clear(d);
    ASSERT_SIZE_EQUAL(d, 0);

    // normal
    int ta[] = { 1, 2, 3, 4, 5 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));
    darray_clear(d);
    ASSERT_SIZE_EQUAL(d, 0);

    // replacement works
    int ta1[] = { 11, 12, 13, 14, 15 };
    ASSERT_GOOD_RET(darray_extend(d, ta1, ARRAY_COUNT(ta1)));
    ASSERT_SIZE_EQUAL(d, sizeof(int) * ARRAY_COUNT(ta1));
    ASSERT_VALUE_AT_INDEX(d, 0, ta1[0]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta1[1]);
    ASSERT_VALUE_AT_INDEX(d, 2, ta1[2]);
    ASSERT_VALUE_AT_INDEX(d, 3, ta1[3]);
    ASSERT_VALUE_AT_INDEX(d, 4, ta1[4]);
}

void test_count(void **state)
{
    struct darray *d = *state;

    // empty
    assert_int_equal(0, DARRAY_COUNT(d));

    // normal
    int ta[] = { 1, 2, 3, 4, 5 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));
    assert_int_equal(ARRAY_COUNT(ta), DARRAY_COUNT(d));
}

void test_at(void **state)
{
    struct darray *d = *state;

    int ta[] = { 1, 2, 3, 4, 5 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));

    assert_int_equal(ta[0], DARRAY_AT(d, 0, int));
    assert_int_equal(ta[1], DARRAY_AT(d, 1, int));
    assert_int_equal(ta[2], DARRAY_AT(d, 2, int));
    assert_int_equal(ta[3], DARRAY_AT(d, 3, int));
    assert_int_equal(ta[4], DARRAY_AT(d, 4, int));
}

void test_pforeach(void **state)
{
    struct darray *d = *state;

    int ta[] = { 1, 2, 3, 4, 5 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));

    int i = 0;
    DARRAY_PFOREACH(d, p, int) {
        assert_int_equal(ta[i], *p);
        i++;
    }
}
