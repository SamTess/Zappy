/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests pour les commandes graphiques à 0% de coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/graphical_commands.h"
#include "../src/Server/include/parsing.h"

// Forward declarations for graphical commands
void command_pdi(server_t *server, client_t *client, char **args);
void command_pdr(server_t *server, client_t *client, char **args);
void command_pex(server_t *server, client_t *client, char **args);
void command_pgt(server_t *server, client_t *client, char **args);
void command_pie(server_t *server, client_t *client, char **args);
void command_sst(server_t *server, client_t *client, char **args);
void command_tna(server_t *server, client_t *client, char **args);

// Helper function to create a basic server
static server_t *create_test_server(void)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server) return NULL;
    
    memset(server, 0, sizeof(server_t));
    
    server->parsed_info = malloc(sizeof(parsing_info_t));
    if (!server->parsed_info) {
        free(server);
        return NULL;
    }
    memset(server->parsed_info, 0, sizeof(parsing_info_t));
    
    server->parsed_info->width = 10;
    server->parsed_info->height = 10;
    
    // Create team names
    server->parsed_info->names = malloc(sizeof(char*) * 3);
    server->parsed_info->names[0] = strdup("team1");
    server->parsed_info->names[1] = strdup("team2");
    server->parsed_info->names[2] = NULL;
    
    return server;
}

// Helper function to create a basic client
static client_t *create_test_client(void)
{
    client_t *client = malloc(sizeof(client_t));
    if (!client) return NULL;
    
    memset(client, 0, sizeof(client_t));
    client->client_fd = 1;
    client->type = GRAPHICAL;
    client->is_fully_connected = true;
    
    return client;
}

static void cleanup_server(server_t *server)
{
    if (!server) return;
    
    if (server->parsed_info) {
        if (server->parsed_info->names) {
            for (int i = 0; server->parsed_info->names[i]; i++) {
                free(server->parsed_info->names[i]);
            }
            free(server->parsed_info->names);
        }
        free(server->parsed_info);
    }
    free(server);
}

// Tests for command_pdi.c
TestSuite(zero_coverage_pdi);

Test(zero_coverage_pdi, test_command_pdi_valid_server)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"pdi", "1", NULL};
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    command_pdi(server, client, args);
    
    free(client);
    cleanup_server(server);
}

Test(zero_coverage_pdi, test_command_pdi_null_server)
{
    client_t *client = create_test_client();
    char *args[] = {"pdi", "1", NULL};
    
    command_pdi(NULL, client, args);
    
    free(client);
}

Test(zero_coverage_pdi, test_command_pdi_null_args)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    command_pdi(server, client, NULL);
    
    free(client);
    cleanup_server(server);
}

// Tests for command_pdr.c
TestSuite(zero_coverage_pdr);

Test(zero_coverage_pdr, test_command_pdr_valid_params)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"pdr", "1", "2", NULL};
    
    command_pdr(server, client, args);
    
    free(client);
    cleanup_server(server);
}

Test(zero_coverage_pdr, test_command_pdr_null_server)
{
    client_t *client = create_test_client();
    char *args[] = {"pdr", "1", "2", NULL};
    
    command_pdr(NULL, client, args);
    
    free(client);
}

// Tests for command_pex.c  
TestSuite(zero_coverage_pex);

Test(zero_coverage_pex, test_command_pex_valid_params)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"pex", "1", NULL};
    
    command_pex(server, client, args);
    
    free(client);
    cleanup_server(server);
}

Test(zero_coverage_pex, test_command_pex_null_client)
{
    server_t *server = create_test_server();
    char *args[] = {"pex", "1", NULL};
    
    command_pex(server, NULL, args);
    
    cleanup_server(server);
}

// Tests for command_pgt.c
TestSuite(zero_coverage_pgt);

Test(zero_coverage_pgt, test_command_pgt_valid_params)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"pgt", "1", "3", NULL};
    
    command_pgt(server, client, args);
    
    free(client);
    cleanup_server(server);
}

Test(zero_coverage_pgt, test_command_pgt_null_args)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    command_pgt(server, client, NULL);
    
    free(client);
    cleanup_server(server);
}

// Tests for command_pic.c
TestSuite(zero_coverage_pic);

Test(zero_coverage_pic, test_command_pic_valid_params)
{
    server_t *server = create_test_server();
    
    command_pic(server, 5, 5, 2);
    
    cleanup_server(server);
}

Test(zero_coverage_pic, test_command_pic_null_server)
{
    command_pic(NULL, 5, 5, 2);
}

// Tests for command_pie.c
TestSuite(zero_coverage_pie);

Test(zero_coverage_pie, test_command_pie_valid_params)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"pie", "5", "5", "1", NULL};
    
    command_pie(server, client, args);
    
    free(client);
    cleanup_server(server);
}

Test(zero_coverage_pie, test_command_pie_null_server)
{
    client_t *client = create_test_client();
    char *args[] = {"pie", "5", "5", "1", NULL};
    
    command_pie(NULL, client, args);
    
    free(client);
}

// Tests for command_sst.c
TestSuite(zero_coverage_sst);

Test(zero_coverage_sst, test_command_sst_valid_time)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"sst", "50", NULL};
    
    command_sst(server, client, args);
    
    free(client);
    cleanup_server(server);
}

Test(zero_coverage_sst, test_command_sst_null_server)
{
    client_t *client = create_test_client();
    char *args[] = {"sst", "50", NULL};
    
    command_sst(NULL, client, args);
    
    free(client);
}

// Tests for command_tna.c
TestSuite(zero_coverage_tna);

Test(zero_coverage_tna, test_command_tna_valid_server)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"tna", NULL};
    
    command_tna(server, client, args);
    
    free(client);
    cleanup_server(server);
}

Test(zero_coverage_tna, test_command_tna_null_client)
{
    server_t *server = create_test_server();
    char *args[] = {"tna", NULL};
    
    command_tna(server, NULL, args);
    
    cleanup_server(server);
}