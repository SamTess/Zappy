/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Server main file
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "include/server.h"

static int check_mini_args(int ac, char **av)
{
    int count = 0;

    for (int i = 0; i < ac; i++){
        if (strcmp(av[i], "-p") == 0 || strcmp(av[i], "-x") == 0 ||
            strcmp(av[i], "-y") == 0 || strcmp(av[i], "-n") == 0 ||
            strcmp(av[i], "-c") == 0)
            count++;
    }
    if (count == 5)
        return 0;
    return 1;
}

static void disp_args(parsing_info_t *parsed_info)
{
    printf("port = %d\n", parsed_info->port);
    printf("width = %d\n", parsed_info->width);
    printf("height = %d\n", parsed_info->height);
    printf("clients_nb = %d\n", parsed_info->client_nb);
    printf("freq = %d\n", parsed_info->frequence);
    printf("name :");
    for (int i = 0; parsed_info->names[i] != NULL; i++){
        printf(" %s", parsed_info->names[i]);
    }
    printf("\n");
}

static void server_loop(server_t *server)
{
    exit(0);
    while (1) {
        check_client(server);
    }
    close(server->s_fd);
}

static void print_tile_resources(const tile_t *tile)
{
    static const char *resource_names[7] = {
        "FOOD",
        "LINEMATE",
        "DERAUMERE",
        "SIBUR",
        "MENDIANE",
        "PHIRAS",
        "THYSTAME"
    };

    for (int r = 0; r < 7; ++r) {
        printf("%s: %d ", resource_names[r], tile->resources[r]);
    }
}

static void print_map(tile_t **map, int width, int height)
{
    int x, y;
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            printf("Tile (%d,%d): ", x, y);
            print_tile_resources(&map[y][x]);
            printf("| Players: %d\n", map[y][x].player_count);
        }
    }
}

static void print_resource_totals(tile_t **map, int width, int height)
{
    int totals[7] = {0};
    int x, y, r;
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            for (r = 0; r < 7; ++r) {
                totals[r] += map[y][x].resources[r];
            }
        }
    }
    static const char *resource_names[7] = {
        "FOOD",
        "LINEMATE",
        "DERAUMERE",
        "SIBUR",
        "MENDIANE",
        "PHIRAS",
        "THYSTAME"
    };
    printf("Resource totals for the map (%dx%d):\n", width, height);
    for (r = 0; r < 7; ++r) {
        printf("%s: %d\n", resource_names[r], totals[r]);
    }
}

static void set_rdm_seed(void)
{
    unsigned long seed = (unsigned long)time(NULL);
    seed ^= (unsigned long)getpid();
    seed ^= (unsigned long)(uintptr_t)&seed;
    seed *= 0x5DEECE66DULL;
    srand((unsigned int)(seed & 0xFFFFFFFF));
}

/**
 * @brief Main function for the zappy server
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return 0 on success, 84 on error
 */
int main(int ac, char **av)
{
    parsing_info_t parsed_info;
    server_t server;

    if (ac <= 10 || check_mini_args(ac, av) == 1) {
        display_help();
        return 0;
    }
    parse_args(ac, av, &parsed_info);
    disp_args(&parsed_info);
    create_server(&server, &parsed_info);
    set_rdm_seed();
    create_map(&server, &parsed_info);
    print_map(server.map, parsed_info.width, parsed_info.height);
    print_resource_totals(server.map, parsed_info.width, parsed_info.height);
    server_loop(&server);
    return 0;
}
