---
sidebar_position: 1
---

# Client IA

Le client IA représente un joueur automatisé qui se connecte au serveur et prend des décisions pour jouer au jeu de façon autonome.

## Fonctionnalités

- Connexion au serveur Zappy
- Analyse de l'environnement de jeu
- Prise de décision basée sur l'état du jeu et les objectifs
- Gestion des ressources
- Collaboration avec d'autres IA de la même équipe

## Architecture d'une IA

Chaque client IA suit généralement une architecture basée sur des agents qui comprend :

1. **Module de perception** : Interprète les informations reçues du serveur
2. **Module de décision** : Détermine les actions à entreprendre
3. **Module d'action** : Exécute les actions choisies
4. **Gestionnaire d'état** : Maintient une représentation interne de l'état du jeu

## Stratégies d'IA

Les IA peuvent mettre en œuvre diverses stratégies :

- **Collecte de ressources** : Recherche et collecte des ressources nécessaires
- **Élévation de niveau** : Réalisation des incantations pour monter de niveau
- **Exploration** : Cartographie de l'environnement
- **Communication** : Coordination avec les autres membres de l'équipe

## Développement

Pour développer une IA efficace, vous devez comprendre :

1. Le protocole de communication avec le serveur
2. Les règles du jeu et les mécanismes
3. Les algorithmes de prise de décision
4. Les structures de données efficaces pour représenter l'état du jeu

## Exemple simplifié

Voici un exemple simplifié du cycle de vie d'une IA :

```
1. Se connecter au serveur
2. Rejoindre une équipe
3. Boucle principale :
   a. Observer l'environnement (Look)
   b. Vérifier l'inventaire
   c. Prendre une décision basée sur les besoins
   d. Exécuter des actions (se déplacer, prendre des objets, etc.)
   e. Communiquer avec les autres joueurs si nécessaire
```

## Test et optimisation

Pour améliorer votre IA :

- Testez contre différentes stratégies
- Analysez les performances dans différentes situations
- Optimisez la prise de décision
- Ajustez les priorités des actions

## Structure du code

Le code de l'IA est structuré en plusieurs modules :
- **Agent** : Gestion de la logique décisionnelle de l'IA
- **Parser** : Analyse des messages reçus du serveur
- **Logger** : Journalisation des actions et événements
- **Utils** : Fonctions utilitaires partagées
- **Defs** : Définitions et constantes du jeu

## Logique de l'IA

L'IA suit une machine à états hiérarchique permettant de :
1. Explorer la carte pour trouver des ressources
2. Collecter les ressources nécessaires pour l'incantation
3. Rechercher d'autres joueurs de son équipe pour l'incantation
4. Réaliser des incantations pour monter de niveau
5. Coordonner ses actions avec les autres joueurs via le broadcast

## États et transitions

L'agent possède plusieurs états principaux :
- **Exploration** : Recherche de nourriture et ressources
- **Collecte** : Ramassage des ressources identifiées
- **Regroupement** : Recherche d'autres joueurs pour incantation
- **Incantation** : Exécution du rituel de montée de niveau
- **SOS** : État d'urgence quand la nourriture est faible

## Communication

Les agents communiquent entre eux via le mécanisme de broadcast du serveur en utilisant un protocole simple :
- Préfixe : Identifiant de l'équipe
- Type de message : Action, Demande, Information
- Contenu : Dépend du type de message

## Compilation et exécution

```bash
# Compilation (si nécessaire)
make -C src/AI

# Exécution
./zappy_ai -p <port> -n <team_name> -h <host>
```

## Fichiers importants
- `main.py` : Point d'entrée du programme
- `agent/agent.py` : Classe principale de l'agent IA
- `defs/zappy.py` : Définitions des constantes du jeu
- `utils/*.py` : Fonctions utilitaires
- `logger/logger.py` : Système de journalisation
