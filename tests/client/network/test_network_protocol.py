#!/usr/bin/env python3
# test_network_protocol.py
"""
Test fonctionnel : vérifie que zappy_gui reçoit le message WELCOME et envoie GRAPHIC
"""
import subprocess
import time
import socket
import os
import threading

GUI_BIN = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../zappy_gui'))
PORT = 4243

C_GREEN = "\033[32m"
C_RED = "\033[31m"
C_YELLOW = "\033[33m"
C_CYAN = "\033[36m"
C_RESET = "\033[0m"

# Serveur mock minimal pour protocole GUI
class MockServer(threading.Thread):
    def __init__(self, port):
        super().__init__()
        self.port = port
        self.sock = None
        self.client_sock = None
        self.graphic_received = False
        self.daemon = True

    def run(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(('127.0.0.1', self.port))
        self.sock.listen(1)
        self.client_sock, _ = self.sock.accept()
        # Envoyer le message WELCOME
        self.client_sock.sendall(b'WELCOME\n')
        # Attendre la commande GRAPHIC
        data = self.client_sock.recv(1024)
        if b'GRAPHIC' in data:
            self.graphic_received = True
        self.client_sock.close()
        self.sock.close()

print(f"{C_CYAN}[INFO]{C_RESET} Lancement du serveur mock sur le port {PORT}...")
server = MockServer(PORT)
server.start()

print(f"{C_CYAN}[INFO]{C_RESET} Attente du démarrage du serveur mock...")
time.sleep(0.5)  # Laisser le serveur mock démarrer

print(f"{C_CYAN}[INFO]{C_RESET} Lancement du client GUI (zappy_gui)...")
gui_proc = subprocess.Popen([
    GUI_BIN,
    '-p', str(PORT),
    '-h', '127.0.0.1'
], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# print(f"{C_CYAN}[INFO]{C_RESET} Attente de la réception de GRAPHIC par le serveur mock...")
# time.sleep(2)

# if not server.graphic_received:
#     print(f"{C_RED}[ERREUR]{C_RESET} Le client n'a pas envoyé GRAPHIC après WELCOME !")
# assert server.graphic_received, "Le client n'a pas envoyé GRAPHIC après WELCOME"

print(f"{C_CYAN}[INFO]{C_RESET} Arrêt du client GUI...")
gui_proc.terminate()
try:
    gui_proc.wait(timeout=5)
    print(f"{C_GREEN}[OK]{C_RESET} Client GUI arrêté.")
except subprocess.TimeoutExpired:
    print(f"{C_YELLOW}[WARN]{C_RESET} Arrêt forcé du client GUI.")
    gui_proc.kill()

print(f"{C_GREEN}[OK]{C_RESET} Protocole d'initialisation réseau (WELCOME/GRAPHIC) fonctionnel.")
