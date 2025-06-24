/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_parse_command_utils - Tests for parse command utilities
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "test_mocks.h"

// Déclarations externes pour les mocks
extern bool __wrap_can_connect_return;
extern int __wrap_write_command_output_calls;
extern char *__wrap_write_command_output_last_msg;
extern bool __wrap_send_pnw_command_to_all_called;

// Tests pour check_team_name
Test(parse_command_utils_tests, test_check_team_name_valid)
{
    server_t server = {0};
    parsing_info_t parsed_info = {0};
    
    server.parsed_info = &parsed_info;
    server.parsed_info->names = malloc(sizeof(char*) * 3);
    server.parsed_info->names[0] = strdup("team1");
    server.parsed_info->names[1] = strdup("team2");
    server.parsed_info->names[2] = NULL;
    
    bool result = check_team_name(&server, "team1");
    cr_assert_eq(result, true);
    
    result = check_team_name(&server, "team2");
    cr_assert_eq(result, true);
    
    result = check_team_name(&server, "invalid_team");
    cr_assert_eq(result, false);
    
    free(server.parsed_info->names[0]);
    free(server.parsed_info->names[1]);
    free(server.parsed_info->names);
}

Test(parse_command_utils_tests, test_check_team_name_null_server)
{
    bool result = check_team_name(NULL, "team1");
    cr_assert_eq(result, false);
}

Test(parse_command_utils_tests, test_check_team_name_null_team)
{
    server_t server = {0};
    parsing_info_t parsed_info = {0};
    
    server.parsed_info = &parsed_info;
    server.parsed_info->names = malloc(sizeof(char*) * 2);
    server.parsed_info->names[0] = strdup("team1");
    server.parsed_info->names[1] = NULL;
    
    bool result = check_team_name(&server, NULL);
    cr_assert_eq(result, false);
    
    free(server.parsed_info->names[0]);
    free(server.parsed_info->names);
}

Test(parse_command_utils_tests, test_check_team_name_empty_team)
{
    server_t server = {0};
    parsing_info_t parsed_info = {0};
    
    server.parsed_info = &parsed_info;
    server.parsed_info->names = malloc(sizeof(char*) * 2);
    server.parsed_info->names[0] = strdup("team1");
    server.parsed_info->names[1] = NULL;
    
    bool result = check_team_name(&server, "");
    cr_assert_eq(result, false);
    
    free(server.parsed_info->names[0]);
    free(server.parsed_info->names);
}

// Tests pour can_connect
Test(parse_command_utils_tests, test_can_connect_valid_team)
{
    server_t server = {0};
    client_t client = {0};
    parsing_info_t parsed_info = {0};
    
    server.parsed_info = &parsed_info;
    server.parsed_info->names = malloc(sizeof(char*) * 2);
    server.parsed_info->names[0] = strdup("team1");
    server.parsed_info->names[1] = NULL;
    server.parsed_info->client_nb = 10;
    
    client.client_fd = 1;
    client.client_id = 1;
    client.type = AI;
    
    __wrap_write_command_output_calls = 0;
    
    bool result = can_connect(&server, &client, "team1\n");
    
    cr_assert_eq(result, true);
    cr_assert_str_eq(client.team_name, "team1");
    cr_assert_eq(__wrap_write_command_output_calls, 2); // connect_nbr + map size
    
    free(server.parsed_info->names[0]);
    free(server.parsed_info->names);
}

Test(parse_command_utils_tests, test_can_connect_invalid_team)
{
    server_t server = {0};
    client_t client = {0};
    parsing_info_t parsed_info = {0};
    
    server.parsed_info = &parsed_info;
    server.parsed_info->names = malloc(sizeof(char*) * 2);
    server.parsed_info->names[0] = strdup("team1");
    server.parsed_info->names[1] = NULL;
    
    client.client_fd = 1;
    
    __wrap_write_command_output_calls = 0;
    
    bool result = can_connect(&server, &client, "invalid_team\n");
    
    cr_assert_eq(result, false);
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ko\n");
    
    free(server.parsed_info->names[0]);
    free(server.parsed_info->names);
}

