##
## EPITECH PROJECT, 2025
## Zappy GUI Makefile
## File description:
## Builds the Zappy graphical interface binary
##

# Binary name
NAME = ../../zappy_gui
BUILD_DIR = ../../build/GUI

MAIN = main.cpp
# Source files
SRC = graphicalContext/GraphicalContext.cpp \
	  parsing/ParsingCLI.cpp \
	  shared/exception/AException.cpp \
	  network/buffer/MessageQueue.cpp \
	  network/connection/TcpConnection.cpp \
	  network/networkThread/NetworkThread.cpp \
	  network/protocol/Message.cpp \
	  network/protocol/ProtocolParser.cpp \
	  network/utils/SystemWrapper.cpp \
	  network/networkManager/NetworkManager.cpp \
	  textureManager/TextureManager.cpp \
	  textureManager/ModelManager.cpp \
	  renderer/Renderer.cpp \
	  renderer/MapRenderer.cpp \
	  renderer/MapRendererObserver.cpp \
	  renderer/UIRenderer.cpp \
	  renderer/strategies/DetailedTileRenderStrategy.cpp \
	  renderer/strategies/ModelTileRenderStrategy.cpp \
	  renderer/strategies/SimpleTileRenderStrategy.cpp \
	  renderer/strategies/TileRenderStrategyFactory.cpp \
	  cameraController/CameraController.cpp \
	  GameLoop.cpp

MAIN_OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(MAIN))
MAIN_DEP = $(patsubst %.cpp, $(BUILD_DIR)/%.d, $(MAIN))
OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC))
DEP = $(patsubst %.cpp, $(BUILD_DIR)/%.d, $(SRC))

# Exclude test_dlloader from main build but keep it for specific targets
TEST_SRC = test_dlloader.cpp
TEST_OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(TEST_SRC))

# Directories
INCLUDE_DIRS = -I. -I../../libs -I../../src/Shared -I../../libs/RaylibCPP \
	-I../../libs/RayGUICPP

# Libraries
LIBS = -L../../build -lraylib -lraylib_cpp -lraygui_cpp -lm -lpthread -ldl

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror $(INCLUDE_DIRS) -fPIC
LDFLAGS = $(LIBS)
DEBUG_FLAGS = -g3 -ggdb

