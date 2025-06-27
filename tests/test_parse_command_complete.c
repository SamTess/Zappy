/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_parse_command_complete - Tests complets pour parse_command.c
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "../src/Server/include/circular_buffer.h"
#include "../src/Server/include/pending_cmd.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

// Mocks pour les fonctions externes
void __wrap_cleanup_client(client_t *user) { (void)user; }
void __wrap_remove_fd(server_t *server, int fd) { (void)server; (void)fd; }
void __wrap_forward(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_right(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_left(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_inventory(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_look(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_eject(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_connect_nbr(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_take_object(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_set_object(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_start_incantation(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_fork_c(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_broadcast(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_command_msz(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_command_bct(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_command_mtc(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_command_tna(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_command_ppo(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_command_plv(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_command_pin(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_command_sgt(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }
void __wrap_command_sst(server_t *s, client_t *c, char **args) { (void)s; (void)c; (void)args; }

void __wrap_free_arr(char **arr) {
    if (!arr) return;
    for (int i = 0; arr[i]; i++) free(arr[i]);
    free(arr);
}

char **__wrap_str_to_word_arr(char *str, char *delimiter) {
    (void)delimiter;
    char **result = malloc(2 * sizeof(char*));
    result[0] = strdup(str);
    result[1] = NULL;
    return result;
}

void __wrap_add_pending_cmd(client_t *user, server_t *server, char *buffer, int cmd_index) {
    (void)user; (void)server; (void)buffer; (void)cmd_index;
}

void __wrap_add_to_command_queue(server_t *server, client_t *user, char *buffer) {
    (void)server; (void)user; (void)buffer;
}

bool __wrap_can_connect(server_t *server, client_t *user, char *buffer) {
    (void)server; (void)user; (void)buffer;
    return true;
}

void __wrap_add_graphic_client(server_t *server, client_t *user) {
    (void)server; (void)user;
}

void __wrap_send_map_info_to_one_client(server_t *server, client_t *user) {
    (void)server; (void)user;
}

int __wrap_connect_nbr_srv(server_t *server, char *team_name) {
    (void)server; (void)team_name;
    return 5;
}

void __wrap_write_command_output(int fd, char *output) {
    (void)fd; (void)output;
}

void __wrap_send_pnw_command_to_all(server_t *server, client_t *user) {
    (void)server; (void)user;
}

char *__wrap_extract_command(circular_buffer_t *buffer, int length) {
    (void)buffer; (void)length;
    return strdup("test_command");
}

int __wrap_find_command_end(circular_buffer_t *buffer) {
    (void)buffer;
    return 5;
}

void __wrap_init_circular_buffer(circular_buffer_t *buffer) {
    (void)buffer;
}

int __wrap_add_to_circular_buffer(circular_buffer_t *buffer, char byte) {
    (void)buffer; (void)byte;
    return 0;
}

// Helper pour créer un client de test
static client_t *create_test_client(enum client_type_e type) {
    client_t *client = malloc(sizeof(client_t));
    client->type = type;
    client->client_id = 1;
    client->client_fd = 1;
    client->is_fully_connected = false;
    
    if (type == AI) {
        client->player = malloc(sizeof(player_t));
        client->player->busy_until = 0;
        client->player->queue_size = 0;
        client->player->team_name = strdup("team1");
    } else {
        client->player = NULL;
    }
    
    client->client_poll = malloc(sizeof(struct pollfd));
    client->client_poll->fd = 1;
    
    return client;
}

// Helper pour créer un serveur de test
static server_t *create_test_server(void) {
    server_t *server = malloc(sizeof(server_t));
    server->current_tick = 10;
    server->parsed_info = malloc(sizeof(parsed_args_t));
    server->parsed_info->width = 10;
    server->parsed_info->height = 10;
    return server;
}

// Tests pour get_command_data
Test(parse_command, test_get_command_data) {
    command_data_t data = get_command_data();
    
    cr_assert_not_null(data.commands);
    cr_assert_not_null(data.functions);
    cr_assert_not_null(data.times);
    cr_assert_not_null(data.accepted_types);
    
    // Vérifier quelques commandes de base
    cr_assert_str_eq(data.commands[0], "Forward");
    cr_assert_str_eq(data.commands[1], "Right");
    cr_assert_str_eq(data.commands[2], "Left");
    cr_assert_str_eq(data.commands[12], "msz");
}

// Tests pour execute_com avec client non connecté
Test(parse_command, test_execute_com_not_connected_ai) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(AI);
    
    execute_com(server, client, "team1");
    
    cr_assert_eq(client->is_fully_connected, true);
    
    free(client->player->team_name);
    free(client->player);
    free(client->client_poll);
    free(client);
    free(server->parsed_info);
    free(server);
}

// Tests pour execute_com avec client graphique non connecté
Test(parse_command, test_execute_com_not_connected_graphical) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(GRAPHICAL);
    
    execute_com(server, client, "GRAPHIC");
    
    cr_assert_eq(client->is_fully_connected, true);
    
    free(client->client_poll);
    free(client);
    free(server->parsed_info);
    free(server);
}

// Tests pour execute_com avec commande valide AI
Test(parse_command, test_execute_com_valid_ai_command) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(AI);
    client->is_fully_connected = true;
    
    execute_com(server, client, "Forward");
    
    free(client->player->team_name);
    free(client->player);
    free(client->client_poll);
    free(client);
    free(server->parsed_info);
    free(server);
}

// Tests pour execute_com avec commande valide graphique
Test(parse_command, test_execute_com_valid_graphical_command) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(GRAPHICAL);
    client->is_fully_connected = true;
    
    execute_com(server, client, "msz");
    
    free(client->client_poll);
    free(client);
    free(server->parsed_info);
    free(server);
}

// Tests pour execute_com avec commande invalide
Test(parse_command, test_execute_com_invalid_command) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(AI);
    client->is_fully_connected = true;
    
    execute_com(server, client, "invalid_command");
    
    free(client->player->team_name);
    free(client->player);
    free(client->client_poll);
    free(client);
    free(server->parsed_info);
    free(server);
}

// Tests pour execute_com avec client NULL
Test(parse_command, test_execute_com_null_client) {
    server_t *server = create_test_server();
    
    execute_com(server, NULL, "Forward");
    
    free(server->parsed_info);
    free(server);
}

// Tests pour AI avec player occupé
Test(parse_command, test_execute_com_ai_busy) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(AI);
    client->is_fully_connected = true;
    client->player->busy_until = 20; // > server->current_tick (10)
    
    execute_com(server, client, "Forward");
    
    free(client->player->team_name);
    free(client->player);
    free(client->client_poll);
    free(client);
    free(server->parsed_info);
    free(server);
}

// Tests pour AI avec queue pleine
Test(parse_command, test_execute_com_ai_queue_full) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(AI);
    client->is_fully_connected = true;
    client->player->busy_until = 20;
    client->player->queue_size = 10; // Queue pleine
    
    execute_com(server, client, "Forward");
    
    free(client->player->team_name);
    free(client->player);
    free(client->client_poll);
    free(client);
    free(server->parsed_info);
    free(server);
}

// Mock pour read() qui simule la lecture d'un message
static int mock_read_calls = 0;
static char mock_data[] = "Forward\n";

ssize_t __wrap_read(int fd, void *buf, size_t count) {
    (void)fd; (void)count;
    
    if (mock_read_calls >= strlen(mock_data)) {
        return 0; // EOF pour terminer la boucle
    }
    
    *(char*)buf = mock_data[mock_read_calls];
    mock_read_calls++;
    return 1;
}

// Tests pour get_message
Test(parse_command, test_get_message_normal) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(AI);
    mock_read_calls = 0;
    
    get_message(server, client);
    
    free(client->player->team_name);
    free(client->player);
    free(client->client_poll);
    free(client);
    free(server->parsed_info);
    free(server);
}

// Mock pour read() qui simule une déconnexion
ssize_t __wrap_read_disconnect(int fd, void *buf, size_t count) {
    (void)fd; (void)buf; (void)count;
    return 0; // EOF = déconnexion
}

// Tests pour get_message avec déconnexion
Test(parse_command, test_get_message_disconnect) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(AI);
    
    // Cette fonction va appeler le mock qui retourne 0 (déconnexion)
    get_message(server, client);
    
    free(client->player->team_name);
    free(client->player);
    free(client->client_poll);
    free(client);
    free(server->parsed_info);
    free(server);
}

// Tests pour différentes commandes spécifiques
Test(parse_command, test_various_commands) {
    command_data_t data = get_command_data();
    
    // Test des commandes AI
    cr_assert_str_eq(data.commands[0], "Forward");
    cr_assert_str_eq(data.commands[3], "Inventory");
    cr_assert_str_eq(data.commands[4], "Look");
    cr_assert_str_eq(data.commands[9], "Incantation");
    cr_assert_str_eq(data.commands[10], "Fork");
    cr_assert_str_eq(data.commands[11], "Broadcast");
    
    // Test des commandes graphiques
    cr_assert_str_eq(data.commands[12], "msz");
    cr_assert_str_eq(data.commands[13], "bct");
    cr_assert_str_eq(data.commands[14], "mtc");
    cr_assert_str_eq(data.commands[15], "tna");
}

// Tests pour les temps d'exécution
Test(parse_command, test_command_times) {
    command_data_t data = get_command_data();
    
    cr_assert_eq(data.times[0], 7);  // Forward = 7 ticks
    cr_assert_eq(data.times[3], 1);  // Inventory = 1 tick
    cr_assert_eq(data.times[9], 300); // Incantation = 300 ticks
    cr_assert_eq(data.times[10], 42); // Fork = 42 ticks
    cr_assert_eq(data.times[12], 0);  // msz = 0 ticks (graphique)
}

// Tests pour les types de clients acceptés
Test(parse_command, test_accepted_types) {
    command_data_t data = get_command_data();
    
    cr_assert_eq(data.accepted_types[0], AI);  // Forward = AI seulement
    cr_assert_eq(data.accepted_types[12], GRAPHICAL); // msz = GRAPHICAL seulement
}