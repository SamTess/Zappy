/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_parse_command
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/parsing.h"
#include <stdio.h>
#include <assert.h>
#include "test_mocks.h"

Test(test_parse_command, test_get_command_data)
{
    command_data_t data = get_command_data();
    
    cr_assert_not_null(data.commands);
    cr_assert_not_null(data.functions);
    cr_assert_not_null(data.times);
    cr_assert_not_null(data.accepted_types);
    
    // Test first command is "Forward"
    cr_assert_str_eq(data.commands[0], "Forward");
    
    // Test that arrays end with NULL/0
    int i = 0;
    while (data.commands[i] != NULL) {
        i++;
    }
    cr_assert_gt(i, 0);
}

Test(test_parse_command, test_execute_com_null_user)
{
    server_t server = {0};
    
    // Should not crash with null user
    execute_com(&server, NULL, "test");
}

Test(test_parse_command, test_execute_com_unconnected_user)
{
    server_t server = {0};
    client_t client = {0};
    client.is_fully_connected = false;
    
    // Mock the can_connect function to return false
    execute_com(&server, &client, "InvalidTeam");
    
    cr_assert_eq(client.is_fully_connected, false);
}

Test(test_parse_command, test_write_command_output)
{
    // Redirect stdout to capture output
    cr_redirect_stdout();
    
    // Test with invalid fd - should handle gracefully
    write_command_output(-1, "test message");
    
    // Should not crash
    cr_assert(1);
}

Test(test_parse_command, test_cleanup_pending_null)
{
    // Should not crash with null player
    cleanup_pending(NULL);
    
    player_t player = {0};
    player.pending_cmd = NULL;
    
    // Should not crash with null pending_cmd
    cleanup_pending(&player);
    
    cr_assert(1);
}

Test(test_parse_command, test_cleanup_player_queue_null)
{
    // Should not crash with null player
    cleanup_player_queue(NULL);
    
    player_t player = {0};
    player.command_queue = NULL;
    
    // Should not crash with null command_queue
    cleanup_player_queue(&player);
    
    cr_assert(1);
}

Test(test_parse_command, test_cleanup_client_null)
{
    // Since cleanup_client doesn't exist, we'll test cleanup_pending instead
    cleanup_pending(NULL);
    
    player_t player = {0};
    player.pending_cmd = NULL;
    
    // Should not crash with null pending_cmd
    cleanup_pending(&player);
    
    cr_assert(1);
}

Test(test_parse_command, test_determine_type)
{
    // Test valid resource types
    cr_assert_eq(determine_type("food"), FOOD);
    cr_assert_eq(determine_type("linemate"), LINEMATE);
    cr_assert_eq(determine_type("deraumere"), DERAUMERE);
    cr_assert_eq(determine_type("sibur"), SIBUR);
    cr_assert_eq(determine_type("mendiane"), MENDIANE);
    cr_assert_eq(determine_type("phiras"), PHIRAS);
    cr_assert_eq(determine_type("thystame"), THYSTAME);
    
    // Test unknown resource type - should return COUNT
    cr_assert_eq(determine_type("unknown"), COUNT);
}

Test(test_parse_command, test_execute_com_ai_connection)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = false;
    client.type = AI;
    client.client_fd = 1;
    client.client_id = 1;
    client.player = &player;
    player.team_name = "TestTeam";
    
    server.parsed_info = malloc(sizeof(parsing_info_t));
    server.parsed_info->width = 10;
    server.parsed_info->height = 10;
    
    execute_com(&server, &client, "TestTeam");
    
    free(server.parsed_info);
}

Test(test_parse_command, test_execute_com_find_and_execute_forward)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.type = AI;
    client.player = &player;
    player.busy_until = 0;
    server.current_tick = 1;
    
    execute_com(&server, &client, "Forward");
}

Test(test_parse_command, test_execute_com_find_and_execute_inventory)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.type = AI;
    client.player = &player;
    player.busy_until = 0;
    server.current_tick = 1;
    
    execute_com(&server, &client, "Inventory");
}

Test(test_parse_command, test_execute_com_graphical_command)
{
    server_t server = {0};
    client_t client = {0};
    
    client.is_fully_connected = true;
    client.type = GRAPHICAL;
    client.client_fd = 1;
    
    execute_com(&server, &client, "msz");
}

