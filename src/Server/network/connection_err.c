/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** connection_err
*/

#include "../include/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

void server_err(char *msg)
{
    perror(msg);
    exit(84);
}
