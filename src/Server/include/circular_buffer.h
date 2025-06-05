/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** circular_buffer
*/

#ifndef CIRCULAR_BUFFER_H_
    #define CIRCULAR_BUFFER_H_
    #define BUFFER_SIZE 8192

typedef struct circular_buffer_s {
    char buffer[BUFFER_SIZE];
    int start;
    int end;
    int count;
} circular_buffer_t;

void init_circular_buffer(circular_buffer_t *cb);
int add_to_circular_buffer(circular_buffer_t *cb, char byte);
int find_command_end(circular_buffer_t *cb);
char *extract_command(circular_buffer_t *cb, int cmd_length);

#endif /* !CIRCULAR_BUFFER_H_ */