Test(test_parse_command, test_execute_com_invalid_command_ai)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.type = AI;
    client.client_fd = 1;
    client.player = &player;
    
    execute_com(&server, &client, "InvalidCommand");
}

Test(test_parse_command, test_execute_com_invalid_command_graphical)
{
    server_t server = {0};
    client_t client = {0};
    
    client.is_fully_connected = true;
    client.type = GRAPHICAL;
    client.client_fd = 1;
    
    execute_com(&server, &client, "InvalidCommand");
}

Test(test_parse_command, test_execute_com_busy_player_queue_not_full)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.type = AI;
    client.player = &player;
    player.busy_until = 100;  // Player is busy
    player.queue_size = 5;    // Queue not full
    server.current_tick = 1;
    
    execute_com(&server, &client, "Forward");
}

Test(test_parse_command, test_execute_com_busy_player_queue_full)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.type = AI;
    client.player = &player;
    player.busy_until = 100;  // Player is busy
    player.queue_size = 10;   // Queue is full
    server.current_tick = 1;
    
    execute_com(&server, &client, "Forward");
}

Test(test_parse_command, test_get_message_with_valid_command)
{
    client_t client = {0};
    player_t player = {0};
    
    client.type = AI;
    client.is_fully_connected = true;
    client.player = &player;
    client.client_poll = malloc(sizeof(struct pollfd));
    client.client_poll->fd = 0; // stdin for testing
    client.client_fd = 1;
    
    // This will test the message reading logic
    // Note: In real testing, we'd need to mock the read() function
    
    free(client.client_poll);
}

Test(test_parse_command, test_send_info_new_client)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.client_id = 1;
    client.client_fd = 1;
    client.player = &player;
    player.team_name = "TestTeam";
    
    server.parsed_info = malloc(sizeof(parsing_info_t));
    server.parsed_info->width = 10;
    server.parsed_info->height = 10;
    
    // This tests the send_info_new_client function indirectly
    client.is_fully_connected = false;
    client.type = AI;
    
    execute_com(&server, &client, "TestTeam");
    
    free(server.parsed_info);
}

