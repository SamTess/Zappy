---
sidebar_position: 1
---

# Interface graphique (GUI)

L'interface graphique permet de visualiser en temps réel l'état du jeu Zappy, les joueurs, les ressources et les actions.

## Fonctionnalités

- Affichage de la carte de jeu en 2D ou 3D
- Visualisation des joueurs et de leur orientation
- Affichage des ressources présentes sur chaque case
- Visualisation des actions des joueurs (mouvement, prise d'objet, etc.)
- Interface pour surveiller l'état des équipes
- Visualisation des incantations
- Contrôles pour ajuster la vitesse de simulation

## Configuration

Le client GUI accepte les paramètres suivants :

| Paramètre | Description                    | Valeur par défaut |
|-----------|--------------------------------|-------------------|
| `-p`      | Numéro de port du serveur      | 4242              |
| `-h`      | Adresse du serveur             | localhost         |

## Protocole de communication

Le client GUI se connecte au serveur et reçoit des informations sur l'état du jeu. Il utilise un protocole texte sur TCP. Le client GUI ne peut pas envoyer de commandes qui affectent le jeu, il est uniquement un observateur.

## Interface utilisateur

L'interface utilisateur comprend plusieurs sections :

1. **Vue de la carte** : Affiche la grille de jeu avec les joueurs et les ressources
2. **Panneau d'information** : Affiche des détails sur l'élément sélectionné
3. **Contrôles de visualisation** : Permettent de zoomer, faire pivoter et déplacer la vue
4. **Liste des équipes** : Affiche les équipes et leurs statistiques
5. **Timeline** : Permet de suivre l'historique des événements

## Développement

Le client GUI est implémenté en C++ et utilise une bibliothèque graphique pour le rendu.
