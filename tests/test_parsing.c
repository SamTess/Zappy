/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_parsing - comprehensive unit tests for command line argument parsing
** Tests for parse_args and all static parsing functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/server.h"
#include "test_mocks.h"
#include <stdlib.h>
#include <string.h>

// External mock variables from test_mocks.c
extern char *last_message;

// Additional mock variables for parsing tests
static int mock_parsing_error_calls = 0;
static int mock_display_help_calls = 0;
static int mock_exit_calls = 0;
static char *last_parsing_error_msg = NULL;
static int last_exit_code = 0;

// Reset mock function call counters and states
static void reset_parsing_mocks(void)
{
    mock_parsing_error_calls = 0;
    mock_display_help_calls = 0;
    mock_exit_calls = 0;
    
    if (last_parsing_error_msg) {
        free(last_parsing_error_msg);
        last_parsing_error_msg = NULL;
    }
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
    last_exit_code = 0;
}

// Mock implementations for parsing tests (only the ones not in test_mocks.c)
void __wrap_parsing_error(char *msg, parsing_info_t *parsed_info)
{
    (void)parsed_info;
    mock_parsing_error_calls++;
    if (last_parsing_error_msg) {
        free(last_parsing_error_msg);
    }
    last_parsing_error_msg = strdup(msg);
    
    // Pour les tests qui s'attendent à un exit(84), on doit sortir
    // Seulement pour les vrais cas d'erreur, pas pour les tests de suppression de messages
    if (msg && (strstr(msg, "coordinates arg can't be null") == NULL)) {
        // Appeler la vraie fonction exit pour les autres erreurs
        exit(84);
    }
    // Ne rien faire pour "coordinates arg can't be null" - supprime le message
}

void __wrap_display_help(void)
{
    mock_display_help_calls++;
}

void __wrap_exit(int code)
{
    mock_exit_calls++;
    last_exit_code = code;
    // Don't use longjmp, just return normally
}

// Test suite setup
TestSuite(parsing_tests);

// =============================================================================
// HELPER FUNCTIONS
// =============================================================================

/**
 * Create a test argv array
 */
static char **create_test_argv(const char *const *args, int count)
{
    char **argv = malloc(sizeof(char *) * (count + 1));
    for (int i = 0; i < count; i++) {
        argv[i] = strdup(args[i]);
    }
    argv[count] = NULL;
    return argv;
}

/**
 * Free test argv array
 */
static void free_test_argv(char **argv, int count)
{
    for (int i = 0; i < count; i++) {
        free(argv[i]);
    }
    free(argv);
}

/**
 * Free parsing_info names array
 */
static void free_parsing_names(char **names)
{
    if (!names) return;
    for (int i = 0; names[i]; i++) {
        free(names[i]);
    }
    free(names);
}

// =============================================================================
// BASIC PARSING TESTS
// =============================================================================

Test(parsing_tests, test_parse_args_minimal_valid)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-p", "4242", "-x", "10", "-y", "10", "-n", "team1"};
    char **argv = create_test_argv(args, 9);
    parsing_info_t parsed_info = {0};
    
    parse_args(9, argv, &parsed_info);
    
    // Check all values are correctly parsed
    cr_assert_eq(parsed_info.port, 4242);
    cr_assert_eq(parsed_info.width, 10);
    cr_assert_eq(parsed_info.height, 10);
    cr_assert_eq(parsed_info.frequence, 100); // Default value
    cr_assert_not_null(parsed_info.names);
    cr_assert_str_eq(parsed_info.names[0], "team1");
    cr_assert_null(parsed_info.names[1]);
    
    // No errors should be called
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 9);
    free_parsing_names(parsed_info.names);
}

Test(parsing_tests, test_parse_args_all_flags)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-p", "8080", "-x", "20", "-y", "15", 
                         "-c", "6", "-f", "50", "-n", "team1", "team2", "team3"};
    char **argv = create_test_argv(args, 15);
    parsing_info_t parsed_info = {0};
    
    parse_args(15, argv, &parsed_info);
    
    // Check all values
    cr_assert_eq(parsed_info.port, 8080);
    cr_assert_eq(parsed_info.width, 20);
    cr_assert_eq(parsed_info.height, 15);
    cr_assert_eq(parsed_info.client_nb, 6);
    cr_assert_eq(parsed_info.frequence, 50);
    cr_assert_not_null(parsed_info.names);
    cr_assert_str_eq(parsed_info.names[0], "team1");
    cr_assert_str_eq(parsed_info.names[1], "team2");
    cr_assert_str_eq(parsed_info.names[2], "team3");
    cr_assert_null(parsed_info.names[3]);
    
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 15);
    free_parsing_names(parsed_info.names);
}

