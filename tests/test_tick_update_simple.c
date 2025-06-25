/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_tick_update_simple - Tests simples pour update_game_tick
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/tile.h"
#include "../src/Server/include/pending_cmd_utils.h"
#include <stdlib.h>
#include <string.h>

TestSuite(tick_update_simple);

// Variables externes définies dans test_mocks.c
extern int mock_write_calls;
extern char *last_message;

// Helper pour reset les mocks
static void reset_mocks(void)
{
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// Helper pour créer un serveur de test complet
static server_t *create_test_server_complete(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    
    info->width = 10;
    info->height = 10;
    info->frequence = 100;
    
    server->parsed_info = info;
    server->current_tick = 0;
    
    // Créer la map
    server->map = calloc(info->height, sizeof(tile_t *));
    for (int i = 0; i < info->height; i++) {
        server->map[i] = calloc(info->width, sizeof(tile_t));
        for (int j = 0; j < info->width; j++) {
            tile_init(&server->map[i][j]);
        }
    }
    
    // Initialiser les ressources
    server->total_resources = calloc(COUNT, sizeof(int));
    server->current_resources = calloc(COUNT, sizeof(int));
    for (int i = 0; i < COUNT; i++) {
        server->total_resources[i] = 100;
        server->current_resources[i] = 50;
    }
    
    // Créer un client dummy en tête de liste
    client_t *dummy = calloc(1, sizeof(client_t));
    dummy->client_fd = -1;
    dummy->client_id = -1;
    dummy->type = AI;
    dummy->next = NULL;
    server->client = dummy;
    
    return server;
}

// Helper pour créer un client AI de test
static client_t *create_test_ai_client(int client_id)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = client_id;
    client->client_fd = 10;
    client->player = player;
    client->type = AI;
    client->is_fully_connected = true;
    client->next = NULL;
    
    // Initialiser le player
    player->pos_x = 0;
    player->pos_y = 0;
    player->level = 1;
    player->life = 126;
    player->busy_until = 0;
    player->queue_size = 0;
    player->is_in_incantation = false;
    player->incantation_leader_id = -1;
    
    // Initialiser les commandes
    player->command_queue = calloc(10, sizeof(char *));
    player->pending_cmd = calloc(1, sizeof(pending_cmd_t));
    player->pending_cmd->func = NULL;
    player->pending_cmd->args = NULL;
    
    // Initialiser l'inventaire
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = 0;
    }
    player->inventory[FOOD] = 10; // Commencer avec de la nourriture
    
    return client;
}

// Helper pour libérer le serveur de test
static void free_test_server_complete(server_t *server)
{
    if (server) {
        if (server->map) {
            for (int i = 0; i < server->parsed_info->height; i++) {
                for (int j = 0; j < server->parsed_info->width; j++) {
                    tile_free(&server->map[i][j]);
                }
                free(server->map[i]);
            }
            free(server->map);
        }
        
        // Libérer les clients
        client_t *current = server->client;
        while (current) {
            client_t *next = current->next;
            if (current->player) {
                if (current->player->command_queue) {
                    for (int i = 0; i < current->player->queue_size; i++) {
                        free(current->player->command_queue[i]);
                    }
                    free(current->player->command_queue);
                }
                free(current->player->pending_cmd);
                free(current->player);
            }
            free(current);
            current = next;
        }
        
        free(server->total_resources);
        free(server->current_resources);
        free(server->parsed_info);
        free(server);
    }
}

// Helper pour libérer un client
static void free_test_client(client_t *client)
{
    if (client) {
        if (client->player) {
            if (client->player->command_queue) {
                for (int i = 0; i < client->player->queue_size; i++) {
                    free(client->player->command_queue[i]);
                }
                free(client->player->command_queue);
            }
            free(client->player->pending_cmd);
            free(client->player);
        }
        free(client);
    }
}

// =============================================================================
// TESTS pour update_game_tick
// =============================================================================

