#!/usr/bin/env python3
# test_network_connection.py
"""
Test fonctionnel : connexion et déconnexion réseau de zappy_gui
"""
import subprocess
import time
import socket
import os
import signal

SERVER_BIN = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../zappy_server_ref'))
GUI_BIN = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../zappy_gui'))
PORT = 4242

C_GREEN = "\033[32m"
C_RED = "\033[31m"
C_YELLOW = "\033[33m"
C_CYAN = "\033[36m"
C_RESET = "\033[0m"

print(f"{C_CYAN}[INFO]{C_RESET} Lancement du serveur de référence (zappy_server_ref)...")
# Lancer le serveur de référence en arrière-plan
server_proc = subprocess.Popen([
    SERVER_BIN,
    '-p', str(PORT),
    '-x', '10',
    '-y', '10',
    '-n', 'team1',
    '-c', '2',
    '-f', '100'
], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

print(f"{C_CYAN}[INFO]{C_RESET} Attente du démarrage du serveur...")
time.sleep(1)  # Attendre que le serveur démarre

try:
    print(f"{C_CYAN}[INFO]{C_RESET} Vérification de l'ouverture du port {PORT}...")
    # Vérifier que le port est ouvert
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(2)
    s.connect(('127.0.0.1', PORT))
    s.close()
    print(f"{C_GREEN}[OK]{C_RESET} Port ouvert, serveur prêt.")

    print(f"{C_CYAN}[INFO]{C_RESET} Lancement du client GUI (zappy_gui)...")
    # Lancer le client GUI (zappy_gui)
    gui_proc = subprocess.Popen([
        GUI_BIN,
        '-p', str(PORT),
        '-h', '127.0.0.1'
    ], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    print(f"{C_CYAN}[INFO]{C_RESET} Attente de l'établissement de la connexion...")
    # Attendre quelques secondes pour laisser la connexion s'établir
    time.sleep(3)

    if gui_proc.poll() is not None:
        print(f"{C_RED}[ERREUR]{C_RESET} zappy_gui s'est arrêté prématurément !")
        raise AssertionError("zappy_gui s'est arrêté prématurément")
    print(f"{C_GREEN}[OK]{C_RESET} zappy_gui est bien connecté au serveur.")

    print(f"{C_CYAN}[INFO]{C_RESET} Arrêt du client GUI...")
    # Arrêter le client GUI
    gui_proc.terminate()
    gui_proc.wait(timeout=5)
    print(f"{C_GREEN}[OK]{C_RESET} Client GUI arrêté.")
finally:
    print(f"{C_CYAN}[INFO]{C_RESET} Arrêt du serveur de référence...")
    # Arrêter le serveur
    server_proc.terminate()
    try:
        server_proc.wait(timeout=5)
        print(f"{C_GREEN}[OK]{C_RESET} Serveur arrêté.")
    except subprocess.TimeoutExpired:
        print(f"{C_YELLOW}[WARN]{C_RESET} Arrêt forcé du serveur.")
        server_proc.kill()

print(f"{C_GREEN}[OK]{C_RESET} Connexion et déconnexion réseau fonctionnelles.")
