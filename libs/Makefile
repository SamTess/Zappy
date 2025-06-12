##
## EPITECH PROJECT, 2025
## Zappy Libraries Makefile
## File description:
## Builds the libraries for the Zappy project
##

# Build directory
BUILD_DIR = ../build/libs

# RaylibCPP source files
RAYLIB_CPP_SRC = RaylibCPP/RayLib.cpp \
				 RaylibCPP/window/Window.cpp \
				 RaylibCPP/texture/Texture.cpp \
				 RaylibCPP/shape/Shape.cpp \
				 RaylibCPP/input/Input.cpp \
				 RaylibCPP/font/Font.cpp \
				 RaylibCPP/font/Text3D.cpp \
				 RaylibCPP/camera/Camera.cpp \
				 RaylibCPP/audio/Audio.cpp \
				 RaylibCPP/utils/Utils.cpp


RAYLIB_CPP_OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(RAYLIB_CPP_SRC))
RAYLIB_CPP_DEP = $(patsubst %.cpp, $(BUILD_DIR)/%.d, $(RAYLIB_CPP_SRC))

# RayGUICPP source files
RAYGUI_CPP_SRC = RayGUICPP/RayGui.cpp \
				 RayGUICPP/raygui_impl.cpp

RAYGUI_CPP_OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(RAYGUI_CPP_SRC))
RAYGUI_CPP_DEP = $(patsubst %.cpp, $(BUILD_DIR)/%.d, $(RAYGUI_CPP_SRC))

# Libraries
RAYLIB_CPP_LIB_STATIC = ../build/libraylib_cpp.a
RAYGUI_CPP_LIB_STATIC = ../build/libraygui_cpp.a
RAYLIB_CPP_LIB_SHARED = ../plugins/libraylibcpp.so
RAYGUI_CPP_LIB_SHARED = ../plugins/libraygui.so

# Include directories
INCLUDE_DIRS = -I. -I../src/Shared -IRaylibCPP -IRayGUICPP

# External libraries
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror $(INCLUDE_DIRS) -fPIC
RAYGUI_CXXFLAGS = -std=c++17 -Wall -Wextra $(INCLUDE_DIRS) -fPIC \
	-Wno-missing-field-initializers -Wno-unused-parameter -Wno-enum-compare
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

# Main target
all: setup_dirs display_banner static shared
	@echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Toutes \
	les bibliothèques ont été compilées !$(RESET)"

static: $(RAYLIB_CPP_LIB_STATIC) $(RAYGUI_CPP_LIB_STATIC)

shared: $(RAYLIB_CPP_LIB_SHARED) $(RAYGUI_CPP_LIB_SHARED)

# Create the necessary build directories
setup_dirs:
	@mkdir -p $(BUILD_DIR)/RaylibCPP/window
	@mkdir -p $(BUILD_DIR)/RaylibCPP/texture
	@mkdir -p $(BUILD_DIR)/RaylibCPP/shape
	@mkdir -p $(BUILD_DIR)/RaylibCPP/input
	@mkdir -p $(BUILD_DIR)/RaylibCPP/font
	@mkdir -p $(BUILD_DIR)/RaylibCPP/camera
	@mkdir -p $(BUILD_DIR)/RaylibCPP/audio
	@mkdir -p $(BUILD_DIR)/RaylibCPP/utils
	@mkdir -p $(BUILD_DIR)/RayGUICPP
	@mkdir -p ../build
	@mkdir -p ../plugins

# Display a fancy banner for the build process
display_banner:
	@clear
	@echo "$(CYAN)╔══════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(MAGENTA)$(BOLD)Z A P P Y   L I B R A R Y \
		B U I L D E R$(RESET)  $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚══════════════════════════════════════════════╝$(RESET)"
	@echo ""
	@echo "$(BLUE)[$(BOLD)INFO$(RESET)$(BLUE)]$(RESET) $(BOLD)Compilation \
	des bibliothèques...$(RESET)"
	@sleep 0.3

# Build the RaylibCPP static library
$(RAYLIB_CPP_LIB_STATIC): $(RAYLIB_CPP_OBJ)
	@echo ""
	@echo "$(GREEN)[$(BOLD)LIBRARY$(RESET)$(GREEN)]$(RESET) $(BOLD)Création \
	de la bibliothèque statique RaylibCPP...$(RESET)"
	@for i in `seq 1 21`; do \
		progress=$$(( i > 20 ? 20 : i )); \
		echo -n "$(GREEN)[$(BOLD)PROGRESS$(RESET)$(GREEN)]$(RESET) $(BOLD)["; \
		for j in `seq 1 $$progress`; do echo -n "▰"; done; \
		for j in `seq $$progress 19`; do echo -n "▱"; done; \
		echo -n "] $$(( i > 20 ? 100 : 5 * i ))%$(RESET)\r"; \
		sleep 0.05; \
	done
	@echo ""
	@ar rc $(RAYLIB_CPP_LIB_STATIC) $(RAYLIB_CPP_OBJ)
	@ranlib $(RAYLIB_CPP_LIB_STATIC)
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Bibliothèque \
	statique RaylibCPP créée : $(RAYLIB_CPP_LIB_STATIC)$(RESET)"