# Colors and animations
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
WHITE = \033[37m
BOLD = \033[1m
RESET = \033[0m
BG_BLACK = \033[40m

# Animation frames for compilation progress
COMPILE_MSG = "$(BLUE)[$(BOLD)COMPILING$(RESET)$(BLUE)]$(RESET)"
LINKING_MSG = "$(GREEN)[$(BOLD)LINKING$(RESET)$(GREEN)]$(RESET)"
SUCCESS_MSG = "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET)"
RAINBOW = "$(RED)Z$(YELLOW)a$(GREEN)p$(CYAN)p$(BLUE)y$(MAGENTA) GUI$(RESET)"

# Main target
all: setup_dirs display_banner $(NAME)

# Create the necessary build directories
setup_dirs:
	@mkdir -p $(BUILD_DIR)/graphicalContext
	@mkdir -p $(BUILD_DIR)/parsing
	@mkdir -p $(BUILD_DIR)/shared/exception
	@mkdir -p $(BUILD_DIR)/network/buffer
	@mkdir -p $(BUILD_DIR)/network/connection
	@mkdir -p $(BUILD_DIR)/network/networkThread
	@mkdir -p $(BUILD_DIR)/network/protocol
	@mkdir -p $(BUILD_DIR)/network/utils
	@mkdir -p $(BUILD_DIR)/network/networkManager
	@mkdir -p ../../build

# Display a fancy banner for the build process
display_banner:
	@clear
	@echo "$(CYAN)╔═════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(BG_BLACK)$(MAGENTA)$(BOLD)Z A P P Y   G U I   \
	B U I L D E R$(RESET)  $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════╝$(RESET)"
	@echo ""
	@echo "$(BLUE)[$(BOLD)INFO$(RESET)$(BLUE)]$(RESET) $(BOLD)Préparation \
	de l'interface graphique...$(RESET)"
	@sleep 0.3

# Build the GUI
$(NAME): $(OBJ) $(MAIN_OBJ)
	@echo ""
	@echo "$(GREEN)[$(BOLD)BUILDING$(RESET)$(GREEN)]$(RESET) $(BOLD)Liaison \
	de l'interface graphique...$(RESET)"
	@for i in `seq 1 21`; do \
		progress=$$(( i > 20 ? 20 : i )); \
		echo -n "$(GREEN)[$(BOLD)PROGRESS$(RESET)$(GREEN)]$(RESET) $(BOLD)["; \
		for j in `seq 1 $$progress`; do echo -n "▰"; done; \
		for j in `seq $$progress 19`; do echo -n "▱"; done; \
		echo -n "] $$(( i > 20 ? 100 : 5 * i ))%$(RESET)\r"; \
		sleep 0.05; \
	done
	@echo ""
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(MAIN_OBJ) $(OBJ) $(LDFLAGS)
	@echo "$(CYAN)[$(BOLD)LIB$(RESET)$(CYAN)]$(RESET) $(BOLD)Création de \
	la bibliothèque statique...$(RESET)"
	@ar rc ../../build/libgui.a $(OBJ)
	@ranlib ../../build/libgui.a
	@sleep 0.2
	@echo ""
	@echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Interface \
	graphique compilée avec succès : $(NAME)$(RESET)"
	@echo "$(CYAN)╔═════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)L'interface graphique est \
	prête !$(RESET)      $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════╝$(RESET)"

# Test DLLoader build
test_dlloader: setup_dirs $(TEST_OBJ) $(filter-out $(BUILD_DIR)/main.o, $(OBJ))
	@echo "$(LINKING_MSG) $(BOLD)Création du programme de test \
	DLLoader...$(RESET)"
	@$(CXX) $(CXXFLAGS) -o ../../test_dlloader $(TEST_OBJ) \
	$(filter-out $(BUILD_DIR)/main.o, $(OBJ)) $(LDFLAGS)
	@echo "$(SUCCESS_MSG) $(BOLD)Test DLLoader compilé avec succès$(RESET)"

# Rule for object files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo -n "$(BLUE)[$(BOLD)COMPILING$(RESET)$(BLUE)]$(RESET)\
	 $(notdir $<)... "
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "$(GREEN)✓$(RESET)"

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean all
	@echo "$(MAGENTA)[$(BOLD)DEBUG$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Mode \
	debug activé !$(RESET)"

# Coverage build for tests
coverage: CXXFLAGS += --coverage -fprofile-arcs -ftest-coverage
coverage: LDFLAGS += --coverage -lgcov
coverage: clean all
	@echo "$(MAGENTA)[$(BOLD)COVERAGE$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Mode \
	couverture activé pour l'interface graphique !$(RESET)"
	@echo "$(CYAN)╔══════════════════════════════════════════\
	═══════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Interface graphique compilée avec \
	instrumentation$(RESET)  $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚══════════════════════════════════════════\
	═══════════╝$(RESET)"

# Clean object files
clean:
	@echo "$(YELLOW)[$(BOLD)CLEANING$(RESET)$(YELLOW)]$(RESET) \
	$(BOLD)Suppression des fichiers objets...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@sleep 0.3
	@for i in `seq 1 10`; do \
		echo -n "$(YELLOW)[$(BOLD)CLEAN$(RESET)$(YELLOW)]$(RESET) \
	$(BOLD)Progression: ["; \
		for j in `seq 1 $$i`; do echo -n "▓"; done; \
		for j in `seq $$i 10`; do echo -n "░"; done; \
		echo -n "]$(RESET)\r"; \
		sleep 0.1; \
	done
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) \
	$(BOLD)Nettoyage effectué !$(RESET)       "

# Clean everything (including binary)
fclean: clean
	@echo "$(RED)[$(BOLD)FULL CLEAN$(RESET)$(RED)]$(RESET) \
	$(BOLD)Suppression du binaire...$(RESET)"
	@rm -f $(NAME)
	@rm -f ../../test_dlloader
	@rm -f ../../build/libgui.a
	@sleep 0.3
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) \
	$(BOLD)Nettoyage complet effectué !$(RESET)"

# Clean and rebuild
re: fclean all

# Include dependency files
-include $(DEP)
-include $(MAIN_DEP)

.PHONY: all clean fclean re debug coverage test_dlloader \
compile_animation setup_dirs
