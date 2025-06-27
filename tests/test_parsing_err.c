/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_parsing_err - tests for parsing error handling with exit(84)
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/server.h"
#include <stdlib.h>
#include <string.h>

// Test suite setup
TestSuite(parsing_err_tests);

// =============================================================================
// HELPER FUNCTIONS
// =============================================================================

/**
 * Create a test parsing_info with team names
 */
static parsing_info_t *create_test_parsing_info_with_names(const char **names, int count)
{
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    
    if (count > 0 && names) {
        info->names = calloc(count + 1, sizeof(char *));
        for (int i = 0; i < count; i++) {
            info->names[i] = strdup(names[i]);
        }
        info->names[count] = NULL;
    } else {
        info->names = NULL;
    }
    
    return info;
}

/**
 * Create a test parsing_info without team names
 */
static parsing_info_t *create_test_parsing_info_empty(void)
{
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    info->names = NULL;
    info->port = 0;
    info->width = 0;
    info->height = 0;
    info->client_nb = 0;
    info->frequence = 100;
    return info;
}

// =============================================================================
// REAL PARSING_ERROR TESTS WITH EXIT(84)
// =============================================================================

Test(parsing_err_tests, test_parsing_error_with_team_names, .exit_code = 84)
{
    // Test parsing_error with team names - should free names and exit(84)
    const char *team_names[] = {"team1", "team2", "team3"};
    parsing_info_t *info = create_test_parsing_info_with_names(team_names, 3);
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // This will call printf, display_help, and exit(84)
    parsing_error("Test error with team names", info);
    
    // This line should never be reached due to exit(84)
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}

Test(parsing_err_tests, test_parsing_error_with_null_names, .exit_code = 84)
{
    // Test parsing_error with NULL names - should skip free_names and exit(84)
    parsing_info_t *info = create_test_parsing_info_empty();
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    parsing_error("Test error with NULL names", info);
    
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}

Test(parsing_err_tests, test_parsing_error_with_empty_names_array, .exit_code = 84)
{
    // Test parsing_error where names != NULL but names[0] == NULL
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    info->names = calloc(1, sizeof(char *));
    info->names[0] = NULL; // Empty names array
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    parsing_error("Test error with empty names array", info);
    
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}

Test(parsing_err_tests, test_parsing_error_single_team, .exit_code = 84)
{
    // Test with single team name
    const char *team_names[] = {"single_team"};
    parsing_info_t *info = create_test_parsing_info_with_names(team_names, 1);
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    parsing_error("Test error with single team", info);
    
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}

Test(parsing_err_tests, test_parsing_error_many_teams, .exit_code = 84)
{
    // Test with many team names to exercise the free_names loop
    const char *team_names[] = {"red", "blue", "green", "yellow", "purple", "orange"};
    parsing_info_t *info = create_test_parsing_info_with_names(team_names, 6);
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    parsing_error("Test error with many teams", info);
    
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}

Test(parsing_err_tests, test_parsing_error_long_message, .exit_code = 84)
{
    // Test with long error message
    parsing_info_t *info = create_test_parsing_info_empty();
    char long_message[500];
    strcpy(long_message, "This is a very long error message that tests how the parsing_error function handles lengthy strings. ");
    strcat(long_message, "It should still work correctly regardless of the message length and properly display the full message to stdout.");
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    parsing_error(long_message, info);
    
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}

Test(parsing_err_tests, test_parsing_error_special_chars, .exit_code = 84)
{
    // Test with special characters in team names and error message
    const char *team_names[] = {"team@#$", "team_with_underscores", "team-with-dashes"};
    parsing_info_t *info = create_test_parsing_info_with_names(team_names, 3);
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    parsing_error("Error: Invalid characters in team names! @#$%^&*()", info);
    
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}

// =============================================================================
// BRANCH COVERAGE TESTS
// =============================================================================

Test(parsing_err_tests, test_free_names_branch_coverage, .exit_code = 84)
{
    // Ensure the branch where names != NULL AND names[0] != NULL is tested
    // This will call free_names function
    const char *team_names[] = {"test_team"};
    parsing_info_t *info = create_test_parsing_info_with_names(team_names, 1);
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    parsing_error("Branch coverage test for free_names", info);
    
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}

Test(parsing_err_tests, test_no_free_names_branch_coverage, .exit_code = 84)
{
    // Ensure the branch where free_names is NOT called is tested
    // This happens when names == NULL OR names[0] == NULL
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    info->names = NULL;
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    parsing_error("Branch coverage test - no free_names call", info);
    
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}

Test(parsing_err_tests, test_free_names_loop_coverage, .exit_code = 84)
{
    // Test the loop inside free_names with multiple iterations
    const char *team_names[] = {"team1", "team2", "team3", "team4", "team5"};
    parsing_info_t *info = create_test_parsing_info_with_names(team_names, 5);
    
    // Rediriger la sortie pour supprimer les messages d'usage répétitifs
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    parsing_error("Testing free_names loop with multiple teams", info);
    
    cr_assert_fail("Should not reach this point - exit(84) should have been called");
}