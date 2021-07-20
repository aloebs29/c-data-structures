#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>

#include "darray.h"
#include "array.h"


#define ASSERT_GOOD_RET(expr) assert_int_equal(0, expr)
#define ASSERT_BAD_RET(expr) assert_int_not_equal(0, expr)
#define ASSERT_SIZE_EQUAL(d, expected) assert_int_equal(expected, (d)->_size)
#define ASSERT_VALUE_AT_INDEX(d, index, expected) assert_int_equal(expected, ((int *)(d)->_data)[index])

static int setup(void **state)
{
    struct darray *d = darray_new(sizeof(int));
    if (!d) return -1;

    *state = d;
    return 0;
}

static int teardown(void **state)
{
    darray_destroy(*state);
    return 0;
}

static void test_new(void **state)
{
    struct darray *d = darray_new(sizeof(int));

    assert_non_null(d);
    assert_non_null(d->_data);
    assert_int_equal(sizeof(int), d->_item_size);
    ASSERT_SIZE_EQUAL(d, 0);
}

static void test_extend(void **state)
{
    struct darray *d = *state;
    // test edge case
    ASSERT_GOOD_RET(darray_extend(d, NULL, 0));
    ASSERT_SIZE_EQUAL(d, 0);

    // when empty
    int ta[] = { 99, 12, 777, 444, 12398, 612, 70912 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta) - 3));
    ASSERT_SIZE_EQUAL(d, (ARRAY_COUNT(ta) - 3) * sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, ta[0]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta[1]);
    ASSERT_VALUE_AT_INDEX(d, 2, ta[2]);
    ASSERT_VALUE_AT_INDEX(d, 3, ta[3]);

    // when non-empty
    ASSERT_GOOD_RET(darray_extend(d, &ta[ARRAY_COUNT(ta) - 3], 3));
    ASSERT_SIZE_EQUAL(d, ARRAY_COUNT(ta) * sizeof(int));
    ASSERT_VALUE_AT_INDEX(d, 0, ta[0]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta[1]);
    ASSERT_VALUE_AT_INDEX(d, 2, ta[2]);
    ASSERT_VALUE_AT_INDEX(d, 3, ta[3]);
    ASSERT_VALUE_AT_INDEX(d, 4, ta[4]);
    ASSERT_VALUE_AT_INDEX(d, 5, ta[5]);
    ASSERT_VALUE_AT_INDEX(d, 6, ta[6]);
}

static void test_append(void **state)
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

static void test_insert(void **state)
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

static void test_pop(void **state)
{
    struct darray *d = *state;

    // empty
    darray_pop(d);
    ASSERT_SIZE_EQUAL(d, 0);

    // normal case
    int ta[] = { 17, 33, 42, 55, 19 };
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

static void test_remove(void **state)
{
    struct darray *d = *state;

    // empty
    darray_remove(d, 0);
    ASSERT_SIZE_EQUAL(d, 0);

    // normal case
    int ta[] = { 17, 33, 42, 55, 19 };
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

static void test_clear(void **state)
{
    struct darray *d = *state;

    // empty
    darray_clear(d);
    ASSERT_SIZE_EQUAL(d, 0);

    // normal
    int ta[] = { 17, 33, 42, 55, 19 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));
    darray_clear(d);
    ASSERT_SIZE_EQUAL(d, 0);

    // replacement works
    int ta1[] = { 19, 32, 43, 90004, 15 };
    ASSERT_GOOD_RET(darray_extend(d, ta1, ARRAY_COUNT(ta1)));
    ASSERT_SIZE_EQUAL(d, sizeof(int) * ARRAY_COUNT(ta1));
    ASSERT_VALUE_AT_INDEX(d, 0, ta1[0]);
    ASSERT_VALUE_AT_INDEX(d, 1, ta1[1]);
    ASSERT_VALUE_AT_INDEX(d, 2, ta1[2]);
    ASSERT_VALUE_AT_INDEX(d, 3, ta1[3]);
    ASSERT_VALUE_AT_INDEX(d, 4, ta1[4]);
}

static void test_count(void **state)
{
    struct darray *d = *state;

    // empty
    assert_int_equal(0, DARRAY_COUNT(d));

    // normal
    int ta[] = { 17, 33, 42, 55, 19 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));
    assert_int_equal(ARRAY_COUNT(ta), DARRAY_COUNT(d));
}