Test(parse_command_utils_tests, test_can_connect_graphical)
{
    server_t server = {0};
    client_t client = {0};
    
    client.client_fd = 1;
    client.type = GRAPHICAL;
    
    __wrap_write_command_output_calls = 0;
    
    bool result = can_connect(&server, &client, "GRAPHIC\n");
    
    cr_assert_eq(result, true);
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "WELCOME\n");
}

Test(parse_command_utils_tests, test_can_connect_null_parameters)
{
    server_t server = {0};
    client_t client = {0};
    
    bool result = can_connect(NULL, &client, "team1\n");
    cr_assert_eq(result, false);
    
    result = can_connect(&server, NULL, "team1\n");
    cr_assert_eq(result, false);
    
    result = can_connect(&server, &client, NULL);
    cr_assert_eq(result, false);
}

// Tests pour send_connection_info
Test(parse_command_utils_tests, test_send_connection_info_ai)
{
    server_t server = {0};
    client_t client = {0};
    parsing_info_t parsed_info = {0};
    
    server.parsed_info = &parsed_info;
    server.parsed_info->client_nb = 5;
    server.parsed_info->width = 10;
    server.parsed_info->height = 8;
    
    client.client_fd = 1;
    client.type = AI;
    
    __wrap_write_command_output_calls = 0;
    
    send_connection_info(&server, &client);
    
    cr_assert_eq(__wrap_write_command_output_calls, 2);
    // Vérifier que les messages contiennent les bonnes valeurs
}

Test(parse_command_utils_tests, test_send_connection_info_graphical)
{
    server_t server = {0};
    client_t client = {0};
    
    client.client_fd = 1;
    client.type = GRAPHICAL;
    
    __wrap_write_command_output_calls = 0;
    
    send_connection_info(&server, &client);
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "WELCOME\n");
}

// Tests pour send_pnw_command_to_all
Test(parse_command_utils_tests, test_send_pnw_command_to_all)
{
    server_t server = {0};
    client_t client = {0};
    client_t *clients[3];
    client_t client1 = {0}, client2 = {0};
    
    // Setup clients
    client.client_id = 1;
    client.type = AI;
    client1.client_id = 2;
    client1.type = GRAPHICAL;
    client1.is_fully_connected = true;
    client2.client_id = 3;
    client2.type = AI;
    client2.is_fully_connected = true;
    
    clients[0] = &client1;
    clients[1] = &client2;
    clients[2] = NULL;
    
    server.clients = clients;
    
    __wrap_send_pnw_command_to_all_called = false;
    
    send_pnw_command_to_all(&server, &client);
    
    cr_assert(__wrap_send_pnw_command_to_all_called);
}

// Tests pour add_to_command_queue
Test(parse_command_utils_tests, test_add_to_command_queue_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.player = &player;
    player.queue_size = 2;
    player.command_queue = malloc(sizeof(char*) * 10);
    for (int i = 0; i < 10; i++) {
        player.command_queue[i] = NULL;
    }
    
    add_to_command_queue(&server, &client, "Forward");
    
    cr_assert_eq(player.queue_size, 3);
    cr_assert_str_eq(player.command_queue[2], "Forward");
    
    // Cleanup
    for (int i = 0; i < 10; i++) {
        if (player.command_queue[i]) {
            free(player.command_queue[i]);
        }
    }
    free(player.command_queue);
}

Test(parse_command_utils_tests, test_add_to_command_queue_full)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.player = &player;
    player.queue_size = 10; // Queue pleine
    player.command_queue = malloc(sizeof(char*) * 10);
    
    add_to_command_queue(&server, &client, "Forward");
    
    // La queue reste pleine, pas d'ajout
    cr_assert_eq(player.queue_size, 10);
    
    free(player.command_queue);
}

Test(parse_command_utils_tests, test_add_to_command_queue_null_parameters)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.player = &player;
    player.queue_size = 0;
    
    // Test avec paramètres NULL
    add_to_command_queue(NULL, &client, "Forward");
    add_to_command_queue(&server, NULL, "Forward");
    add_to_command_queue(&server, &client, NULL);
    
    // Aucun crash attendu
    cr_assert_eq(player.queue_size, 0);
}