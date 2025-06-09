---
sidebar_position: 1
---

# Serveur Zappy

Le serveur est le composant central du projet Zappy. Il gère l'état du jeu, la logique, les règles et coordonne la communication entre les clients IA et l'interface graphique.

## Architecture du serveur

Le serveur Zappy est conçu avec une architecture modulaire :

- **Gestionnaire de réseau** : Gère les connexions client et la communication
- **Moteur de jeu** : Implémente les règles du jeu et la logique
- **Gestionnaire de ressources** : Gère la carte de jeu et les ressources
- **Gestionnaire d'équipe** : Gère les équipes et les joueurs

## Configuration du serveur

Le serveur accepte les paramètres de ligne de commande suivants :

| Paramètre | Description                                  | Valeur par défaut |
|-----------|----------------------------------------------|-------------------|
| `-p`      | Numéro de port                               | 4242              |
| `-x`      | Largeur de la carte                          | 20                |
| `-y`      | Hauteur de la carte                          | 20                |
| `-n`      | Noms des équipes (séparés par des espaces)   | -                 |
| `-c`      | Nombre maximal de clients par équipe         | 10                |
| `-f`      | Fréquence d'exécution (ticks par seconde)    | 100               |

## Protocole de communication

Le serveur communique avec les clients en utilisant un protocole texte basé sur TCP. Les commandes et réponses sont envoyées sous forme de chaînes de texte terminées par un caractère de nouvelle ligne (`\n`).

### Commandes du client IA

Le serveur accepte les commandes suivantes des clients IA :

- `Forward` : Avancer d'une case
- `Right` : Tourner à droite de 90°
- `Left` : Tourner à gauche de 90°
- `Look` : Observer l'environnement
- `Inventory` : Afficher l'inventaire
- `Broadcast text` : Diffuser un message à tous les joueurs
- `Connect_nbr` : Obtenir le nombre de places disponibles dans l'équipe
- `Fork` : Créer un nouvel œuf (nouveau joueur)
- `Eject` : Éjecter les joueurs sur la même case
- `Take object` : Prendre un objet
- `Set object` : Poser un objet
- `Incantation` : Démarrer une incantation pour monter de niveau

### Protocole GUI

Le serveur envoie des mises à jour au client GUI pour lui permettre de visualiser l'état du jeu en temps réel.

## Implémentation

Le serveur est implémenté en C et utilise les sockets POSIX pour la communication réseau et un modèle de programmation événementiel pour gérer plusieurs clients simultanément.

## Structure des fichiers

Le code du serveur est organisé comme suit :
- `main.c` : Point d'entrée du programme
- `parsing.c` : Analyse des arguments de la ligne de commande
- `network/*.c` : Gestion des connexions réseau
- `map/*.c` : Gestion de la carte et des ressources
- `player/*.c` : Gestion des joueurs et des commandes

## Compilation et Exécution

```bash
# Compilation
make -C src/Server

# Exécution
./zappy_server -p <port> -x <width> -y <height> -n <team_names> -c <clients_per_team> -f <freq>
```

## Mécanismes importants

### Gestion des ressources
Le serveur génère aléatoirement les ressources sur la carte et les renouvelle à intervalles réguliers en fonction de la fréquence configurée.

### Gestion du temps
Toutes les actions dans le jeu sont mesurées en unités temporelles. Le serveur utilise un système de file d'attente pour gérer les actions qui doivent être exécutées après un certain délai.

### Montée de niveau
Le jeu comprend un système de montée de niveau basé sur des rituels d'incantation qui nécessitent :
- Un nombre spécifique de joueurs
- Des combinaisons précises de ressources
- Tous les joueurs doivent être au même niveau
