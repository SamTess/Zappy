#!/usr/bin/env python3
# test_network_invalid_server.py
"""
Test fonctionnel : comportement de zappy_gui lors d'une tentative de connexion à un serveur inexistant
"""
import subprocess
import time
import os

GUI_BIN = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../zappy_gui'))
PORT = 5999  # Port probablement inutilisé

C_GREEN = "\033[32m"
C_RED = "\033[31m"
C_YELLOW = "\033[33m"
C_CYAN = "\033[36m"
C_RESET = "\033[0m"

print(f"{C_CYAN}[INFO]{C_RESET} Lancement du client GUI (zappy_gui) sur le port {PORT} (aucun serveur ne doit répondre)...")
gui_proc = subprocess.Popen([
    GUI_BIN,
    '-p', str(PORT),
    '-h', '127.0.0.1'
], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

print(f"{C_CYAN}[INFO]{C_RESET} Attente de la fin du processus (échec attendu)...")
try:
    gui_proc.wait(timeout=5)
    print(f"{C_GREEN}[OK]{C_RESET} Le client GUI s'est arrêté (échec de connexion détecté).")
except subprocess.TimeoutExpired:
    print(f"{C_RED}[ERREUR]{C_RESET} Le client GUI ne s'est pas arrêté alors que le serveur est absent !")
    gui_proc.kill()
    assert False, "zappy_gui ne s'est pas arrêté alors que le serveur est absent"

if gui_proc.returncode == 0:
    print(f"{C_RED}[ERREUR]{C_RESET} Le client GUI a retourné 0 alors qu'il aurait dû échouer.")
assert gui_proc.returncode != 0, "zappy_gui aurait dû échouer à se connecter à un serveur inexistant"

print(f"{C_GREEN}[OK]{C_RESET} Gestion de l'échec de connexion réseau fonctionnelle.")
