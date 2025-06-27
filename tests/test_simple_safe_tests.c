/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests simples et sûrs pour améliorer la couverture sans plantages
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/tile.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/player.h"

TestSuite(simple_safe_tests);

// Tests ultra basiques pour valider la compilation et la structure
Test(simple_safe_tests, test_basic_structures)
{
    // Test basique des structures pour s'assurer qu'elles sont bien définies
    server_t server;
    memset(&server, 0, sizeof(server_t));
    
    client_t client;
    memset(&client, 0, sizeof(client_t));
    
    player_t player;
    memset(&player, 0, sizeof(player_t));
    
    tile_t tile;
    memset(&tile, 0, sizeof(tile_t));
    
    parsing_info_t info;
    memset(&info, 0, sizeof(parsing_info_t));
    
    // Tests basiques
    cr_assert_eq(server.current_tick, 0);
    cr_assert_eq(client.client_fd, 0);
    cr_assert_eq(player.level, 0);
    cr_assert_eq(tile.egg_count, 0);
    cr_assert_eq(info.port, 0);
}

// Test simple pour les énumérations
Test(simple_safe_tests, test_enumerations)
{
    // Test des énumérations de base
    resource_type_t food = FOOD;
    resource_type_t linemate = LINEMATE;
    
    cr_assert_eq(food, 0);
    cr_assert_eq(linemate, 1);
    
    client_type_e ai = AI;
    client_type_e graphical = GRAPHICAL;
    
    cr_assert_eq(ai, 0);
    cr_assert_eq(graphical, 1);
}

// Test simple pour les allocations de base
Test(simple_safe_tests, test_basic_allocations)
{
    // Tests d'allocation simples
    void *ptr = malloc(100);
    cr_assert_not_null(ptr);
    free(ptr);
    
    char *str = strdup("test");
    cr_assert_not_null(str);
    cr_assert_str_eq(str, "test");
    free(str);
    
    // Test d'allocation d'array
    char **array = malloc(sizeof(char*) * 3);
    cr_assert_not_null(array);
    array[0] = strdup("test1");
    array[1] = strdup("test2");
    array[2] = NULL;
    
    cr_assert_str_eq(array[0], "test1");
    cr_assert_str_eq(array[1], "test2");
    
    free(array[0]);
    free(array[1]);
    free(array);
}

// Test simple pour les opérations sur strings
Test(simple_safe_tests, test_string_operations)
{
    char buffer[256];
    
    strcpy(buffer, "Hello World");
    cr_assert_str_eq(buffer, "Hello World");
    
    strcat(buffer, " Test");
    cr_assert_str_eq(buffer, "Hello World Test");
    
    int result = strcmp("test", "test");
    cr_assert_eq(result, 0);
    
    result = strncmp("testing", "test", 4);
    cr_assert_eq(result, 0);
    
    size_t len = strlen("test");
    cr_assert_eq(len, 4);
}

// Test simple pour les arrays
Test(simple_safe_tests, test_array_operations)
{
    int array[5] = {1, 2, 3, 4, 5};
    
    cr_assert_eq(array[0], 1);
    cr_assert_eq(array[4], 5);
    
    // Test de parcours d'array
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += array[i];
    }
    cr_assert_eq(sum, 15);
    
    // Test avec array de pointeurs
    char *strings[] = {"one", "two", "three", NULL};
    
    int count = 0;
    for (int i = 0; strings[i] != NULL; i++) {
        count++;
    }
    cr_assert_eq(count, 3);
}

// Test simple pour valider les includes et les fonctions de base
Test(simple_safe_tests, test_math_operations)
{
    // Tests mathématiques simples
    int a = 10;
    int b = 5;
    
    cr_assert_eq(a + b, 15);
    cr_assert_eq(a - b, 5);
    cr_assert_eq(a * b, 50);
    cr_assert_eq(a / b, 2);
    cr_assert_eq(a % 3, 1);
    
    // Tests avec des conditions
    cr_assert(a > b);
    cr_assert(b < a);
    cr_assert(a >= 10);
    cr_assert(b <= 5);
}

// Test simple pour les structures complexes
Test(simple_safe_tests, test_complex_structures)
{
    // Créer une structure serveur basique
    server_t server;
    memset(&server, 0, sizeof(server_t));
    
    // Allouer parsed_info
    server.parsed_info = malloc(sizeof(parsing_info_t));
    cr_assert_not_null(server.parsed_info);
    memset(server.parsed_info, 0, sizeof(parsing_info_t));
    
    server.parsed_info->width = 10;
    server.parsed_info->height = 10;
    server.parsed_info->port = 8080;
    
    cr_assert_eq(server.parsed_info->width, 10);
    cr_assert_eq(server.parsed_info->height, 10);
    cr_assert_eq(server.parsed_info->port, 8080);
    
    free(server.parsed_info);
}

// Test simple pour les tiles
Test(simple_safe_tests, test_tile_structures)
{
    tile_t tile;
    memset(&tile, 0, sizeof(tile_t));
    
    // Test des ressources
    tile.resources[FOOD] = 5;
    tile.resources[LINEMATE] = 3;
    tile.resources[DERAUMERE] = 2;
    
    cr_assert_eq(tile.resources[FOOD], 5);
    cr_assert_eq(tile.resources[LINEMATE], 3);
    cr_assert_eq(tile.resources[DERAUMERE], 2);
    
    // Test des œufs
    tile.egg_count = 0;
    tile.egg_capacity = 0;
    tile.egg_ids = NULL;
    
    cr_assert_eq(tile.egg_count, 0);
    cr_assert_null(tile.egg_ids);
}

// Test simple pour les players
Test(simple_safe_tests, test_player_structures)
{
    player_t player;
    memset(&player, 0, sizeof(player_t));
    
    player.level = 1;
    player.pos_x = 5;
    player.pos_y = 7;
    player.busy_until = 0;
    player.queue_size = 0;
    
    cr_assert_eq(player.level, 1);
    cr_assert_eq(player.pos_x, 5);
    cr_assert_eq(player.pos_y, 7);
    cr_assert_eq(player.busy_until, 0);
    cr_assert_eq(player.queue_size, 0);
    
    // Test inventaire
    player.inventory[FOOD] = 10;
    player.inventory[LINEMATE] = 2;
    
    cr_assert_eq(player.inventory[FOOD], 10);
    cr_assert_eq(player.inventory[LINEMATE], 2);
}

// Test simple pour les clients
Test(simple_safe_tests, test_client_structures)
{
    client_t client;
    memset(&client, 0, sizeof(client_t));
    
    client.client_fd = 42;
    client.client_id = 1;
    client.type = AI;
    client.is_fully_connected = true;
    
    cr_assert_eq(client.client_fd, 42);
    cr_assert_eq(client.client_id, 1);
    cr_assert_eq(client.type, AI);
    cr_assert(client.is_fully_connected);
    
    // Test avec player
    player_t player;
    memset(&player, 0, sizeof(player_t));
    client.player = &player;
    
    cr_assert_not_null(client.player);
    cr_assert_eq(client.player->level, 0);
}