/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_object_commands - Tests for set_object and take_object commands
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "test_mocks.h"

// Déclarations externes pour les mocks
extern int __wrap_write_command_output_calls;
extern char *__wrap_write_command_output_last_msg;
extern int __wrap_command_pdr_calls;
extern int __wrap_command_pgt_calls;

// Tests pour set_object
Test(object_commands_tests, test_set_object_food_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    
    // Setup player
    client.player = &player;
    player.x = 5;
    player.y = 5;
    player.inventory.food = 2;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pdr_calls = 0;
    
    set_object(&server, &client, "food");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ok\n");
    cr_assert_eq(player.inventory.food, 1);
    cr_assert_eq(tiles[5 * 10 + 5].food, 1);
    cr_assert_eq(__wrap_command_pdr_calls, 1);
}

Test(object_commands_tests, test_set_object_linemate_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    
    // Setup player
    client.player = &player;
    player.x = 3;
    player.y = 4;
    player.inventory.linemate = 3;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pdr_calls = 0;
    
    set_object(&server, &client, "linemate");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ok\n");
    cr_assert_eq(player.inventory.linemate, 2);
    cr_assert_eq(tiles[4 * 10 + 3].linemate, 1);
    cr_assert_eq(__wrap_command_pdr_calls, 1);
}

Test(object_commands_tests, test_set_object_deraumere_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    
    // Setup player
    client.player = &player;
    player.x = 2;
    player.y = 7;
    player.inventory.deraumere = 1;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pdr_calls = 0;
    
    set_object(&server, &client, "deraumere");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ok\n");
    cr_assert_eq(player.inventory.deraumere, 0);
    cr_assert_eq(tiles[7 * 10 + 2].deraumere, 1);
    cr_assert_eq(__wrap_command_pdr_calls, 1);
}

Test(object_commands_tests, test_set_object_sibur_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    
    // Setup player
    client.player = &player;
    player.x = 1;
    player.y = 8;
    player.inventory.sibur = 4;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pdr_calls = 0;
    
    set_object(&server, &client, "sibur");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ok\n");
    cr_assert_eq(player.inventory.sibur, 3);
    cr_assert_eq(tiles[8 * 10 + 1].sibur, 1);
    cr_assert_eq(__wrap_command_pdr_calls, 1);
}

Test(object_commands_tests, test_set_object_mendiane_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    
    // Setup player
    client.player = &player;
    player.x = 6;
    player.y = 2;
    player.inventory.mendiane = 2;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pdr_calls = 0;
    
    set_object(&server, &client, "mendiane");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ok\n");
    cr_assert_eq(player.inventory.mendiane, 1);
    cr_assert_eq(tiles[2 * 10 + 6].mendiane, 1);
    cr_assert_eq(__wrap_command_pdr_calls, 1);
}

Test(object_commands_tests, test_set_object_phiras_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    
    // Setup player
    client.player = &player;
    player.x = 9;
    player.y = 1;
    player.inventory.phiras = 1;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pdr_calls = 0;
    
    set_object(&server, &client, "phiras");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ok\n");
    cr_assert_eq(player.inventory.phiras, 0);
    cr_assert_eq(tiles[1 * 10 + 9].phiras, 1);
    cr_assert_eq(__wrap_command_pdr_calls, 1);
}

Test(object_commands_tests, test_set_object_thystame_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    
    // Setup player
    client.player = &player;
    player.x = 4;
    player.y = 6;
    player.inventory.thystame = 3;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pdr_calls = 0;
    
    set_object(&server, &client, "thystame");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ok\n");
    cr_assert_eq(player.inventory.thystame, 2);
    cr_assert_eq(tiles[6 * 10 + 4].thystame, 1);
    cr_assert_eq(__wrap_command_pdr_calls, 1);
}

Test(object_commands_tests, test_set_object_no_item)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    
    // Setup player avec inventaire vide
    client.player = &player;
    player.x = 5;
    player.y = 5;
    player.inventory.food = 0;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pdr_calls = 0;
    
    set_object(&server, &client, "food");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ko\n");
    cr_assert_eq(player.inventory.food, 0);
    cr_assert_eq(tiles[5 * 10 + 5].food, 0);
    cr_assert_eq(__wrap_command_pdr_calls, 0);
}

Test(object_commands_tests, test_set_object_invalid_object)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.player = &player;
    
    __wrap_write_command_output_calls = 0;
    
    set_object(&server, &client, "invalid_object");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ko\n");
}