# Build the RaylibCPP shared library
$(RAYLIB_CPP_LIB_SHARED): $(RAYLIB_CPP_OBJ)
	@echo ""
	@echo "$(GREEN)[$(BOLD)LIBRARY$(RESET)$(GREEN)]$(RESET) $(BOLD)Création \
	de la bibliothèque partagée RaylibCPP...$(RESET)"
	@for i in `seq 1 21`; do \
		progress=$$(( i > 20 ? 20 : i )); \
		echo -n "$(GREEN)[$(BOLD)PROGRESS$(RESET)$(GREEN)]$(RESET) $(BOLD)["; \
		for j in `seq 1 $$progress`; do echo -n "▰"; done; \
		for j in `seq $$progress 19`; do echo -n "▱"; done; \
		echo -n "] $$(( i > 20 ? 100 : 5 * i ))%$(RESET)\r"; \
		sleep 0.05; \
	done
	@echo ""
	@$(CXX) -shared -o $(RAYLIB_CPP_LIB_SHARED) $(RAYLIB_CPP_OBJ) $(LDFLAGS)
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Bibliothèque \
	partagée RaylibCPP créée : $(RAYLIB_CPP_LIB_SHARED)$(RESET)"

# Build the RayGUICPP static library
$(RAYGUI_CPP_LIB_STATIC): $(RAYGUI_CPP_OBJ)
	@echo ""
	@echo "$(GREEN)[$(BOLD)LIBRARY$(RESET)$(GREEN)]$(RESET) $(BOLD)Création \
	de la bibliothèque statique RayGUICPP...$(RESET)"
	@for i in `seq 1 21`; do \
		progress=$$(( i > 20 ? 20 : i )); \
		echo -n "$(GREEN)[$(BOLD)PROGRESS$(RESET)$(GREEN)]$(RESET) $(BOLD)["; \
		for j in `seq 1 $$progress`; do echo -n "▰"; done; \
		for j in `seq $$progress 19`; do echo -n "▱"; done; \
		echo -n "] $$(( i > 20 ? 100 : 5 * i ))%$(RESET)\r"; \
		sleep 0.05; \
	done
	@echo ""
	@ar rc $(RAYGUI_CPP_LIB_STATIC) $(RAYGUI_CPP_OBJ)
	@ranlib $(RAYGUI_CPP_LIB_STATIC)
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Bibliothèque \
	statique RayGUICPP créée : $(RAYGUI_CPP_LIB_STATIC)$(RESET)"

# Build the RayGUICPP shared library
$(RAYGUI_CPP_LIB_SHARED): $(RAYGUI_CPP_OBJ)
	@echo ""
	@echo "$(GREEN)[$(BOLD)LIBRARY$(RESET)$(GREEN)]$(RESET) $(BOLD)Création \
	de la bibliothèque partagée RayGUICPP...$(RESET)"
	@for i in `seq 1 21`; do \
		progress=$$(( i > 20 ? 20 : i )); \
		echo -n "$(GREEN)[$(BOLD)PROGRESS$(RESET)$(GREEN)]$(RESET) $(BOLD)["; \
		for j in `seq 1 $$progress`; do echo -n "▰"; done; \
		for j in `seq $$progress 19`; do echo -n "▱"; done; \
		echo -n "] $$(( i > 20 ? 100 : 5 * i ))%$(RESET)\r"; \
		sleep 0.05; \
	done
	@echo ""
	@$(CXX) -shared -o $(RAYGUI_CPP_LIB_SHARED) $(RAYGUI_CPP_OBJ) $(LDFLAGS)
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Bibliothèque \
	partagée RayGUICPP créée : $(RAYGUI_CPP_LIB_SHARED)$(RESET)"

# Rule for object files
$(BUILD_DIR)/RayGUICPP/%.o: RayGUICPP/%.cpp
	@mkdir -p $(dir $@)
	@echo -n "$(CYAN)[$(BOLD)COMPILING$(RESET)$(CYAN)]$(RESET) $(notdir $<)... "
	@$(CXX) $(RAYGUI_CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "$(GREEN)✓$(RESET)"

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo -n "$(CYAN)[$(BOLD)COMPILING$(RESET)$(CYAN)]$(RESET) $(notdir $<)... "
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "$(GREEN)✓$(RESET)"

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean all
	@echo "$(MAGENTA)[$(BOLD)DEBUG$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Mode \
	debug activé pour les bibliothèques !$(RESET)"

# Coverage build
coverage: CXXFLAGS += --coverage -fprofile-arcs -ftest-coverage
coverage: LDFLAGS += --coverage -lgcov
coverage: clean all
	@echo "$(MAGENTA)[$(BOLD)COVERAGE$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Mode \
	couverture activé pour les bibliothèques !$(RESET)"
	@echo "$(CYAN)╔═══════════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Bibliothèques compilées avec \
	instrumentation$(RESET)   $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════════════╝$(RESET)"

# Clean object files
clean:
	@echo "$(YELLOW)[$(BOLD)CLEANING$(RESET)$(YELLOW)]$(RESET) \
	$(BOLD)Suppression des fichiers objets...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@sleep 0.5
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Nettoyage \
	effectué !$(RESET)"

# Clean everything (including binary)
fclean: clean
	@echo "$(RED)[$(BOLD)FULL CLEAN$(RESET)$(RED)]$(RESET) $(BOLD)Suppression \
	des bibliothèques...$(RESET)"
	@rm -f $(RAYLIB_CPP_LIB_STATIC)
	@rm -f $(RAYGUI_CPP_LIB_STATIC)
	@rm -f $(RAYLIB_CPP_LIB_SHARED)
	@rm -f $(RAYGUI_CPP_LIB_SHARED)
	@sleep 0.5
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Nettoyage \
	complet effectué !$(RESET)"

# Clean and rebuild
re: fclean all

# Include dependency files
-include $(RAYLIB_CPP_DEP) $(RAYGUI_CPP_DEP)

.PHONY: all static shared clean fclean re debug coverage setup_dirs \
display_banner
