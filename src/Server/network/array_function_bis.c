/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** array_function_bis
*/
#include "../include/parsing.h"
#include <stdlib.h>

void free_arr(char **array)
{
    if (!array)
        return;
    for (int i = 0; array[i] != NULL; i++) {
        if (array[i] == NULL)
            break;
        free(array[i]);
    }
    free(array);
}
