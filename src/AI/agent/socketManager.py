import threading
import queue
import socket
import uuid
import time

class SocketManager:
  def __init__(self, sock):
    self.sock = sock
    self.message_queue = queue.Queue()
    self.pending_requests = {}
    self.running = False
    self.thread = None
    self.buffer = ""
    self.lock = threading.Lock()

  def start(self):
    self.running = True
    self.thread = threading.Thread(target=self._listen_loop, daemon=True)
    self.thread.start()

  def stop(self):
    self.running = False
    if self.thread:
      self.thread.join()

# fonction bloquante
  def _read_line(self):
    while '\n' not in self.buffer:
      try:
        data = self.sock.recv(1024).decode('utf-8')
        if not data:
          return None
        self.buffer += data
      except socket.error:
        return None
    line, self.buffer = self.buffer.split('\n', 1)
    return line.strip()

# détermine si le message est une réponse à une requête ou un message du serveur
  def _handle_message(self, message):
    with self.lock:
      # si il y a des requêtes en attente on considère que c'est une réponse
      if self.pending_requests:
        request_id = next(iter(self.pending_requests))
        response_queue = self.pending_requests.pop(request_id)
        response_queue.put(message)
      else:
        # Aucune requête en attente, c'est un message non sollicité
        self.message_queue.put(message)

  def _listen_loop(self):
    while self.running:
      try:
        message = self._read_line()
        if message is not None and message != "":
          self._handle_message(message)
        elif message is None:
          break
      except Exception as e:
        print(f"Communication error: {e}")
        break

# envoie une commande et attend une réponse
  def send_command(self, command, timeout=5.0):
    if command is None:
      print("Command is None, not sending.")
      return None

    request_id = str(uuid.uuid4())
    response_queue = queue.Queue()

    with self.lock:
      self.pending_requests[request_id] = response_queue

    try:
      self.sock.send(f"{command}\n".encode('utf-8'))
      try:
        response = response_queue.get(timeout=timeout)
        return response
      except queue.Empty:
        with self.lock:
          self.pending_requests.pop(request_id, None)
        print(f"Response to command '{command}' timed out.")
        return None
    except socket.error as e:
      with self.lock:
        self.pending_requests.pop(request_id, None)
      print(f"Error sending command '{command}': {e}")
      return None

  def get_message(self, timeout=None):
    try:
      return self.message_queue.get(timeout=timeout)
    except queue.Empty:
      return None

  def has_messages(self):
    return not self.message_queue.empty()
