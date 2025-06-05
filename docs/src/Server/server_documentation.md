# Documentation du Serveur Zappy

## Vue d'ensemble

Le serveur Zappy est le composant central du projet, gérant la logique du jeu, la communication entre les clients (IA) et l'interface graphique. Il est responsable de :
- La gestion de la carte et des ressources
- La gestion des joueurs et des équipes
- Le traitement des commandes des IA
- La communication avec l'interface graphique

## Architecture

Le serveur est structuré en plusieurs modules :
- **Réseau** : Gestion des connexions, envoi/réception de messages
- **Carte** : Gestion de la carte 2D et des ressources
- **Joueur** : Gestion des états, inventaires et actions des joueurs
- **Équipe** : Gestion des équipes et de leurs slots disponibles
- **Commandes** : Traitement des requêtes des clients

## Protocole de Communication

Le serveur utilise un protocole texte simple pour communiquer avec les clients et l'interface graphique. Les commandes sont envoyées sous forme de chaînes de caractères terminées par `\n`.

### Commandes des IA
- `Forward` : Avancer d'une case
- `Right` : Tourner à droite
- `Left` : Tourner à gauche
- `Look` : Observer les cases visibles
- `Inventory` : Consulter l'inventaire
- `Broadcast text` : Envoyer un message
- `Connect_nbr` : Nombre de slots disponibles dans l'équipe
- `Fork` : Créer un nouvel œuf
- `Eject` : Éjecter les joueurs d'une case
- `Take object` : Ramasser un objet
- `Set object` : Poser un objet
- `Incantation` : Démarrer une incantation

### Protocole Graphique
Le serveur communique avec l'interface graphique via un protocole spécifique détaillé dans la documentation "GUI Protocol".

## Compilation et Exécution

```bash
# Compilation
make -C src/Server

# Exécution
./zappy_server -p <port> -x <width> -y <height> -n <team_names> -c <clients_per_team> -f <freq>
```

## Fichiers importants
- `main.c` : Point d'entrée du programme
- `parsing.c` : Analyse des arguments de la ligne de commande
- `network/*.c` : Gestion des connexions réseau
- `map/*.c` : Gestion de la carte et des ressources
- `player/*.c` : Gestion des joueurs et des commandes
