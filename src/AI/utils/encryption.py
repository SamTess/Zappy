import os
import base64
import hashlib
from cryptography.fernet import Fernet

try:
  from dotenv import load_dotenv
  load_dotenv()
except ImportError:
  print("Warning: 'python-dotenv' module not found. Environment variables may not be loaded from .env file.")
except Exception as e:
  print(f"Warning: Could not load environment variables: {e}")

base_key = os.getenv("BASE_KEY", "default_base_key")
secret_key = base_key

# generates a key using fernet algo (based on the secret key)
def _get_encryption_key():
  key_hash = hashlib.sha256(secret_key.encode()).digest()
  return base64.urlsafe_b64encode(key_hash)


def encrypt_message(message):
  try:
    fernet = Fernet(_get_encryption_key())
    encrypted_data = fernet.encrypt(message.encode())
    return base64.urlsafe_b64encode(encrypted_data).decode()

  except Exception as e:
    print(f"Encryption error: {e}")
    return message


def decrypt_message(encrypted_message):
  try:
    fernet = Fernet(_get_encryption_key())
    encrypted_data = base64.urlsafe_b64decode(encrypted_message.encode())
    decrypted_data = fernet.decrypt(encrypted_data)
    return decrypted_data.decode()

  except Exception as e:
    print(f"Decryption error: {e}")
    return None