Test(tick_update_simple, test_update_game_tick_basic)
{
    reset_mocks();
    
    server_t *server = create_test_server_complete();
    
    // Test basique : incrémenter le tick
    int initial_tick = server->current_tick;
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, initial_tick + 1);
    
    free_test_server_complete(server);
}

Test(tick_update_simple, test_update_game_tick_with_client)
{
    reset_mocks();
    
    server_t *server = create_test_server_complete();
    client_t *client = create_test_ai_client(1);
    
    // Ajouter le client à la liste
    client->next = server->client->next;
    server->client->next = client;
    
    int initial_tick = server->current_tick;
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, initial_tick + 1);
    
    free_test_server_complete(server);
}

Test(tick_update_simple, test_update_game_tick_resource_respawn)
{
    reset_mocks();
    
    server_t *server = create_test_server_complete();
    
    // Mettre le tick à 19 pour que le prochain soit 20 (divisible par 20)
    server->current_tick = 19;
    
    update_game_tick(server);
    
    // Vérifier que le tick a été incrémenté à 20
    cr_assert_eq(server->current_tick, 20);
    
    free_test_server_complete(server);
}

Test(tick_update_simple, test_update_game_tick_with_pending_command)
{
    reset_mocks();
    
    server_t *server = create_test_server_complete();
    client_t *client = create_test_ai_client(1);
    
    // Ajouter le client à la liste
    client->next = server->client->next;
    server->client->next = client;
    
    // Simuler une commande en attente
    client->player->pending_cmd->args = strdup("Forward");
    client->player->busy_until = server->current_tick; // Prêt à être exécuté
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    free_test_server_complete(server);
}

Test(tick_update_simple, test_update_game_tick_with_queued_commands)
{
    reset_mocks();
    
    server_t *server = create_test_server_complete();
    client_t *client = create_test_ai_client(1);
    
    // Ajouter le client à la liste
    client->next = server->client->next;
    server->client->next = client;
    
    // Ajouter des commandes en file
    client->player->command_queue[0] = strdup("Forward");
    client->player->queue_size = 1;
    client->player->busy_until = server->current_tick; // Prêt
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    free_test_server_complete(server);
}

Test(tick_update_simple, test_update_game_tick_incantation_in_progress)
{
    reset_mocks();
    
    server_t *server = create_test_server_complete();
    client_t *client = create_test_ai_client(1);
    
    // Ajouter le client à la liste
    client->next = server->client->next;
    server->client->next = client;
    
    // Simuler une incantation en cours
    client->player->is_in_incantation = true;
    client->player->busy_until = server->current_tick; // Prêt à finir
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    free_test_server_complete(server);
}

Test(tick_update_simple, test_update_game_tick_multiple_ticks)
{
    reset_mocks();
    
    server_t *server = create_test_server_complete();
    
    // Tester plusieurs ticks consécutifs
    for (int i = 1; i <= 5; i++) {
        update_game_tick(server);
        cr_assert_eq(server->current_tick, i);
    }
    
    free_test_server_complete(server);
}

Test(tick_update_simple, test_update_game_tick_client_busy)
{
    reset_mocks();
    
    server_t *server = create_test_server_complete();
    client_t *client = create_test_ai_client(1);
    
    // Ajouter le client à la liste
    client->next = server->client->next;
    server->client->next = client;
    
    // Client occupé jusqu'au tick 10
    client->player->busy_until = 10;
    client->player->command_queue[0] = strdup("Forward");
    client->player->queue_size = 1;
    
    update_game_tick(server);
    
    // Le client ne devrait pas traiter les commandes car il est occupé
    cr_assert_eq(server->current_tick, 1);
    cr_assert_eq(client->player->queue_size, 1); // Commande toujours en attente
    
    free_test_server_complete(server);
}

Test(tick_update_simple, test_update_game_tick_no_clients)
{
    reset_mocks();
    
    server_t *server = create_test_server_complete();
    
    // Test avec seulement le client dummy (pas de vrais clients)
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    free_test_server_complete(server);
}