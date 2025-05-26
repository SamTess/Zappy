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
