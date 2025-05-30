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

static void malloc_failed_bis(int i)
{
    if (i == 5){
        perror("Malloc failed for command qeue");
        exit(84);
    }
}

void malloc_failed(int i)
{
    if (i > 5)
        malloc_failed_bis(i);
    if (i == 1){
        perror("New client allocation failed");
        exit(84);
    }
    if (i == 2){
        perror("Poll fd struct allocation in new client struct failed");
        exit(84);
    }
    if (i == 3){
        perror("Failed to allocate player");
        exit(84);
    }
    if (i == 4){
        perror("Strdup failed for player team name");
        exit(84);
    }
}

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
