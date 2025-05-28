/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parsing
*/

#ifndef PARSING_H_
    #define PARSING_H_
    #include <stdbool.h>

typedef struct parsing_info_s {
    int port;
    int width;
    int height;
    char **names;
    int client_nb;
    int frequence;
} parsing_info_t;

void parse_args(int ac, char **av, parsing_info_t *parsed_info);
void parsing_error(char *error_message, parsing_info_t *parsed_info);

#endif /* !PARSING_H_ */
