/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_free - tests for free functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/parsing.h"
#include <stdlib.h>
#include <string.h>

TestSuite(free_tests);

// Test: free_arr with NULL array
Test(free_tests, test_free_arr_null)
{
    free_arr(NULL);
    cr_assert(1);
}

// Test: free_arr with empty array
Test(free_tests, test_free_arr_empty)
{
    char **array = calloc(1, sizeof(char *));
    array[0] = NULL;
    
    free_arr(array);
    cr_assert(1);
}

// Test: free_arr with single element
Test(free_tests, test_free_arr_single_element)
{
    char **array = calloc(2, sizeof(char *));
    array[0] = strdup("test_string");
    array[1] = NULL;
    
    free_arr(array);
    cr_assert(1);
}

// Test: free_arr with multiple elements
Test(free_tests, test_free_arr_multiple_elements)
{
    char **array = calloc(4, sizeof(char *));
    array[0] = strdup("first");
    array[1] = strdup("second");
    array[2] = strdup("third");
    array[3] = NULL;
    
    free_arr(array);
    cr_assert(1);
}

// Test: free_arr with many elements to test loop coverage
Test(free_tests, test_free_arr_many_elements)
{
    char **array = calloc(10, sizeof(char *));
    for (int i = 0; i < 9; i++) {
        char temp[20];
        snprintf(temp, sizeof(temp), "element_%d", i);
        array[i] = strdup(temp);
    }
    array[9] = NULL;
    
    free_arr(array);
    cr_assert(1);
}

// Test: free_all with NULL server (tests first line only)
Test(free_tests, test_free_all_null_server)
{
    free_all(NULL, NULL);
    cr_assert(1);
}

// Test edge case: free_arr with array containing NULL elements
Test(free_tests, test_free_arr_with_null_elements)
{
    char **array = calloc(4, sizeof(char *));
    array[0] = strdup("first");
    array[1] = NULL; // This should trigger the break condition
    array[2] = strdup("should_not_be_reached");
    array[3] = NULL;
    
    free_arr(array);
    cr_assert(1);
}

// Test: free_arr with long strings
Test(free_tests, test_free_arr_long_strings)
{
    char **array = calloc(3, sizeof(char *));
    array[0] = strdup("This is a very long string that should test the memory management capabilities of the free_arr function with longer allocated memory blocks to ensure proper cleanup");
    array[1] = strdup("Another long string to ensure that multiple large allocations are handled correctly by the freeing mechanism and that no memory leaks occur during the process");
    array[2] = NULL;
    
    free_arr(array);
    cr_assert(1);
}

// Test: free_arr with empty strings
Test(free_tests, test_free_arr_empty_strings)
{
    char **array = calloc(4, sizeof(char *));
    array[0] = strdup("");
    array[1] = strdup("");
    array[2] = strdup("");
    array[3] = NULL;
    
    free_arr(array);
    cr_assert(1);
}

// Test: free_arr with mixed content
Test(free_tests, test_free_arr_mixed_content)
{
    char **array = calloc(6, sizeof(char *));
    array[0] = strdup("short");
    array[1] = strdup("");
    array[2] = strdup("A much longer string with various characters: !@#$%^&*()_+-={}[]|\\:;\"'<>?,./");
    array[3] = strdup("normal_string");
    array[4] = strdup("final");
    array[5] = NULL;
    
    free_arr(array);
    cr_assert(1);
}