import sys
import termios
import tty
import select

def read_line(sock):
    line = b""
    while True:
        ready, _, _ = select.select([sock], [], [], 0)
        if not ready:
            break

        char = sock.recv(1)
        if not char:
            break
        line += char
        if char == b'\n':
            break
    return line.decode('utf-8').strip()

def read_line_blocking(sock):
    line = b""
    while True:
        char = sock.recv(1)
        if not char:
            break
        line += char
        if char == b'\n':
            break
    return line.decode('utf-8').strip()

def get_key():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(sys.stdin.fileno())
        if select.select([sys.stdin], [], [], 0.1) == ([sys.stdin], [], []):
            key = sys.stdin.read(1)
            if key == '\x1b':
                key += sys.stdin.read(2)
            return key
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return None
