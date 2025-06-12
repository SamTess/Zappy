/*
** EPITECH PROJECT, 2025
** B-NWP-400-NAN-4-1-myftp-marin.lamy
** File description:
** array_function
*/

#include "../include/parsing.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int arr_len(char **array)
{
    int i = 0;

    if (!array)
        return 0;
    while (array[i])
        i++;
    return i;
}

static char *next_valid_token(char *str, char *delim, int *first)
{
    char *token;

    while (1) {
        if (*first == 0) {
            token = strtok(str, delim);
            *first = 1;
        } else
            token = strtok(NULL, delim);
        if (token == NULL)
            return NULL;
        if (strlen(token) > 0)
            return token;
    }
}

static int count_valid_tokens(char *str, char *delim)
{
    char *copy = strdup(str);
    char *token;
    int count = 0;
    int first = 0;

    if (!copy)
        return -1;
    while (1) {
        token = next_valid_token(copy, delim, &first);
        if (!token)
            break;
        count++;
    }
    free(copy);
    return count;
}

static char **init_result_array(int size)
{
    char **array = malloc(sizeof(char *) * (size + 1));

    if (!array)
        return NULL;
    array[size] = NULL;
    return array;
}

static char **free_cpy_arr(char *copy, char **array)
{
    if (array)
        free_arr(array);
    if (copy)
        free(copy);
    return NULL;
}

static int add_token(char **array, char *token, int pos)
{
    array[pos] = strdup(token);
    if (array[pos] == NULL)
        return 0;
    return 1;
}

static bool check_if_failed(char *copy, char **array)
{
    if (!copy) {
        if (array)
            free_arr(array);
        return true;
    }
    if (!array) {
        free(copy);
        return true;
    }
    return false;
}

static char **fill_result_array(char *str, char *delim, int size)
{
    char *copy = strdup(str);
    char **array = init_result_array(size);
    char *token;
    int pos = 0;
    int first = 0;

    if (check_if_failed(copy, array))
        return NULL;
    while (1) {
        token = next_valid_token(copy, delim, &first);
        if (!token || pos >= size)
            break;
        if (!add_token(array, token, pos))
            return free_cpy_arr(copy, array);
        pos++;
    }
    free(copy);
    return array;
}

static char **process_tokens(char *str_copy, char *delim)
{
    int count;
    char **array;

    count = count_valid_tokens(str_copy, delim);
    if (count < 0)
        return NULL;
    if (count == 0) {
        array = malloc(sizeof(char *));
        if (!array)
            return NULL;
        array[0] = NULL;
        return array;
    }
    array = fill_result_array(str_copy, delim, count);
    return array;
}

char **str_to_word_arr(char *str, char *delim)
{
    char *str_copy;
    char **result;
    int len = 0;

    if (!str || !delim)
        return NULL;
    len = strlen(str);
    str_copy = strdup(str);
    if (!str_copy)
        return NULL;
    if (len > 0 && str[len - 1] == '\n')
        str_copy[len - 1] = '\0';
    if (!str_copy)
        return NULL;
    result = process_tokens(str_copy, delim);
    free(str_copy);
    return result;
}
