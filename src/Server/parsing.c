/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parsing
*/
#include "parsing.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

static bool check_zero(char *str)
{
    if (strcmp(str, "0") == 0)
        return true;
    return false;
}

static int parse_port(char **av, int i, parsing_info_t *parsed_info)
{
    int res = 0;

    if (av[i + 1] == NULL)
        parsing_error("-p argument null", parsed_info);
    if (check_zero(av[i + 1]))
        return res;
    res = atoi(av[i + 1]);
    if (res < 1 || res > 65535)
        parsing_error("Port needs to be between 1 and 65535", parsed_info);
    return res;
}

//! If needed to limit map coordinates add limit to 3rd if
static int parse_coordinates(char **av, int i, parsing_info_t *parsed_info)
{
    int res = 0;

    if (av[i + 1] == NULL)
        parsing_error("-x or -y argument null", parsed_info);
    if (check_zero(av[i + 1]))
        parsing_error("coordinates arg can't be null", parsed_info);
    res = atoi(av[i + 1]);
    if (res < 1)
        parsing_error("coordinates can't be less than 1", parsed_info);
    return res;
}

static char **parse_names(char **av, int i)
{
    int j = i + 1;
    int count = 0;
    char **result = malloc(sizeof(char *) * 2);

    while (av[j] != NULL && av[j][0] != '-'){
        result[count] = strdup(av[j]);
        result = realloc(result, (sizeof(char *) * (count + 2)));
        j++;
        count++;
    }
    result[count] = NULL;
    return result;
}

//! If needed to limit in any way client_nb change 3rd if
static int parse_client_nb(char **av, int i, parsing_info_t *parsed_info)
{
    int res = 0;

    if (av[i + 1] == NULL)
        parsing_error("-c argument null", parsed_info);
    if (check_zero(av[i + 1]))
        parsing_error("number of client can't be null", parsed_info);
    res = atoi(av[i + 1]);
    if (res < 1)
        parsing_error("number can't be less than 1", parsed_info);
    return res;
}

//! If needed to limit in any way frequence change 3rd if
static int parse_frequence(char **av, int i, parsing_info_t *parsed_info)
{
    int res = 0;

    if (av[i + 1] == NULL)
        parsing_error("-f argument null", parsed_info);
    if (check_zero(av[i + 1]))
        parsing_error("frequence can't be null", parsed_info);
    res = atoi(av[i + 1]);
    if (res < 1)
        parsing_error("frequence can't be less than 1", parsed_info);
    return res;
}

static void init_parsing(parsing_info_t *parsed_info)
{
    parsed_info->port = 0;
    parsed_info->width = 0;
    parsed_info->height = 0;
    parsed_info->client_nb = 0;
    parsed_info->frequence = 100;
    parsed_info->names = NULL;
}

static bool is_dup_flag(char **av, int current_pos, char *flag,
    parsing_info_t *parsed_info)
{
    char error_msg[256];

    for (int i = 1; i < current_pos; i++) {
        if (strcmp(av[i], flag) == 0) {
            sprintf(error_msg, "Duplicate flag: %s", flag);
            parsing_error(error_msg, parsed_info);
        }
    }
    return false;
}

void parse_args(int ac, char **av, parsing_info_t *parsed_info)
{
    init_parsing(parsed_info);
    for (int i = 1; i < ac; i++){
        if (strcmp(av[i], "-p") == 0 && !is_dup_flag(av, i, "-p", parsed_info))
            parsed_info->port = parse_port(av, i, parsed_info);
        if (strcmp(av[i], "-x") == 0 && !is_dup_flag(av, i, "-x", parsed_info))
            parsed_info->width = parse_coordinates(av, i, parsed_info);
        if (strcmp(av[i], "-y") == 0 && !is_dup_flag(av, i, "-y", parsed_info))
            parsed_info->height = parse_coordinates(av, i, parsed_info);
        if (strcmp(av[i], "-n") == 0 && !is_dup_flag(av, i, "-n", parsed_info))
            parsed_info->names = parse_names(av, i);
        if (strcmp(av[i], "-c") == 0 && !is_dup_flag(av, i, "-c", parsed_info))
            parsed_info->client_nb = parse_client_nb(av, i, parsed_info);
        if (strcmp(av[i], "-f") == 0 && !is_dup_flag(av, i, "-f", parsed_info))
            parsed_info->frequence = parse_frequence(av, i, parsed_info);
    }
}
