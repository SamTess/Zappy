import threading
import queue
import socket

# lis les messages du serveur depuis un thread séparé et les met dans une queue
class SocketManager:
  def __init__(self, sock):
    self.sock = sock
    self.message_queue = queue.Queue()
    self.running = False
    self.thread = None
    self.buffer = ""

  def start(self):
    self.running = True
    self.thread = threading.Thread(target=self._listen_loop, daemon=True)
    self.thread.daemon = True
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

  def _listen_loop(self):
    while self.running:
      try:
        message = self._read_line()
        if message is not None and message != "":
          self.message_queue.put(message)
        elif message is None:
          break
      except Exception as e:
        print(f"Comunication error: {e}")
        break

  def get_message(self, timeout=None):
    try:
      return self.message_queue.get(timeout=timeout)
    except queue.Empty:
      return None

  def has_messages(self):
    return not self.message_queue.empty()

  def send_command(self, command):
    if command is not None:
      try:
        self.sock.send(f"{command}\n".encode('utf-8'))
      except socket.error as e:
        print(f"Error sending command '{command}': {e}")
    else:
      print("Command is None, not sending.")
