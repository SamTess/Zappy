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
#include <signal.h>
#include "../include/server.h"

static int *get_running_flag(void)
{
    static int keep_running = 1;

    return &keep_running;
}

static void sigint_handler(int signum)
{
    int *flag = get_running_flag();

    (void)signum;
    *flag = 0;
}

static int setup_signal_handler(void)
{
    struct sigaction sa = {0};

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Failed to setup SIGINT handler");
        return -1;
    }
    return 0;
}

static int should_continue_running(void)
{
    int *keep_running = get_running_flag();

    return *keep_running;
}

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
    while (should_continue_running()) {
        check_client(server);
    }
    printf("Server is shutting down\n");
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
    if (setup_signal_handler() == -1)
        server_err("Couldn't setup signal handler");
    parse_args(ac, av, &parsed_info);
    disp_args(&parsed_info);
    create_server(&server, &parsed_info);
    set_rdm_seed();
    create_map(&server, &parsed_info);
    init_server_eggs(&server);
    server_loop(&server);
    free_all(&server, &parsed_info);
    close(server.s_fd);
    return 0;
}
