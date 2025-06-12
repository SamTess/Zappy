##
## EPITECH PROJECT, 2025
## B-YEP-400 Zappy GUI
## File description:
## Makefile
##

CC = g++
CPPFLAGS = -std=c++17 -Wall -Wextra -g3
LDFLAGS = -ldl

# Chemins des sources
SRC_DIR = ../..
GUI_SRC = $(SRC_DIR)/GUI
SHARED_SRC = $(SRC_DIR)/Shared

# Fichiers sources
SRC = test_map_renderer.cpp \
      $(GUI_SRC)/renderer/MapRenderer.cpp \
      $(GUI_SRC)/graphicalContext/GraphicalContext.cpp \
      $(GUI_SRC)/textureManager/ModelManager.cpp \
      $(GUI_SRC)/textureManager/TextureManager.cpp \
      $(GUI_SRC)/cameraController/CameraController.cpp \
      $(GUI_SRC)/network/protocol/Message.cpp

# Fichiers objets
OBJ = $(SRC:.cpp=.o)

# En-têtes
INC = -I$(SRC_DIR)

# Nom de l'exécutable
NAME = test_map_renderer

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
