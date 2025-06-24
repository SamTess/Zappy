/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parsing_err
*/
#include "include/parsing.h"
#include "include/server.h"
#include <stdlib.h>
#include <stdio.h>

static void free_names(char **names)
{
    for (int i = 0; names[i] != NULL; i++) {
        free(names[i]);
    }
    free(names);
}

void parsing_error(char *error_message, parsing_info_t *parsed_info)
{
    if (parsed_info->names != NULL && parsed_info->names[0] != NULL)
        free_names(parsed_info->names);
    printf("%s\n", error_message);
    display_help();
    exit(84);
}

bool check_if_names_exist(char **names, char *name, int k)
{
    for (int i = 0; names[i] != NULL; i++) {
        if (i == k)
            continue;
        if (strcmp(names[i], name) == 0)
            return true;
    }
    return false;
}