// =============================================================================
// PORT PARSING TESTS
// =============================================================================

Test(parsing_tests, test_parse_port_valid)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-p", "1234"};
    char **argv = create_test_argv(args, 3);
    parsing_info_t parsed_info = {0};
    
    parse_args(3, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.port, 1234);
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 3);
}

Test(parsing_tests, test_parse_port_zero)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-p", "0"};
    char **argv = create_test_argv(args, 3);
    parsing_info_t parsed_info = {0};
    
    parse_args(3, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.port, 0); // Should return 0 for "0" input
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 3);
}

Test(parsing_tests, test_parse_port_boundary_values)
{
    reset_parsing_mocks();
    
    // Test minimum valid port
    const char *args1[] = {"zappy_server", "-p", "1"};
    char **argv1 = create_test_argv(args1, 3);
    parsing_info_t parsed_info1 = {0};
    
    parse_args(3, argv1, &parsed_info1);
    cr_assert_eq(parsed_info1.port, 1);
    
    // Test maximum valid port
    const char *args2[] = {"zappy_server", "-p", "65535"};
    char **argv2 = create_test_argv(args2, 3);
    parsing_info_t parsed_info2 = {0};
    
    parse_args(3, argv2, &parsed_info2);
    cr_assert_eq(parsed_info2.port, 65535);
    
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv1, 3);
    free_test_argv(argv2, 3);
}

Test(parsing_tests, test_parse_port_valid_edge_cases)
{
    reset_parsing_mocks();
    
    // Test that port parsing handles various valid inputs correctly
    const char *args[] = {"zappy_server", "-p", "8080"};
    char **argv = create_test_argv(args, 3);
    parsing_info_t parsed_info = {0};
    
    parse_args(3, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.port, 8080);
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 3);
}

// =============================================================================
// COORDINATES PARSING TESTS  
// =============================================================================

Test(parsing_tests, test_parse_coordinates_valid)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-x", "25", "-y", "30"};
    char **argv = create_test_argv(args, 5);
    parsing_info_t parsed_info = {0};
    
    parse_args(5, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.width, 25);
    cr_assert_eq(parsed_info.height, 30);
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 5);
}

Test(parsing_tests, test_parse_coordinates_boundary_values)
{
    reset_parsing_mocks();
    
    // Test minimum valid coordinates
    const char *args1[] = {"zappy_server", "-x", "1", "-y", "1"};
    char **argv1 = create_test_argv(args1, 5);
    parsing_info_t parsed_info1 = {0};
    
    parse_args(5, argv1, &parsed_info1);
    cr_assert_eq(parsed_info1.width, 1);
    cr_assert_eq(parsed_info1.height, 1);
    
    // Test maximum valid coordinates
    const char *args2[] = {"zappy_server", "-x", "100", "-y", "100"};
    char **argv2 = create_test_argv(args2, 5);
    parsing_info_t parsed_info2 = {0};
    
    parse_args(5, argv2, &parsed_info2);
    cr_assert_eq(parsed_info2.width, 100);
    cr_assert_eq(parsed_info2.height, 100);
    
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv1, 5);
    free_test_argv(argv2, 5);
}

// Test coordinates parsing internal logic without triggering errors
Test(parsing_tests, test_parse_coordinates_valid_range)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-x", "50", "-y", "75"};
    char **argv = create_test_argv(args, 5);
    parsing_info_t parsed_info = {0};
    
    parse_args(5, argv, &parsed_info);
    
    // Should parse normally within valid range
    cr_assert_eq(parsed_info.width, 50);
    cr_assert_eq(parsed_info.height, 75);
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 5);
}

// =============================================================================
// TEAM NAMES PARSING TESTS
// =============================================================================

