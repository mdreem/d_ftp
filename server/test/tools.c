#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../tools.h"

static void test_is_newline(void **state) {
    int result = isnewline('a');
    assert_int_equal( result, 0 );

    result = isnewline('\n');
    assert_int_not_equal( result, 0 );
}

static void test_trim_whitespace(void **state) {
    char trim_string[] = " STRING ";
    const char test_string[] = "STRING";

    char *result_string = trim_whitespace(trim_string);
    int result = strcmp(result_string, test_string);

    assert_int_equal( result, 0 );
}

int main(void) {
    const UnitTest tests[] = {
        unit_test(test_is_newline),
        unit_test(test_trim_whitespace),
    };
    return run_tests(tests);
}
