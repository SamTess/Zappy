// Simple test main for player death/starvation logic
// Compile with: gcc -o test_player_death test_player_death.c ../src/Server/player/player_death.c -I../src/Server/include -DTEST_PLAYER_DEATH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Server/include/player.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/command.h"

// --- Mocked functions and types ---
static int mock_food_in_inventory = 0;
static int food_called = 0;

void write_command_output(int fd, char *msg) {
    printf("[MOCK] write_command_output(fd=%d, msg=%s)\n", fd, msg);
}
void remove_fd(server_t *server, int fd) {
    printf("[MOCK] remove_fd(fd=%d)\n", fd);
}
int tile_remove_player(tile_t *tile, int player_id) {
    printf("[MOCK] tile_remove_player(player_id=%d)\n", player_id);
    return 0;
}
void free_inventory(player_t *player) {
    printf("[MOCK] free_inventory()\n");
}
void cleanup_player_queue(player_t *player) {
    printf("[MOCK] cleanup_player_queue()\n");
}
int how_many_in_inventory(player_t *player, resource_type_t type) {
    return mock_food_in_inventory; // Controlled by test
}
bool remove_item_from_inventory(player_t *player, resource_type_t t, int nb) {
    printf("[MOCK] remove_item_from_inventory()\n");
    food_called = 1;
    return true;
}

// --- Function prototypes from player_death.c ---
// (No need to redeclare, already in headers)

int main(void) {
    // Setup test server and client
    server_t server = {0};
    parsing_info_t info = {0};
    info.width = 10;
    info.height = 10;
    server.parsed_info = &info;
    client_t client = {0};
    client.client_fd = 42;
    client.client_id = 1;
    player_t *player = malloc(sizeof(player_t));
    player->life = 1; // Will die after one starvation check
    player->pos_x = 0;
    player->pos_y = 0;
    player->team_name = strdup("TeamA");
    client.player = player;
    printf("Before starvation: player->life = %d\n", player->life);
    mock_food_in_inventory = 0;
    food_called = 0;
    check_player_starvation(&server, &client);
    if (client.player == NULL)
        printf("Player has died and been cleaned up.\n");
    else {
        printf("Player is still alive: life = %d\n", client.player->life);
        free(player->team_name);
        free(player);
    }

    // Tricky test 1: Player with food in inventory (should eat food, not die)
    printf("\n[Tricky Test 1] Player with food in inventory\n");
    player_t *player2 = malloc(sizeof(player_t));
    player2->life = 1;
    player2->pos_x = 0;
    player2->pos_y = 0;
    player2->team_name = strdup("TeamB");
    client.player = player2;
    mock_food_in_inventory = 1;
    food_called = 0;
    check_player_starvation(&server, &client);
    if (client.player && client.player->life == 126 && food_called) {
        printf("PASS: Player ate food and survived.\n");
        free(player2->team_name);
        free(player2);
    } else {
        printf("FAIL: Player did not eat food as expected.\n");
        if (client.player) {
            free(player2->team_name);
            free(player2);
        }
    }
    mock_food_in_inventory = 0;

    // Tricky test 2: Player with life > 1 (should just decrement life)
    printf("\n[Tricky Test 2] Player with life > 1\n");
    player_t *player3 = malloc(sizeof(player_t));
    player3->life = 5;
    player3->pos_x = 0;
    player3->pos_y = 0;
    player3->team_name = strdup("TeamC");
    client.player = player3;
    check_player_starvation(&server, &client);
    if (client.player && client.player->life == 4) {
        printf("PASS: Player life decremented correctly.\n");
        free(player3->team_name);
        free(player3);
    } else {
        printf("FAIL: Player life not decremented as expected.\n");
        if (client.player) {
            free(player3->team_name);
            free(player3);
        }
    }

    // Tricky test 3: Player with invalid position (should not crash)
    printf("\n[Tricky Test 3] Player with invalid position\n");
    player_t *player4 = malloc(sizeof(player_t));
    player4->life = 1;
    player4->pos_x = 9999;
    player4->pos_y = 9999;
    player4->team_name = strdup("TeamD");
    client.player = player4;
    check_player_starvation(&server, &client);
    printf("PASS: No crash with invalid position.\n");
    if (client.player) {
        free(player4->team_name);
        free(player4);
    }

    // Tricky test 4: Null player and null client (should not crash)
    printf("\n[Tricky Test 4] Null player and null client\n");
    client.player = NULL;
    check_player_starvation(&server, &client);
    check_player_starvation(&server, NULL);
    printf("PASS: No crash with null player or client.\n");
    return 0;
}
