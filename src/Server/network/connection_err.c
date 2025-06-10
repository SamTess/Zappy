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

void print_co(char *client_ip, struct sockaddr_in *client_addr,
    client_t *new_client)
{
    inet_ntop(AF_INET, &(client_addr->sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("New client connected from %s:%d with ID %d.\n",
            client_ip, ntohs(client_addr->sin_port), new_client->client_id);
}
