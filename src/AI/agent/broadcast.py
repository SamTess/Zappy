import os
import base64
import hashlib
from cryptography.fernet import Fernet

try:
    from dotenv import load_dotenv
    load_dotenv()
except (ImportError, AssertionError):
    pass

secret_key = os.getenv("SECRET_KEY", "default_secret_key")

# generates a key using fernet algo, based on the secret key
def _get_encryption_key():
    key_hash = hashlib.sha256(secret_key.encode()).digest()
    return base64.urlsafe_b64encode(key_hash)

def _encrypt_message(message):
    try:
        fernet = Fernet(_get_encryption_key())
        encrypted_data = fernet.encrypt(message.encode())
        return base64.urlsafe_b64encode(encrypted_data).decode()
    except Exception as e:
        print(f"Encryption error: {e}")
        return message

def _decrypt_message(encrypted_message):
    try:
        fernet = Fernet(_get_encryption_key())
        encrypted_data = base64.urlsafe_b64decode(encrypted_message.encode())
        decrypted_data = fernet.decrypt(encrypted_data)
        return decrypted_data.decode()
    except Exception as e:
        print(f"Decryption error: {e}")
        return encrypted_message

# handles the messages sent by other agents in the game
def handle_message(self, message):
    decrypted_message = _decrypt_message(message)
    return decrypted_message

# broadcasts a message to all agents in the game
def send_message(self, message):
    encrypted_message = _encrypt_message(message)
    return encrypted_message