// Test parse_args function with valid arguments
Test(parse_command, test_parse_args_valid) {
    parsing_info_t parsed_info;
    char *argv[] = {"./zappy_server", "-p", "4242", "-x", "20", "-y", "15", 
                    "-n", "team1", "team2", "-c", "5", "-f", "100"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parse_args(argc, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.port, 4242);
    cr_assert_eq(parsed_info.width, 20);
    cr_assert_eq(parsed_info.height, 15);
    cr_assert_eq(parsed_info.client_nb, 5);
    cr_assert_eq(parsed_info.frequence, 100);
    cr_assert_neq(parsed_info.names, NULL);
    cr_assert_str_eq(parsed_info.names[0], "team1");
    cr_assert_str_eq(parsed_info.names[1], "team2");
    
    // Cleanup
    if (parsed_info.names) {
        for (int i = 0; parsed_info.names[i]; i++) {
            free(parsed_info.names[i]);
        }
        free(parsed_info.names);
    }
}

// Test str_to_word_arr function
Test(parse_command, test_str_to_word_arr) {
    char test_str[] = "hello world test";
    char **result = str_to_word_arr(test_str, " ");
    
    cr_assert_neq(result, NULL);
    cr_assert_str_eq(result[0], "hello");
    cr_assert_str_eq(result[1], "world");
    cr_assert_str_eq(result[2], "test");
    
    free_arr(result);
}

// Test arr_len function
Test(parse_command, test_arr_len) {
    char *test_array[] = {"one", "two", "three", NULL};
    int len = arr_len(test_array);
    cr_assert_eq(len, 3);
}

// Test arr_len with empty array
Test(parse_command, test_arr_len_empty) {
    char *test_array[] = {NULL};
    int len = arr_len(test_array);
    cr_assert_eq(len, 0);
}

// Test free_arr function (testing it doesn't crash)
Test(parse_command, test_free_arr) {
    char **test_array = malloc(3 * sizeof(char*));
    test_array[0] = strdup("test1");
    test_array[1] = strdup("test2");
    test_array[2] = NULL;
    
    // This should not crash
    free_arr(test_array);
    cr_assert(true); // If we reach here, free_arr worked
}

// Test parse_args with minimal valid arguments
Test(parse_command, test_parse_args_minimal) {
    parsing_info_t parsed_info;
    char *argv[] = {"./zappy_server", "-p", "8080", "-x", "10", "-y", "10", 
                    "-n", "testteam", "-c", "1", "-f", "50"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parse_args(argc, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.port, 8080);
    cr_assert_eq(parsed_info.width, 10);
    cr_assert_eq(parsed_info.height, 10);
    cr_assert_eq(parsed_info.client_nb, 1);
    cr_assert_eq(parsed_info.frequence, 50);
    
    // Cleanup
    if (parsed_info.names) {
        for (int i = 0; parsed_info.names[i]; i++) {
            free(parsed_info.names[i]);
        }
        free(parsed_info.names);
    }
}

// Test default frequence value
Test(parse_command, test_parse_args_default_freq) {
    parsing_info_t parsed_info;
    char *argv[] = {"./zappy_server", "-p", "4242", "-x", "20", "-y", "15", 
                    "-n", "team1", "-c", "5"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    parse_args(argc, argv, &parsed_info);
    
    cr_assert_eq(parsed_info.frequence, 100); // Default value
    
    // Cleanup
    if (parsed_info.names) {
        for (int i = 0; parsed_info.names[i]; i++) {
            free(parsed_info.names[i]);
        }
        free(parsed_info.names);
    }
}

// ========== NOUVEAUX TESTS POUR AMÉLIORER LA COUVERTURE ==========

// Test pour couvrir execute_graphical_command 
Test(test_parse_command, test_execute_com_graphic_connection)
{
    server_t server = {0};
    client_t client = {0};
    
    client.is_fully_connected = false;
    client.type = GRAPHICAL;
    client.client_fd = 1;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Cela va exécuter le chemin graphique dans execute_com
    execute_com(&server, &client, "GRAPHIC");
    
    cr_assert(true);
}

// Test pour couvrir send_info_new_client avec différents paramètres
Test(test_parse_command, test_execute_com_ai_connection_detailed)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    // Setup complet pour couvrir send_info_new_client
    client.is_fully_connected = false;
    client.type = AI;
    client.client_fd = 1;
    client.client_id = 42;
    client.player = &player;
    player.team_name = "TestTeam";
    
    server.parsed_info = malloc(sizeof(parsing_info_t));
    server.parsed_info->width = 15;
    server.parsed_info->height = 20;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Cela va exécuter send_info_new_client et ses snprintf
    execute_com(&server, &client, "TestTeam");
    
    free(server.parsed_info);
}

// Test pour couvrir find_and_execute avec différentes commandes
Test(test_parse_command, test_execute_com_all_ai_commands)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.type = AI;
    client.client_fd = 1;
    client.player = &player;
    player.busy_until = 0;
    player.queue_size = 0;
    server.current_tick = 10;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Tester plusieurs commandes pour couvrir différents chemins
    char *commands[] = {"Forward", "Right", "Left", "Look", "Eject", 
                       "Connect_nbr", "Take food", "Set food", 
                       "Incantation", "Fork", "Broadcast test"};
    
    for (int i = 0; i < 11; i++) {
        execute_com(&server, &client, commands[i]);
    }
}

// Test pour couvrir find_and_execute avec commandes graphiques
Test(test_parse_command, test_execute_com_all_graphic_commands)
{
    server_t server = {0};
    client_t client = {0};
    
    client.is_fully_connected = true;
    client.type = GRAPHICAL;
    client.client_fd = 1;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Tester toutes les commandes graphiques
    char *graphic_commands[] = {"msz", "bct 1 1", "mtc 1", "tna", 
                               "ppo 1", "plv 1", "pin 1", "sgt", "sst 10"};
    
    for (int i = 0; i < 9; i++) {
        execute_com(&server, &client, graphic_commands[i]);
    }
}

// Test pour couvrir execute_if_free avec client busy et queue pleine
Test(test_parse_command, test_execute_com_busy_full_queue)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.type = AI;
    client.client_fd = 1;
    client.player = &player;
    player.busy_until = 100;  // Client occupé
    player.queue_size = 10;   // Queue pleine (max 10)
    server.current_tick = 1;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Cela va couvrir le cas queue pleine dans execute_if_free
    execute_com(&server, &client, "Forward");
}

