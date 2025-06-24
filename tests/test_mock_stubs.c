/* Mock stubs for external functions */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void write_command_output(int fd, char *msg) { (void)fd; (void)msg; }
void command_pgt(void *s, void *c, void *r) { (void)s; (void)c; (void)r; }
void command_pdr(void *s, void *c, void *r) { (void)s; (void)c; (void)r; }
bool send_ppo_command(void *s, int id) { (void)s; (void)id; return true; }
int tile_add_player(void *t, int id) { (void)t; (void)id; return 0; }
int tile_remove_player(void *t, int id) { (void)t; (void)id; return 0; }
void send_bct_to_all_graphical_clients(void *s, int x, int y) { (void)s; (void)x; (void)y; }
int connect_nbr_srv(void *s, char *t) { (void)s; (void)t; return 42; }
