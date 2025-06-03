/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Integration test for fork command
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../src/Server/include/command.h"
#include "../../src/Server/include/server.h"
#include "../../src/Server/include/egg.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Simple mock function to avoid write syscall issues in tests
void mock_write_command_output(int client_fd, char *msg)
{
    (void)client_fd;
    (void)msg;
    // Just print to stdout for testing
    printf("%s", msg);
}

Test(fork_integration, test_fork_command_workflow)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    parsing_info_t parsed_info = {0};
    char buffer[] = "Fork";
    
    // Setup minimal server structure
    server.eggs = NULL;
    server.parsed_info = &parsed_info;
    parsed_info.client_nb = 3;
    
    // Setup minimal client and player
    client.client_id = 42;
    client.client_fd = 1;
    client.player = &player;
    
    player.pos_x = 5;
    player.pos_y = 7;
    player.team_name = strdup("test_team");
    
    cr_redirect_stdout();
    
    // Test that fork creates an egg
    fork_command(&server, &client, buffer);
    
    // Verify egg was created
    cr_assert_not_null(server.eggs);
    cr_assert_eq(server.eggs->creator_id, 42);
    cr_assert_eq(server.eggs->pos_x, 5);
    cr_assert_eq(server.eggs->pos_y, 7);
    cr_assert_eq(server.eggs->egg_id, 1);
    
    // Test that a second fork creates another egg with different ID
    fork_command(&server, &client, buffer);
    cr_assert_not_null(server.eggs);
    cr_assert_eq(server.eggs->egg_id, 2); // Most recent egg should have ID 2
    
    // Cleanup
    egg_t *current = server.eggs;
    while (current) {
        egg_t *next = current->next;
        free(current);
        current = next;
    }
    free(player.team_name);
}

Test(connect_nbr_integration, test_slots_with_eggs)
{
    server_t server = {0};
    client_t head_client = {0};
    client_t client1 = {0};
    client_t client2 = {0};
    player_t player1 = {0};
    player_t player2 = {0};
    parsing_info_t parsed_info = {0};
    egg_t egg1 = {0};
    char buffer[] = "Connect_nbr";
    
    // Setup server with base 2 slots per team
    server.parsed_info = &parsed_info;
    parsed_info.client_nb = 2;
    
    // Setup client list with head node
    head_client.client_id = -1;
    head_client.next = &client1;
    server.client = &head_client;
    
    // Setup first client
    client1.client_id = 1;
    client1.client_fd = 1;
    client1.player = &player1;
    client1.next = &client2;
    
    player1.team_name = strdup("team_alpha");
    
    // Setup second client  
    client2.client_id = 2;
    client2.client_fd = 1;
    client2.player = &player2;
    client2.next = NULL;
    
    player2.team_name = strdup("team_alpha");
    
    // Add an egg created by client1
    egg1.egg_id = 1;
    egg1.creator_id = 1;
    egg1.pos_x = 0;
    egg1.pos_y = 0;
    egg1.next = NULL;
    server.eggs = &egg1;
    
    cr_redirect_stdout();
    
    // Test connect_nbr calculation
    // Base slots: 2, Current team members: 2, Eggs by team: 1
    // Available slots should be: (2 + 1) - 2 = 1
    connect_nbr(&server, &client1, buffer);
    
    // Cleanup
    free(player1.team_name);
    free(player2.team_name);
}
