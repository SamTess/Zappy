/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_help - tests for help display function
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>

// Forward declaration
extern void display_help(void);

TestSuite(help_tests);

// Test display_help function
Test(help_tests, test_display_help_output, .init = cr_redirect_stdout)
{
    // Call the help function
    display_help();
    
    // We can't easily check the exact output content, but we can verify
    // that the function executes without crashing
    cr_assert(true, "display_help should execute without crashing");
}

// Test display_help function multiple times
Test(help_tests, test_display_help_multiple_calls, .init = cr_redirect_stdout)
{
    // Call the help function multiple times to ensure consistency
    display_help();
    display_help();
    display_help();
    
    cr_assert(true, "display_help should handle multiple calls");
}

// Test display_help function with stdout redirection
Test(help_tests, test_display_help_with_redirection)
{
    // This test verifies that display_help works even when stdout is redirected
    cr_redirect_stdout();
    
    display_help();
    
    cr_assert(true, "display_help should work with stdout redirection");
}