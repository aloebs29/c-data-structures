/**
 * @file		test_array.c
 * @author		Andrew Loebs
 * @brief		Tests for the array module.
 *
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "array.h"

static void test_count(void **state)
{
    // empty
    int ta[0];
    assert_int_equal(0, ARRAY_COUNT(ta));

    // normal
    int ta1[5] = { 17, 33, 42, 55, 19 };
    assert_int_equal(5, ARRAY_COUNT(ta1));
}

static void test_pforeach(void **state)
{
    // empty
    int ta[0];
    ARRAY_PFOREACH(ta, p, int) {
        assert_true(0); // should not reach here
    }

    // normal
    int ta1[] = { 21, 23, 99, 120873, -123, 55 };
    int i = 0;
    ARRAY_PFOREACH(ta1, p, int) {
        assert_int_equal(ta1[i], *p);
        i++;
    }
    assert_int_equal(i, ARRAY_COUNT(ta1));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_count),
        cmocka_unit_test(test_pforeach),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
