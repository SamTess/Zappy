#!/usr/bin/env python3
import zipapp
import os
import shutil
import sys

def create_zipapp():
    app_dir = "zappy_ai_app"
    if os.path.exists(app_dir):
        shutil.rmtree(app_dir)
    os.makedirs(app_dir)
    try:
        if os.path.exists("../main.py"):
            shutil.copy("../main.py", os.path.join(app_dir, "__main__.py"))
        else:
            print("Error: main.py not found", file=sys.stderr)
            return False
        # Copier tous les dossiers Python du répertoire parent
        parent_dir = ".."
        for item in os.listdir(parent_dir):
            item_path = os.path.join(parent_dir, item)
            if os.path.isdir(item_path) and not item.startswith('.') and item != 'build_zipapp':
                dest_path = os.path.join(app_dir, item)
                shutil.copytree(item_path, dest_path)
        # Créer l'archive exécutable dans le répertoire racine de Zappy
        output_path = "../../../zappy_ai"
        zipapp.create_archive(app_dir, output_path, interpreter="/usr/bin/env python3")
        shutil.rmtree(app_dir)
        return True
    except Exception as e:
        print(f"Error creating zipapp: {e}", file=sys.stderr)
        if os.path.exists(app_dir):
            shutil.rmtree(app_dir)
        return False

if __name__ == "__main__":
    success = create_zipapp()
    sys.exit(0 if success else 1)
