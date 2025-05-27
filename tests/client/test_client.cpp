#include <criterion/criterion.h>

Test(client_compilation, dummy_test) {
    cr_assert(1, "Compilation OK");
}