#!/usr/bin/env python3
# test_network_timeout.py
"""
Test fonctionnel : comportement de zappy_gui si le serveur n'envoie pas WELCOME (timeout)
"""
import subprocess
import time
import socket
import os
import threading

GUI_BIN = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../zappy_gui'))
PORT = 4245

# Couleurs ANSI pour les messages
C_GREEN = "\033[32m"
C_RED = "\033[31m"
C_YELLOW = "\033[33m"
C_CYAN = "\033[36m"
C_RESET = "\033[0m"

# Serveur mock qui n'envoie jamais WELCOME
class MockTimeoutServer(threading.Thread):
    def __init__(self, port):
        super().__init__()
        self.port = port
        self.sock = None
        self.client_sock = None
        self.daemon = True

    def run(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(('127.0.0.1', self.port))
        self.sock.listen(1)
        self.client_sock, _ = self.sock.accept()
        # Ne rien envoyer, simuler un serveur muet
        time.sleep(5)
        self.client_sock.close()
        self.sock.close()

print(f"{C_CYAN}[INFO]{C_RESET} Lancement du serveur mock muet sur le port {PORT}...")
server = MockTimeoutServer(PORT)
server.start()

print(f"{C_CYAN}[INFO]{C_RESET} Attente du démarrage du serveur mock...")
time.sleep(0.5)

print(f"{C_CYAN}[INFO]{C_RESET} Lancement du client GUI (zappy_gui)...")
gui_proc = subprocess.Popen([
    GUI_BIN,
    '-p', str(PORT),
    '-h', '127.0.0.1'
], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

print(f"{C_CYAN}[INFO]{C_RESET} Attente du timeout côté client...")
time.sleep(6)

if gui_proc.poll() is not None:
    if gui_proc.returncode == -11:
        print(f"{C_RED}[ERREUR]{C_RESET} zappy_gui a crashé (segfault) lors d'un timeout réseau !")
    else:
        print(f"{C_GREEN}[OK]{C_RESET} Timeout réseau géré sans crash (processus arrêté)")
    assert gui_proc.returncode != -11, "zappy_gui a crashé (segfault) lors d'un timeout réseau"
else:
    print(f"{C_GREEN}[OK]{C_RESET} Timeout réseau géré sans crash (processus encore vivant)")
    gui_proc.terminate()
    gui_proc.wait(timeout=5)
