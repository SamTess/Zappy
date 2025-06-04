#!/usr/bin/env python3
# test_network_multiple_clients.py
"""
Test fonctionnel : plusieurs clients GUI se connectent au serveur de référence
"""
import subprocess
import time
import socket
import os

SERVER_BIN = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../zappy_server_ref'))
GUI_BIN = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../zappy_gui'))
PORT = 4244

C_GREEN = "\033[32m"
C_RED = "\033[31m"
C_YELLOW = "\033[33m"
C_CYAN = "\033[36m"
C_RESET = "\033[0m"

print(f"{C_CYAN}[INFO]{C_RESET} Lancement du serveur de référence (zappy_server_ref) sur le port {PORT}...")
server_proc = subprocess.Popen([
    SERVER_BIN,
    '-p', str(PORT),
    '-x', '10',
    '-y', '10',
    '-n', 'team1',
    '-c', '5',
    '-f', '100'
], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

print(f"{C_CYAN}[INFO]{C_RESET} Attente du démarrage du serveur...")
time.sleep(1)

clients = []
try:
    print(f"{C_CYAN}[INFO]{C_RESET} Lancement de 3 clients GUI...")
    for i in range(3):
        print(f"{C_CYAN}[INFO]{C_RESET} Lancement du client GUI n°{i+1}...")
        proc = subprocess.Popen([
            GUI_BIN,
            '-p', str(PORT),
            '-h', '127.0.0.1'
        ], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        clients.append(proc)
        time.sleep(0.5)
    print(f"{C_CYAN}[INFO]{C_RESET} Vérification que tous les clients sont connectés...")
    for idx, proc in enumerate(clients):
        if proc.poll() is not None:
            print(f"{C_RED}[ERREUR]{C_RESET} Le client GUI n°{idx+1} s'est arrêté prématurément !")
            raise AssertionError("Un client s'est arrêté prématurément")
    print(f"{C_GREEN}[OK]{C_RESET} Tous les clients sont connectés.")
finally:
    print(f"{C_CYAN}[INFO]{C_RESET} Arrêt des clients GUI...")
    for idx, proc in enumerate(clients):
        proc.terminate()
        try:
            proc.wait(timeout=5)
            print(f"{C_GREEN}[OK]{C_RESET} Client GUI n°{idx+1} arrêté.")
        except subprocess.TimeoutExpired:
            print(f"{C_YELLOW}[WARN]{C_RESET} Arrêt forcé du client GUI n°{idx+1}.")
            proc.kill()
    print(f"{C_CYAN}[INFO]{C_RESET} Arrêt du serveur de référence...")
    server_proc.terminate()
    try:
        server_proc.wait(timeout=5)
        print(f"{C_GREEN}[OK]{C_RESET} Serveur arrêté.")
    except subprocess.TimeoutExpired:
        print(f"{C_YELLOW}[WARN]{C_RESET} Arrêt forcé du serveur.")
        server_proc.kill()

print(f"{C_GREEN}[OK]{C_RESET} Connexion simultanée de plusieurs clients GUI fonctionnelle.")
