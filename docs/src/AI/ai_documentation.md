# Documentation de l'IA Zappy

## Vue d'ensemble

L'IA Zappy est implémentée en Python et représente les joueurs virtuels qui évoluent dans l'environnement du jeu. Chaque IA est indépendante et prend des décisions basées sur :
- Son état actuel (niveau, inventaire)
- Sa perception de l'environnement
- Les objectifs à atteindre pour monter de niveau

## Architecture

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

## États et Transitions

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

## Utilisation

```bash
# Exécution
./zappy_ai -p <port> -n <team_name> -h <host>
```

## Fichiers importants
- `main.py` : Point d'entrée du programme
- `agent/agent.py` : Classe principale de l'agent IA
- `defs/zappy.py` : Définitions des constantes du jeu
- `utils/*.py` : Fonctions utilitaires
- `logger/logger.py` : Système de journalisation
