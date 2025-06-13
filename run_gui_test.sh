#!/bin/bash

# Couleurs pour les messages
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Compilation et exécution du test d'interface utilisateur...${NC}"

# Se placer dans le bon répertoire
cd /root/Desktop/Zappy

# Vérifier que les bibliothèques sont bien compilées
if [ ! -f "plugins/libraylibcpp.so" ] || [ ! -f "plugins/libraygui.so" ]; then
    echo -e "${YELLOW}Compilation des bibliothèques...${NC}"
    make -C libs/RaylibCPP
    make -C libs/RayGUICPP
    
    # Créer le dossier plugins s'il n'existe pas
    mkdir -p plugins
    
    # Copier les bibliothèques
    cp libs/RaylibCPP/libraylibcpp.so plugins/
    cp libs/RayGUICPP/libraygui.so plugins/
fi

echo -e "${YELLOW}Compilation de la bibliothèque GUI...${NC}"
make lib-gui

echo -e "${YELLOW}Compilation du programme de test...${NC}"
g++ -std=c++17 -o zappy_gui_test src/GUI/test_gui.cpp \
    build/libgui.a \
    -Isrc -ldl -lraylib -lpthread -lGL -lm -lrt -lX11 \
    -Wl,-rpath=./plugins

# Exécuter le test si la compilation a réussi
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Compilation réussie!${NC}"
    echo -e "${YELLOW}Lancement du test d'interface utilisateur...${NC}"
    echo -e "${YELLOW}Touches de test disponibles:${NC}"
    echo -e "  ${GREEN}T${NC} - Sélectionner une tuile aléatoire"
    echo -e "  ${GREEN}B${NC} - Ajouter un broadcast de test"
    echo -e "  ${GREEN}L${NC} - Ajouter un message de log"
    echo -e "  ${GREEN}ESC${NC} - Quitter le test"
    
    # Lancement avec ajout de permissions d'exécution si nécessaire
    chmod +x ./zappy_gui_test
    ./zappy_gui_test
else
    echo -e "${RED}Erreur de compilation.${NC}"
    exit 1
fi