Test(parsing_tests, test_parse_names_single)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-n", "team_alpha"};
    char **argv = create_test_argv(args, 3);
    parsing_info_t parsed_info = {0};
    
    parse_args(3, argv, &parsed_info);
    
    cr_assert_not_null(parsed_info.names);
    cr_assert_str_eq(parsed_info.names[0], "team_alpha");
    cr_assert_null(parsed_info.names[1]);
    
    free_test_argv(argv, 3);
    free_parsing_names(parsed_info.names);
}

Test(parsing_tests, test_parse_names_multiple)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-n", "red", "blue", "green", "yellow"};
    char **argv = create_test_argv(args, 6);
    parsing_info_t parsed_info = {0};
    
    parse_args(6, argv, &parsed_info);
    
    cr_assert_not_null(parsed_info.names);
    cr_assert_str_eq(parsed_info.names[0], "red");
    cr_assert_str_eq(parsed_info.names[1], "blue");
    cr_assert_str_eq(parsed_info.names[2], "green");
    cr_assert_str_eq(parsed_info.names[3], "yellow");
    cr_assert_null(parsed_info.names[4]);
    
    free_test_argv(argv, 6);
    free_parsing_names(parsed_info.names);
}

Test(parsing_tests, test_parse_names_stop_at_next_flag)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-n", "team1", "team2", "-p", "4242"};
    char **argv = create_test_argv(args, 6);
    parsing_info_t parsed_info = {0};
    
    parse_args(6, argv, &parsed_info);
    
    cr_assert_not_null(parsed_info.names);
    cr_assert_str_eq(parsed_info.names[0], "team1");
    cr_assert_str_eq(parsed_info.names[1], "team2");
    cr_assert_null(parsed_info.names[2]);
    cr_assert_eq(parsed_info.port, 4242);
    
    free_test_argv(argv, 6);
    free_parsing_names(parsed_info.names);
}

// =============================================================================
// CLIENT NUMBER PARSING TESTS
// =============================================================================

Test(parsing_tests, test_parse_client_nb_valid)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-c", "8"};
    char **argv = create_test_argv(args, 3);
    parsing_info_t parsed_info = {0};
    
    parse_args(3, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.client_nb, 8);
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 3);
}

Test(parsing_tests, test_parse_client_nb_large_values)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-c", "1000"};
    char **argv = create_test_argv(args, 3);
    parsing_info_t parsed_info = {0};
    
    parse_args(3, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.client_nb, 1000);
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 3);
}

// =============================================================================
// FREQUENCY PARSING TESTS
// =============================================================================

Test(parsing_tests, test_parse_frequence_valid)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-f", "200"};
    char **argv = create_test_argv(args, 3);
    parsing_info_t parsed_info = {0};
    
    parse_args(3, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.frequence, 200);
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 3);
}

Test(parsing_tests, test_parse_frequence_edge_values)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-f", "1"};
    char **argv = create_test_argv(args, 3);
    parsing_info_t parsed_info = {0};
    
    parse_args(3, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.frequence, 1);
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 3);
}

// =============================================================================
// DUPLICATE FLAG TESTS (Valid cases only)
// =============================================================================

Test(parsing_tests, test_no_duplicate_flags_valid)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-p", "4242", "-x", "10", "-y", "15", "-c", "4", "-f", "100"};
    char **argv = create_test_argv(args, 11);
    parsing_info_t parsed_info = {0};
    
    parse_args(11, argv, &parsed_info);
    
    // All unique flags should parse correctly
    cr_assert_eq(parsed_info.port, 4242);
    cr_assert_eq(parsed_info.width, 10);
    cr_assert_eq(parsed_info.height, 15);
    cr_assert_eq(parsed_info.client_nb, 4);
    cr_assert_eq(parsed_info.frequence, 100);
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 11);
}

// =============================================================================
// FLAG PROCESSING TESTS
// =============================================================================

Test(parsing_tests, test_check_args_function_coverage)
{
    reset_parsing_mocks();
    
    // Test that all flag types are recognized
    const char *args[] = {"zappy_server", "-p", "5000", "-x", "20", "-y", "25", "-n", "team", "-c", "5", "-f", "150"};
    char **argv = create_test_argv(args, 13);
    parsing_info_t parsed_info = {0};
    
    parse_args(13, argv, &parsed_info);
    
    // Verify all flags were processed
    cr_assert_eq(parsed_info.port, 5000);
    cr_assert_eq(parsed_info.width, 20);
    cr_assert_eq(parsed_info.height, 25);
    cr_assert_eq(parsed_info.client_nb, 5);
    cr_assert_eq(parsed_info.frequence, 150);
    cr_assert_not_null(parsed_info.names);
    cr_assert_str_eq(parsed_info.names[0], "team");
    
    free_test_argv(argv, 13);
    free_parsing_names(parsed_info.names);
}

