class CharCircularBuffer:
    def __init__(self, size):
        self.size = size
        self.buffer = [''] * size
        self.head = 0
        self.count = 0


    # Écrit les caractères dans le buffer circulaire
    def write(self, chars):
        for char in chars:
            self.buffer[self.head] = char
            self.head = (self.head + 1) % self.size
            if self.count < self.size:
                self.count += 1

    # Lit le buffer jusqu'à un \n ou jusqu'à la fin du buffer
    def read(self):
        if self.count == 0:
            return None
        result = ""
        chars_read = 0
        start_pos = (self.head - self.count) % self.size
        for i in range(self.count):
            pos = (start_pos + i) % self.size
            char = self.buffer[pos]
            result += char
            chars_read += 1
            if char == '\n':
                self.count -= chars_read
                return result[:-1]
        return None

    # Vérifie s'il y a un \n dans le buffer
    def has_newline(self):
        start_pos = (self.head - self.count) % self.size
        for i in range(self.count):
            pos = (start_pos + i) % self.size
            if self.buffer[pos] == '\n':
                return True
        return False