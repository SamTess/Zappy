import os
from dotenv import load_dotenv

load_dotenv()

secret_key = os.getenv("SECRET_KEY", "default_secret_key")

# handles the messages sent by other agents in the game
def handle_broadcast_messages(message):
    decrypted_message = message  # Replace with actual decryption logic if needed


# broadcasts a message to all agents in the game
def send_broadcast_message(sock, message):
    print("secret_key", secret_key)
    encrypted_message = message  # Replace with actual encryption logic if needed
