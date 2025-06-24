#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"

Test(client_node, free_node_with_graphical_client)
{
    server_t *server = malloc(sizeof(server_t));
    client_t *node = malloc(sizeof(client_t));
    
    memset(server, 0, sizeof(server_t));
    memset(node, 0, sizeof(client_t));
    
    node->type = GRAPHICAL;
    node->client_poll = malloc(sizeof(int));
    node->client_add = malloc(sizeof(char));
    
    free_node(node, server);
    free(server);
}

Test(client_node, free_node_with_player_client)
{
    server_t *server = malloc(sizeof(server_t));
    client_t *node = malloc(sizeof(client_t));
    player_t *player = malloc(sizeof(player_t));
    
    memset(server, 0, sizeof(server_t));
    memset(node, 0, sizeof(client_t));
    memset(player, 0, sizeof(player_t));
    
    // Setup player
    player->team_name = strdup("test_team");
    player->command_queue = malloc(sizeof(char*) * 10);
    player->queue_size = 0;
    
    // Setup pending command
    player->pending_cmd = malloc(sizeof(pending_cmd_t));
    player->pending_cmd->args = malloc(sizeof(char*) * 5);
    
    node->type = AI;
    node->player = player;
    node->client_poll = malloc(sizeof(int));
    
    free_node(node, server);
    free(server);
}

Test(client_node, free_node_minimal)
{
    server_t *server = malloc(sizeof(server_t));
    client_t *node = malloc(sizeof(client_t));
    
    memset(server, 0, sizeof(server_t));
    memset(node, 0, sizeof(client_t));
    
    node->type = AI;
    
    free_node(node, server);
    free(server);
}