// Tests pour take_object
Test(object_commands_tests, test_take_object_food_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map avec nourriture
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    tiles[5 * 10 + 5].food = 3;
    
    // Setup player
    client.player = &player;
    player.x = 5;
    player.y = 5;
    player.inventory.food = 1;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pgt_calls = 0;
    
    take_object(&server, &client, "food");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ok\n");
    cr_assert_eq(player.inventory.food, 2);
    cr_assert_eq(tiles[5 * 10 + 5].food, 2);
    cr_assert_eq(__wrap_command_pgt_calls, 1);
}

Test(object_commands_tests, test_take_object_linemate_success)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map avec linemate
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    tiles[3 * 10 + 4].linemate = 2;
    
    // Setup player
    client.player = &player;
    player.x = 4;
    player.y = 3;
    player.inventory.linemate = 0;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pgt_calls = 0;
    
    take_object(&server, &client, "linemate");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ok\n");
    cr_assert_eq(player.inventory.linemate, 1);
    cr_assert_eq(tiles[3 * 10 + 4].linemate, 1);
    cr_assert_eq(__wrap_command_pgt_calls, 1);
}

Test(object_commands_tests, test_take_object_all_stones)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map avec toutes les pierres
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    tiles[2 * 10 + 7].deraumere = 1;
    tiles[2 * 10 + 7].sibur = 1;
    tiles[2 * 10 + 7].mendiane = 1;
    tiles[2 * 10 + 7].phiras = 1;
    tiles[2 * 10 + 7].thystame = 1;
    
    // Setup player
    client.player = &player;
    player.x = 7;
    player.y = 2;
    
    __wrap_command_pgt_calls = 0;
    
    // Test deraumere
    take_object(&server, &client, "deraumere");
    cr_assert_eq(player.inventory.deraumere, 1);
    cr_assert_eq(tiles[2 * 10 + 7].deraumere, 0);
    
    // Test sibur
    take_object(&server, &client, "sibur");
    cr_assert_eq(player.inventory.sibur, 1);
    cr_assert_eq(tiles[2 * 10 + 7].sibur, 0);
    
    // Test mendiane
    take_object(&server, &client, "mendiane");
    cr_assert_eq(player.inventory.mendiane, 1);
    cr_assert_eq(tiles[2 * 10 + 7].mendiane, 0);
    
    // Test phiras
    take_object(&server, &client, "phiras");
    cr_assert_eq(player.inventory.phiras, 1);
    cr_assert_eq(tiles[2 * 10 + 7].phiras, 0);
    
    // Test thystame
    take_object(&server, &client, "thystame");
    cr_assert_eq(player.inventory.thystame, 1);
    cr_assert_eq(tiles[2 * 10 + 7].thystame, 0);
    
    cr_assert_eq(__wrap_command_pgt_calls, 5);
}

Test(object_commands_tests, test_take_object_no_item_on_tile)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    map_t map = {0};
    tile_t tiles[100] = {0};
    
    // Setup map sans objets
    server.map = &map;
    map.width = 10;
    map.height = 10;
    map.tiles = tiles;
    tiles[5 * 10 + 5].food = 0;
    
    // Setup player
    client.player = &player;
    player.x = 5;
    player.y = 5;
    player.inventory.food = 0;
    
    __wrap_write_command_output_calls = 0;
    __wrap_command_pgt_calls = 0;
    
    take_object(&server, &client, "food");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ko\n");
    cr_assert_eq(player.inventory.food, 0);
    cr_assert_eq(tiles[5 * 10 + 5].food, 0);
    cr_assert_eq(__wrap_command_pgt_calls, 0);
}

Test(object_commands_tests, test_take_object_invalid_object)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.player = &player;
    
    __wrap_write_command_output_calls = 0;
    
    take_object(&server, &client, "invalid_object");
    
    cr_assert_eq(__wrap_write_command_output_calls, 1);
    cr_assert_str_eq(__wrap_write_command_output_last_msg, "ko\n");
}

Test(object_commands_tests, test_take_object_null_parameters)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.player = &player;
    
    __wrap_write_command_output_calls = 0;
    
    // Test avec paramètres NULL
    take_object(NULL, &client, "food");
    take_object(&server, NULL, "food");
    take_object(&server, &client, NULL);
    
    // Aucun crash attendu
    cr_assert_eq(__wrap_write_command_output_calls, 0);
}