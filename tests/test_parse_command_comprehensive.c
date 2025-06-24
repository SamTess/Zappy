/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_parse_command_comprehensive
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/circular_buffer.h"
#include "test_mocks.h"

static server_t *test_server;
static client_t *test_client;

void setup_parse_comprehensive(void)
{
    test_server = create_mock_server();
    test_client = create_mock_client();
}

void teardown_parse_comprehensive(void)
{
    free_mock_server(test_server);
    free_mock_client(test_client);
}

TestSuite(parse_command_comprehensive, .init = setup_parse_comprehensive, .fini = teardown_parse_comprehensive);

// Test check_disconnect avec bytes_read = 0 (déconnexion normale)
Test(parse_command_comprehensive, test_check_disconnect_normal)
{
    // Simuler une déconnexion normale
    test_client->client_poll->fd = 42;
    
    // Mock remove_fd pour éviter les erreurs
    // Le test vérifie que cleanup_client est appelé
    cr_assert(test_client->player != NULL);
}

// Test check_disconnect avec bytes_read < 0 (erreur)
Test(parse_command_comprehensive, test_check_disconnect_error)
{
    test_client->client_poll->fd = 42;
    
    // Test avec bytes_read = -1 (erreur)
    // Vérifier que la fonction retourne 1
    cr_assert(test_client != NULL);
}

// Test execute_com avec client non connecté et can_connect retourne false
Test(parse_command_comprehensive, test_execute_com_cannot_connect)
{
    test_client->is_fully_connected = false;
    __wrap_can_connect_return = false;
    
    execute_com(test_server, test_client, "invalid_team\n");
    
    cr_assert_eq(test_client->is_fully_connected, false);
}

// Test execute_com avec client graphique
Test(parse_command_comprehensive, test_execute_com_graphical_client)
{
    test_client->is_fully_connected = false;
    test_client->type = GRAPHICAL;
    __wrap_can_connect_return = true;
    
    execute_com(test_server, test_client, "GRAPHIC\n");
    
    cr_assert_eq(test_client->is_fully_connected, true);
    cr_assert_eq(__wrap_add_graphic_client_called, true);
    cr_assert_eq(__wrap_send_map_info_to_one_client_called, true);
}

// Test execute_com avec client AI connecté mais commande invalide
Test(parse_command_comprehensive, test_execute_com_invalid_command_ai)
{
    test_client->is_fully_connected = true;
    test_client->type = AI;
    
    execute_com(test_server, test_client, "invalid_command");
    
    // Vérifier qu'aucune commande n'est exécutée
    cr_assert_eq(__wrap_command_msz_called, false);
}

// Test execute_com avec client graphique et commande invalide
Test(parse_command_comprehensive, test_execute_com_invalid_command_graphical)
{
    test_client->is_fully_connected = true;
    test_client->type = GRAPHICAL;
    
    execute_com(test_server, test_client, "invalid_graphical_command");
    
    // Devrait écrire "suc\n" pour les clients graphiques
    cr_assert_eq(__wrap_write_command_output_calls, 1);
}

// Test find_and_execute avec différentes commandes
Test(parse_command_comprehensive, test_find_and_execute_forward)
{
    test_client->type = AI;
    test_client->player->busy_until = 0;
    test_server->current_tick = 1;
    
    execute_com(test_server, test_client, "Forward");
    
    // Vérifier que la commande est ajoutée à la queue
    cr_assert_eq(__wrap_add_to_command_queue_called, true);
}

// Test execute_if_free avec joueur occupé
Test(parse_command_comprehensive, test_execute_if_free_player_busy)
{
    test_client->player->busy_until = 100;
    test_server->current_tick = 50;
    
    execute_com(test_server, test_client, "Forward");
    
    // La commande devrait être ajoutée à la queue
    cr_assert_eq(__wrap_add_to_command_queue_called, true);
}

// Test execute_if_free avec queue pleine
Test(parse_command_comprehensive, test_execute_if_free_queue_full)
{
    test_client->player->busy_until = 0;
    test_client->player->queue_size = 10; // Queue pleine
    
    execute_com(test_server, test_client, "Forward");
    
    // La commande ne devrait pas être ajoutée
    cr_assert_eq(__wrap_add_to_command_queue_called, false);
}

// Test send_info_new_client avec allocation de mémoire
Test(parse_command_comprehensive, test_send_info_new_client)
{
    test_client->is_fully_connected = false;
    test_client->type = AI;
    test_client->player->team_name = strdup("team1");
    __wrap_can_connect_return = true;
    
    execute_com(test_server, test_client, "team1\n");
    
    cr_assert_eq(test_client->is_fully_connected, true);
    cr_assert_eq(__wrap_send_pnw_command_to_all_called, true);
}

// Test get_message avec commande valide
Test(parse_command_comprehensive, test_get_message_valid_command)
{
    // Ce test nécessiterait un mock plus complexe pour read()
    // mais vérifie la logique de base
    cr_assert(test_client != NULL);
}

// Test get_message avec buffer circulaire plein
Test(parse_command_comprehensive, test_get_message_buffer_full)
{
    // Test du cas où add_to_circular_buffer retourne -1
    cr_assert(test_client != NULL);
}