/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Server include
*/

#ifndef SERVER_H_
    #define SERVER_H_
    #include <sys/socket.h>
    #include <stdbool.h>
    #include "client.h"
    #include "parsing.h"
    #include "tile.h"
    #include "egg.h"

/**
 * @brief Structure de gestion du polling non-bloquant
 *
 * Cette structure gère les descripteurs de fichiers pour le polling
 * et optimise les performances en évitant les reconstructions inutiles.
 */
typedef struct poll_manager_s {
    struct pollfd *fds;         ///< Tableau des descripteurs de fichiers
    int capacity;               ///< Capacité du tableau
    bool needs_rebuild; /// Flag indiquant si une reconstruction est nécessaire
} poll_manager_t;

/**
 * @brief Structure principale du serveur Zappy
 *
 * Cette structure contient tous les éléments nécessaires au fonctionnement
 * du serveur : connexions clients, état du jeu, configuration, etc.
 */
typedef struct server_s {
    int nfds;   ///< Nombre de descripteurs de fichiers actifs
    int s_fd;      ///< Socket d'écoute du serveur
    struct sockaddr_in *serv_add;    ///< Adresse du serveur
    client_t *client;    ///< Liste chaînée des clients IA connectés
    graphical_client_t *graphical_clients; ///< Liste des clients graphiques
    egg_t *eggs;   ///< Liste des œufs présents sur la carte
    tile_t **map;     ///< Carte du jeu (tableau 2D de tiles)
    int current_tick;      ///< Tick actuel du serveur pour la synchronisation
    parsing_info_t *parsed_info;  ///< Informations de configuration du serveur
    int *total_resources;   ///< Nombre total de chaque type de ressource
    int *current_resources;   ///< Nombre actuel de chaque type de ressource
    int should_run; ///< Flag de contrôle de la boucle principale
    poll_manager_t *poll_manager; ///< Gestionnaire de polling
} server_t;

/**
 * @brief Met à jour le tick de jeu et traite les actions en attente
 * @param server Pointeur vers la structure du serveur
 */
void update_game_tick(server_t *server);

/**
 * @brief Finalise une incantation en cours
 * @param server Pointeur vers la structure du serveur
 * @param client Client effectuant l'incantation
 */
void finish_incantation(server_t *server, client_t *client);

/**
 * @brief Affiche l'aide d'utilisation du serveur
 */
void display_help(void);

/**
 * @brief Supprime un descripteur de fichier du gestionnaire de polling
 * @param server Pointeur vers la structure du serveur
 * @param fd Descripteur de fichier à supprimer
 */
void remove_fd(server_t *server, int fd);

/**
 * @brief Ajoute un descripteur de fichier au gestionnaire de polling
 * @param server Pointeur vers la structure du serveur
 * @param fd Descripteur de fichier à ajouter
 */
void add_fd(server_t *server, int fd);

/**
 * @brief Initialise et configure le serveur
 * @param server Pointeur vers la structure du serveur à initialiser
 * @param parsed_info Informations de configuration parsées
 */
void create_server(server_t *server, parsing_info_t *parsed_info);

/**
 * @brief Affiche un message d'erreur et termine le programme
 * @param msg Message d'erreur à afficher
 */
void server_err(char *msg);

/**
 * @brief Affiche les informations de connexion d'un nouveau client
 * @param client_ip Adresse IP du client
 * @param client_addr Structure d'adresse du client
 * @param new_client Structure du nouveau client
 */
void print_co(char *client_ip, struct sockaddr_in *client_addr,
    client_t *new_client);

/**
 * @brief Vérifie et traite les nouvelles connexions clients
 * @param server Pointeur vers la structure du serveur
 */
void check_client(server_t *server);

/**
 * @brief Initialise et génère la carte de jeu
 * @param server Pointeur vers la structure du serveur
 * @param parsed_info Informations de configuration pour la carte
 */
void create_map(server_t *server, parsing_info_t *parsed_info);

/**
 * @brief Initialise la position d'un nouveau joueur sur la carte
 * @param server Pointeur vers la structure du serveur
 * @param new_client Client dont le joueur doit être positionné
 */
void init_new_player_pos(server_t *server, client_t *new_client);

/**
 * @brief Traite la prochaine commande en file d'attente pour un client
 * @param server Pointeur vers la structure du serveur
 * @param client Client dont la commande doit être traitée
 */
void process_next_queued_command(server_t *server, client_t *client);

/**
 * @brief Ajoute une commande à la file d'attente d'un client
 * @param server Pointeur vers la structure du serveur
 * @param client Client destinataire de la commande
 * @param command Commande à ajouter à la file
 */
void add_to_command_queue(server_t *server, client_t *client, char *command);

/**
 * @brief Libère la mémoire d'un nœud client et le supprime du serveur
 * @param node Nœud client à supprimer
 * @param server Pointeur vers la structure du serveur
 */
void free_node(client_t *node, server_t *server);

/**
 * @brief Initialise les œufs de départ sur le serveur
 * @param n_server Pointeur vers la structure du serveur
 */
void init_server_eggs(server_t *n_server);

/**
 * @brief Libère toute la mémoire allouée par le serveur
 * @param server Pointeur vers la structure du serveur
 * @param parsed_info Informations de configuration à libérer
 */
void free_all(server_t *server, parsing_info_t *parsed_info);

/**
 * @brief Compte le nombre d'équipes configurées
 * @param n_server Pointeur vers la structure du serveur
 * @return Nombre d'équipes
 */
int count_team(server_t *n_server);

#endif /* !SERVER_H_ */