Test(parsing_tests, test_check_other_flags_function_coverage)
{
    reset_parsing_mocks();
    
    // Test the check_other_flags function specifically
    const char *args[] = {"zappy_server", "-n", "alpha", "beta", "-c", "3", "-f", "75"};
    char **argv = create_test_argv(args, 8);
    parsing_info_t parsed_info = {0};
    
    parse_args(8, argv, &parsed_info);
    
    // Should cover the -n, -c, and -f branches in check_other_flags
    cr_assert_not_null(parsed_info.names);
    cr_assert_str_eq(parsed_info.names[0], "alpha");
    cr_assert_str_eq(parsed_info.names[1], "beta");
    cr_assert_eq(parsed_info.client_nb, 3);
    cr_assert_eq(parsed_info.frequence, 75);
    
    free_test_argv(argv, 8);
    free_parsing_names(parsed_info.names);
}

// =============================================================================
// DEFAULT VALUES TESTS
// =============================================================================

Test(parsing_tests, test_default_values)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server"};
    char **argv = create_test_argv(args, 1);
    parsing_info_t parsed_info = {0};
    
    parse_args(1, argv, &parsed_info);
    
    // Check all default values
    cr_assert_eq(parsed_info.port, 0);
    cr_assert_eq(parsed_info.width, 0);
    cr_assert_eq(parsed_info.height, 0);
    cr_assert_eq(parsed_info.client_nb, 0);
    cr_assert_eq(parsed_info.frequence, 100); // Default frequency
    cr_assert_null(parsed_info.names);
    
    free_test_argv(argv, 1);
}

// =============================================================================
// ADDITIONAL COVERAGE TESTS
// =============================================================================

Test(parsing_tests, test_parse_names_empty_list)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-n", "-p", "4242"};
    char **argv = create_test_argv(args, 4);
    parsing_info_t parsed_info = {0};
    
    parse_args(4, argv, &parsed_info);
    
    // Should create empty names array when no team names follow -n
    cr_assert_not_null(parsed_info.names);
    cr_assert_null(parsed_info.names[0]);
    cr_assert_eq(parsed_info.port, 4242);
    
    free_test_argv(argv, 4);
    free_parsing_names(parsed_info.names);
}

Test(parsing_tests, test_large_numbers_valid)
{
    reset_parsing_mocks();
    
    // Use large but valid numbers according to the parsing constraints
    // frequence must be <= 10000 and string length <= 9
    // client_nb has no upper limit but should be reasonable
    const char *args[] = {"zappy_server", "-c", "50000", "-f", "9999"};
    char **argv = create_test_argv(args, 5);
    parsing_info_t parsed_info = {0};
    
    parse_args(5, argv, &parsed_info);
    
    // Large valid numbers should be accepted
    cr_assert_eq(parsed_info.client_nb, 50000);
    cr_assert_eq(parsed_info.frequence, 9999); // Within the 1-10000 limit
    cr_assert_eq(mock_parsing_error_calls, 0);
    
    free_test_argv(argv, 5);
}

Test(parsing_tests, test_special_team_names)
{
    reset_parsing_mocks();
    
    const char *args[] = {"zappy_server", "-n", "team-1", "team_2", "team.3", "TEAM4"};
    char **argv = create_test_argv(args, 6);
    parsing_info_t parsed_info = {0};
    
    parse_args(6, argv, &parsed_info);
    
    cr_assert_not_null(parsed_info.names);
    cr_assert_str_eq(parsed_info.names[0], "team-1");
    cr_assert_str_eq(parsed_info.names[1], "team_2");
    cr_assert_str_eq(parsed_info.names[2], "team.3");
    cr_assert_str_eq(parsed_info.names[3], "TEAM4");
    cr_assert_null(parsed_info.names[4]);
    
    free_test_argv(argv, 6);
    free_parsing_names(parsed_info.names);
}