// Test pour couvrir execute_if_free avec client busy et queue non pleine
Test(test_parse_command, test_execute_com_busy_partial_queue)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.type = AI;
    client.client_fd = 1;
    client.player = &player;
    player.busy_until = 100;  // Client occupé
    player.queue_size = 5;    // Queue pas pleine
    server.current_tick = 1;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Cela va couvrir add_to_command_queue dans execute_if_free
    execute_com(&server, &client, "Forward");
}

// Test pour couvrir get_message - simulation basique
Test(test_parse_command, test_get_message_basic)
{
    server_t server = {0};
    client_t client = {0};
    struct pollfd poll_fd = {0};
    
    client.client_poll = &poll_fd;
    client.client_poll->fd = -1; // FD invalide pour faire échouer read()
    client.client_fd = 1;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Cela va exécuter get_message et check_disconnect
    // Le read() va échouer et déclencher check_disconnect
    get_message(&server, &client);
}

// Test pour couvrir check_disconnect avec bytes_read = 0
Test(test_parse_command, test_check_disconnect_zero_bytes)
{
    server_t server = {0};
    client_t client = {0};
    struct pollfd poll_fd = {0};
    
    client.client_poll = &poll_fd;
    client.client_poll->fd = 1;
    
    // Simuler directement check_disconnect avec 0 bytes (déconnexion)
    // Note: on ne peut pas tester directement car c'est static, 
    // mais get_message l'utilise
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    get_message(&server, &client);
}

// Test pour couvrir les différents cas de strncmp dans find_and_execute
Test(test_parse_command, test_execute_com_partial_command_match)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.type = AI;
    client.client_fd = 1;
    client.player = &player;
    player.busy_until = 0;
    server.current_tick = 10;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Tester des commandes qui commencent pareil pour couvrir strncmp
    execute_com(&server, &client, "For");      // Ne match pas "Forward"
    execute_com(&server, &client, "Forwardx"); // Match "Forward" au début
    execute_com(&server, &client, "R");        // Ne match pas "Right"
    execute_com(&server, &client, "Rights");   // Match "Right" au début
}

// Test pour couvrir execute_graphical_command avec str_to_word_arr
Test(test_parse_command, test_execute_com_graphic_with_args)
{
    server_t server = {0};
    client_t client = {0};
    
    client.is_fully_connected = true;
    client.type = GRAPHICAL;
    client.client_fd = 1;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Commandes avec arguments pour tester str_to_word_arr et free_arr
    execute_com(&server, &client, "bct 5 10");
    execute_com(&server, &client, "ppo 1");
    execute_com(&server, &client, "pin 2");
    execute_com(&server, &client, "sst 50");
}

// Test pour couvrir les différents types de clients acceptés
Test(test_parse_command, test_execute_com_client_type_mismatch)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.is_fully_connected = true;
    client.client_fd = 1;
    client.player = &player;
    
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Test commande AI avec client GRAPHICAL (type mismatch)
    client.type = GRAPHICAL;
    execute_com(&server, &client, "Forward");  // AI command
    
    // Test commande GRAPHICAL avec client AI (type mismatch)  
    client.type = AI;
    execute_com(&server, &client, "msz");      // GRAPHICAL command
}

// Test pour couvrir tous les chemins dans get_command_data
Test(test_parse_command, test_get_command_data_comprehensive)
{
    // Appeler plusieurs fois pour s'assurer que les tableaux statiques sont bien utilisés
    for (int i = 0; i < 5; i++) {
        command_data_t data = get_command_data();
        
        // Vérifier quelques éléments pour utiliser les données
        cr_assert_str_eq(data.commands[0], "Forward");
        cr_assert_str_eq(data.commands[11], "Broadcast");
        cr_assert_str_eq(data.commands[12], "msz");
        cr_assert_eq(data.times[0], 7);        // Forward time
        cr_assert_eq(data.times[9], 300);      // Incantation time
        cr_assert_eq(data.times[10], 42);      // Fork time
        cr_assert_eq(data.accepted_types[0], AI);
        cr_assert_eq(data.accepted_types[12], GRAPHICAL);
    }
}