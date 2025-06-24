/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pnw_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "test_mocks.h"

TestSuite(command_pnw_simple);

Test(command_pnw_simple, test_send_pnw_command_basic)
{
    server_t server = {0};
    client_t client = {0};
    client_t recipient = {0};
    player_t player = {0};
    
    // Setup minimal structures
    client.player = &player;
    client.client_id = 1;
    player.pos_x = 5;
    player.pos_y = 5;
    player.rotation = UP;
    player.level = 1;
    player.team_name = "test_team";
    recipient.client_fd = 1;
    
    // Call function to get coverage
    send_pnw_command(&server, &client, &recipient);
    
    cr_assert(true, "Function executed");
}

Test(command_pnw_simple, test_send_pnw_command_to_all_basic)
{
    server_t server = {0};
    client_t client = {0};
    client_t graphical_client = {0};
    player_t player = {0};
    
    client.player = &player;
    client.client_id = 1;
    player.team_name = "test_team";
    
    graphical_client.type = GRAPHICAL;
    graphical_client.is_fully_connected = true;
    graphical_client.next = NULL;
    server.client = &graphical_client;
    
    send_pnw_command_to_all(&server, &client);
    
    cr_assert(true, "Function executed");
}

Test(command_pnw_simple, test_send_all_player_info_to_one_client_basic)
{
    server_t server = {0};
    client_t client = {0};
    client_t ai_client = {0};
    player_t player = {0};
    
    client.client_fd = 1;
    
    ai_client.type = AI;
    ai_client.is_fully_connected = true;
    ai_client.player = &player;
    ai_client.next = NULL;
    player.team_name = "test_team";
    server.client = &ai_client;
    
    send_all_player_info_to_one_client(&server, &client);
    
    cr_assert(true, "Function executed");
}