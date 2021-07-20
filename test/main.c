#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
extern const struct CMUnitTest array_tests[];
extern const size_t array_num_tests;
extern const struct CMUnitTest darray_tests[];
extern const size_t darray_num_tests;
int main(void)
{
_cmocka_run_group_tests("array", array_tests, array_num_tests, NULL, NULL);
_cmocka_run_group_tests("darray", darray_tests, darray_num_tests, NULL, NULL);
return 0;
}