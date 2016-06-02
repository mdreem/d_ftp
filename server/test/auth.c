#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>


#include "../auth.h"

int __wrap_pam_start() {
    return mock();
}

int __wrap_pam_authenticate() {
    return mock();
}

int __wrap_pam_end() {
    return mock();
}

static void test_authenticate(void **state) {
    will_return(__wrap_pam_start, 0);
    will_return(__wrap_pam_authenticate, PAM_SUCCESS);
    will_return(__wrap_pam_end, 0);

    char username[] = "username";
    char password[] = "password";

    authenticate(username, password);

}

int main(void) {
    const UnitTest tests[] = {
        unit_test(test_authenticate)
    };
    return run_tests(tests);
}
