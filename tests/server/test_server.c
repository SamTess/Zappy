#include <criterion/criterion.h>

Test(server_compilation, dummy_test) {
    cr_assert(1, "Compilation OK");
}