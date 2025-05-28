/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Server main file
*/
#include <stdio.h>
#include <string.h>
#include "include/server.h"
#include "include/parsing.h"

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

/**
 * @brief Main function for the zappy server
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return 0 on success, 84 on error
 */
int main(int ac, char **av)
{
    parsing_info_t parsed_info;

    if (ac <= 10 || check_mini_args(ac, av) == 1) {
        display_help();
        return 0;
    }
    parse_args(ac, av, &parsed_info);
    disp_args(&parsed_info);
    return 0;
}
