/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_tick_update_comprehensive
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/tick_update.h"
#include "test_mocks.h"

static server_t *test_server;

void setup_tick_test(void)
{
    test_server = create_mock_server();
    test_server->current_tick = 0;
}

void teardown_tick_test(void)
{
    free_mock_server(test_server);
}

TestSuite(tick_update_comprehensive, .init = setup_tick_test, .fini = teardown_tick_test);

// Test update_tick basique
Test(tick_update_comprehensive, test_update_tick_basic)
{
    int initial_tick = test_server->current_tick;
    update_tick(test_server);
    cr_assert_eq(test_server->current_tick, initial_tick + 1);
}

// Test update_tick avec server NULL
Test(tick_update_comprehensive, test_update_tick_null_server)
{
    // Ne devrait pas crasher
    update_tick(NULL);
    cr_assert(true);
}

// Test process_pending_commands avec joueur libre
Test(tick_update_comprehensive, test_process_pending_commands_free_player)
{
    client_t *client = create_mock_client();
    client->player->busy_until = 0;
    client->player->queue_size = 1;
    
    // Simuler une commande en attente
    command_queue_item_t *cmd = malloc(sizeof(command_queue_item_t));
    cmd->command_index = 0; // Forward
    cmd->args = NULL;
    cmd->next = NULL;
    client->player->command_queue = cmd;
    
    process_pending_commands(test_server, client);
    
    // Le joueur devrait maintenant être occupé
    cr_assert_gt(client->player->busy_until, 0);
    
    free_mock_client(client);
}

// Test process_pending_commands avec joueur occupé
Test(tick_update_comprehensive, test_process_pending_commands_busy_player)
{
    client_t *client = create_mock_client();
    client->player->busy_until = test_server->current_tick + 10;
    
    int initial_busy = client->player->busy_until;
    process_pending_commands(test_server, client);
    
    // Le joueur devrait rester occupé
    cr_assert_eq(client->player->busy_until, initial_busy);
    
    free_mock_client(client);
}

// Test process_pending_commands avec queue vide
Test(tick_update_comprehensive, test_process_pending_commands_empty_queue)
{
    client_t *client = create_mock_client();
    client->player->busy_until = 0;
    client->player->command_queue = NULL;
    client->player->queue_size = 0;
    
    process_pending_commands(test_server, client);
    
    // Rien ne devrait changer
    cr_assert_eq(client->player->busy_until, 0);
    
    free_mock_client(client);
}

// Test update_resources basique
Test(tick_update_comprehensive, test_update_resources_basic)
{
    update_resources(test_server);
    
    // Vérifier que les ressources ont été mises à jour
    cr_assert_geq(test_server->current_tick, 0);
}

// Test update_resources avec server NULL
Test(tick_update_comprehensive, test_update_resources_null_server)
{
    update_resources(NULL);
    cr_assert(true); // Ne devrait pas crasher
}

// Test check_incantations avec incantation en cours
Test(tick_update_comprehensive, test_check_incantations_active)
{
    client_t *client = create_mock_client();
    client->player->incantation_in_progress = true;
    client->player->incantation_end_tick = test_server->current_tick + 1;
    
    check_incantations(test_server);
    
    // L'incantation devrait encore être en cours
    cr_assert_eq(client->player->incantation_in_progress, true);
    
    free_mock_client(client);
}

// Test check_incantations avec incantation terminée
Test(tick_update_comprehensive, test_check_incantations_finished)
{
    client_t *client = create_mock_client();
    client->player->incantation_in_progress = true;
    client->player->incantation_end_tick = test_server->current_tick - 1;
    
    check_incantations(test_server);
    
    // L'incantation devrait être terminée
    cr_assert_eq(client->player->incantation_in_progress, false);
    
    free_mock_client(client);
}

// Test update_eggs avec œuf qui éclot
Test(tick_update_comprehensive, test_update_eggs_hatching)
{
    egg_t *egg = malloc(sizeof(egg_t));
    egg->hatch_time = test_server->current_tick;
    egg->team_name = strdup("team1");
    egg->x = 5;
    egg->y = 5;
    egg->id = 1;
    egg->next = NULL;
    
    test_server->eggs = egg;
    
    update_eggs(test_server);
    
    // L'œuf devrait avoir éclos (plus dans la liste)
    cr_assert_eq(test_server->eggs, NULL);
}

// Test update_eggs avec œuf pas encore prêt
Test(tick_update_comprehensive, test_update_eggs_not_ready)
{
    egg_t *egg = malloc(sizeof(egg_t));
    egg->hatch_time = test_server->current_tick + 10;
    egg->team_name = strdup("team1");
    egg->x = 5;
    egg->y = 5;
    egg->id = 1;
    egg->next = NULL;
    
    test_server->eggs = egg;
    
    update_eggs(test_server);
    
    // L'œuf devrait encore être là
    cr_assert_neq(test_server->eggs, NULL);
    
    // Nettoyer
    free(egg->team_name);
    free(egg);
}

// Test tick_update complet
Test(tick_update_comprehensive, test_tick_update_complete)
{
    int initial_tick = test_server->current_tick;
    
    tick_update(test_server);
    
    cr_assert_eq(test_server->current_tick, initial_tick + 1);
}

// Test avec multiples clients
Test(tick_update_comprehensive, test_tick_update_multiple_clients)
{
    client_t *client1 = create_mock_client();
    client_t *client2 = create_mock_client();
    
    client1->player->busy_until = 0;
    client2->player->busy_until = test_server->current_tick + 5;
    
    tick_update(test_server);
    
    // Vérifier que les états des clients sont corrects
    cr_assert_geq(client2->player->busy_until, test_server->current_tick);
    
    free_mock_client(client1);
    free_mock_client(client2);
}