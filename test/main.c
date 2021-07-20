#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "test_array.c"
#include "test_darray.c"
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

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}