# Protocole de communication Serveur-IA

## Vue d'ensemble

Ce document détaille le protocole de communication entre le serveur Zappy et les clients d'intelligence artificielle (IA). Le protocole est basé sur des échanges textuels avec des commandes spécifiques et leurs réponses associées.

## Connexion

1. L'IA se connecte au serveur via TCP/IP
2. Le serveur répond avec : `WELCOME\n`
3. L'IA envoie son nom d'équipe : `<team_name>\n`
4. Le serveur répond avec :
   - `<client_num>\n` : Numéro du client dans l'équipe
   - `<X> <Y>\n` : Dimensions de la carte

## Format des commandes

- Chaque commande est une chaîne se terminant par `\n`
- Les réponses du serveur se terminent aussi par `\n`
- Chaque commande a un temps d'exécution spécifique
- Les commandes sont mises en file d'attente si le joueur est occupé

## Commandes disponibles

### Commandes de base

| Commande | Description | Résultat | Durée (unité de temps serveur) |
|----------|-------------|----------|-------------------------------|
| `Forward` | Avancer d'une case | `ok\n` | 7 |
| `Right` | Tourner à droite | `ok\n` | 7 |
| `Left` | Tourner à gauche | `ok\n` | 7 |
| `Look` | Observer l'environnement | Liste des objets visibles | 7 |
| `Inventory` | Vérifier l'inventaire | Liste des objets possédés | 1 |
| `Broadcast <text>` | Communiquer avec les autres joueurs | `ok\n` | 7 |
| `Connect_nbr` | Nombre de connexions disponibles | Nombre entier | 0 |
| `Fork` | Créer un nouvel œuf | `ok\n` | 42 |
| `Eject` | Éjecter les joueurs de la case | `ok\n` ou `ko\n` | 7 |
| `Take <object>` | Ramasser un objet | `ok\n` ou `ko\n` | 7 |
| `Set <object>` | Poser un objet | `ok\n` ou `ko\n` | 7 |
| `Incantation` | Lancer une incantation | Succès ou échec de l'incantation | 300 |

### Objets manipulables

- `food` : Nourriture (consommée automatiquement : 1 unité/126 ticks)
- `linemate` : Pierre précieuse niveau 1
- `deraumere` : Pierre précieuse niveau 2
- `sibur` : Pierre précieuse niveau 3
- `mendiane` : Pierre précieuse niveau 4
- `phiras` : Pierre précieuse niveau 5
- `thystame` : Pierre précieuse niveau 6

## Format des réponses détaillées

### Look
La commande `Look` retourne une vue de l'environnement sous forme de cases concentriques :
```
[<object1> <object2> ...],
[<object1> <object2> ...],
[<object1> <object2> ...],
...
```

**Objets visibles :** `player`, `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`, `egg`

**Organisation de la vue :**
- Case 0 : Position actuelle du joueur
- Cases 1-2 : Cases directement devant (selon l'orientation)
- Cases 3-8 : Cases de niveau 2 (en éventail)
- Cases 9-15 : Cases de niveau 3, etc.

### Inventory
```
{food <n>, linemate <n>, deraumere <n>, sibur <n>, mendiane <n>, phiras <n>, thystame <n>}
```

### Broadcast
- **Émission** : Le joueur émetteur reçoit `ok\n`
- **Réception** : Les autres joueurs reçoivent `message <direction>, <message>\n`
- **Direction** : Nombre de 0 à 8 représentant la direction du message
  - 0 : Même case que l'émetteur
  - 1-8 : Directions cardinales et diagonales

### Incantation
1. **Démarrage** : Serveur répond immédiatement `Elevation underway\n`
2. **Vérification** : Le serveur vérifie les conditions (ressources + joueurs)
3. **Résultat** :
   - **Succès** : `Current level: <level>\n` (nouveau niveau)
   - **Échec** : `ko\n` (conditions non remplies)

## Conditions d'élévation détaillées

| Niveau | Joueurs requis | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|--------|----------------|----------|-----------|-------|----------|--------|----------|
| 1→2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2→3 | 1 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3→4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4→5 | 2 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5→6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6→7 | 4 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7→8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

**Notes importantes :**
- Tous les joueurs sur la case doivent être du même niveau pour l'incantation
- Les ressources sont consommées lors de l'incantation réussie
- L'incantation peut échouer si les conditions ne sont plus remplies au moment de l'exécution

## Gestion des erreurs et cas spéciaux

### Commandes invalides
- **Commande inconnue** : `ko\n`
- **Paramètre invalide** : `ko\n`
- **Action impossible** : `ko\n` (ex: prendre un objet qui n'existe pas)

### Mort du joueur
- **Cause** : Manque de nourriture (inventaire de food à 0)
- **Comportement** : Le serveur ferme la connexion sans notification

### Éjection
- **Mécanisme** : La commande `Eject` pousse tous les autres joueurs de la case
- **Direction** : Les joueurs éjectés sont déplacés dans la direction de l'éjecteur
- **Notification** : Les joueurs éjectés reçoivent `eject: <direction>\n`

## Orientation et déplacements

### Système d'orientation
- **0** : Nord (haut de la carte)
- **1** : Est (droite de la carte)
- **2** : Sud (bas de la carte)
- **3** : Ouest (gauche de la carte)

### Carte torique
- La carte est torique (les bords se rejoignent)
- Sortir par un bord fait apparaître de l'autre côté
- Les coordonnées sont calculées en modulo des dimensions

## Exemple d'échange complet

```
CLIENT: <connexion TCP>
SERVER: WELCOME\n
CLIENT: team1\n
SERVER: 0\n
SERVER: 10 10\n
CLIENT: Look\n
SERVER: [player food],[linemate],[food sibur],[],[],[],[],[]\n
CLIENT: Forward\n
SERVER: ok\n
CLIENT: Take linemate\n
SERVER: ok\n
CLIENT: Inventory\n
SERVER: {food 9, linemate 1, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0}\n
CLIENT: Broadcast Hello team!\n
SERVER: ok\n
CLIENT: Incantation\n
SERVER: Elevation underway\n
SERVER: Current level: 2\n
```

## Considérations techniques

### Performance
- **Fréquence du serveur** : Configurable via le paramètre `-f`
- **Files d'attente** : Les commandes sont mises en file si le joueur est occupé
- **Timeout** : Aucun timeout côté serveur, mais les clients peuvent implémenter leurs propres timeouts

### Multithreading
- **Thread-safety** : Le serveur gère les accès concurrents
- **Ordre des commandes** : Les commandes sont traitées dans l'ordre d'arrivée par joueur
- **Synchronisation** : Les événements (incantations, éjections) sont synchronisés entre tous les clients
