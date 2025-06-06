---
sidebar_position: 1
---

# Protocoles de communication

## Vue d'ensemble

Le projet Zappy repose sur des protocoles de communication réseau qui permettent aux différents composants d'interagir. Il existe deux protocoles principaux :

1. **Protocole Serveur-IA** : Utilisé pour la communication entre le serveur et les clients d'intelligence artificielle
2. **Protocole Serveur-GUI** : Utilisé pour la communication entre le serveur et l'interface graphique

## Caractéristiques communes

Les deux protocoles partagent certaines caractéristiques :
- Basés sur TCP/IP
- Communication par messages textuels
- Chaque message se termine par un caractère de nouvelle ligne (`\n`)
- Format de commande/réponse

## Différences principales

| Caractéristique | Protocole Serveur-IA | Protocole Serveur-GUI |
|-----------------|----------------------|------------------------|
| **Objectif** | Contrôle des joueurs | Visualisation du jeu |
| **Bidirectionnalité** | L'IA envoie des commandes, le serveur répond | Le GUI peut demander des informations, le serveur envoie aussi des notifications |
| **Impact sur le jeu** | Les commandes modifient l'état du jeu | Le GUI ne modifie pas l'état du jeu (à l'exception de la commande de vitesse) |
| **Authentification** | L'IA doit rejoindre une équipe | Le GUI s'identifie avec la commande `GRAPHIC` |

## Utilisation

Pour une documentation détaillée de chaque protocole, consultez les pages suivantes :
- [Protocole Serveur-IA](./server-ai)
- [Protocole Serveur-GUI](./server-gui)