static void test_at(void **state)
{
    struct darray *d = *state;

    int ta[] = { 17, 33, 42, 55, 19 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));

    assert_int_equal(ta[0], DARRAY_AT(d, 0, int));
    assert_int_equal(ta[1], DARRAY_AT(d, 1, int));
    assert_int_equal(ta[2], DARRAY_AT(d, 2, int));
    assert_int_equal(ta[3], DARRAY_AT(d, 3, int));
    assert_int_equal(ta[4], DARRAY_AT(d, 4, int));
}

static void test_pforeach(void **state)
{
    struct darray *d = *state;

    // empty
    DARRAY_PFOREACH(d, p, int) {
        assert_true(0); // should not reach here
    }

    // normal
    int ta[] = { 17, 33, 42, 55, 19 };
    ASSERT_GOOD_RET(darray_extend(d, ta, ARRAY_COUNT(ta)));
    int i = 0;
    DARRAY_PFOREACH(d, p, int) {
        assert_int_equal(ta[i], *p);
        i++;
    }
    assert_int_equal(i, ARRAY_COUNT(ta));
}

static void test_resizing(void **state)
{
    struct darray *d = *state;

    // NOTE (aloebs): these are defined in the uut's impl file; duplicating the values is kind of
    // gross but these don't belong in the uut's header. This test will need to be updated if these
    // values change in the uut.
    const int default_capacity = 16;
    const int growth_factor = 2;
    // NOTE (aloebs): this test only works if shrink_at_divisor <= growth_factor^3
    const int shrink_at_divisor = 4; 

    // empty
    assert_int_equal(default_capacity * sizeof(int), d->_capacity);

    // initialize test array
    unsigned int ta_count = default_capacity * growth_factor * growth_factor * growth_factor;
    int *ta = (int *)malloc(sizeof(int) * ta_count);
    int ta_i_to_val_mul = 3;
    for (int i = 0; i < ta_count; i++) ta[i] = i * ta_i_to_val_mul;

    // doesn't grow until needed
    ASSERT_GOOD_RET(darray_extend(d, ta, default_capacity));
    assert_int_equal(default_capacity * sizeof(int), d->_capacity);
    // grows as expected and contents intact
    ASSERT_GOOD_RET(darray_extend(d, &ta[default_capacity], 1));
    assert_int_equal(default_capacity * growth_factor * sizeof(int), d->_capacity);
    for (int i = 0; i < (default_capacity + 1); i++) ASSERT_VALUE_AT_INDEX(d, i, ta[i]);

    darray_clear(d);

    // will grow above growth factor if needed
    ASSERT_GOOD_RET(darray_extend(d, ta, ta_count));
    assert_int_equal(ta_count * sizeof(int), d->_capacity);
    for (int i = 0; i < ta_count; i++) ASSERT_VALUE_AT_INDEX(d, i, ta[i]);

    // doesn't shrink too soon
    unsigned int pop_count = ta_count - (ta_count / shrink_at_divisor);
    for (int i = 0; i < (pop_count - 1); i++) darray_pop(d);
    assert_int_equal(ta_count * sizeof(int), d->_capacity);

    // shrinks
    darray_pop(d);
    assert_int_equal((ta_count * sizeof(int)) / growth_factor, d->_capacity);

    // doesn't shrink past default capacity
    darray_clear(d);
    assert_int_equal(default_capacity * sizeof(int), d->_capacity);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_new),
        cmocka_unit_test_setup_teardown(test_extend, setup, teardown),
        cmocka_unit_test_setup_teardown(test_append, setup, teardown),
        cmocka_unit_test_setup_teardown(test_insert, setup, teardown),
        cmocka_unit_test_setup_teardown(test_pop, setup, teardown),
        cmocka_unit_test_setup_teardown(test_remove, setup, teardown),
        cmocka_unit_test_setup_teardown(test_clear, setup, teardown),
        cmocka_unit_test_setup_teardown(test_count, setup, teardown),
        cmocka_unit_test_setup_teardown(test_at, setup, teardown),
        cmocka_unit_test_setup_teardown(test_pforeach, setup, teardown),
        cmocka_unit_test_setup_teardown(test_resizing, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
