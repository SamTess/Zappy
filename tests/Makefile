##
## EPITECH PROJECT, 2025
## Zappy Tests Makefile
## File description:
## Builds and runs the tests for the Zappy project
##

# Test binaries
C_UNIT_TESTS_NAME = ../c_unit_tests
CPP_UNIT_TESTS_NAME = ../cpp_unit_tests
FUNCTIONAL_TESTS_NAME = ../functional_tests

# Build directory
BUILD_DIR = ../build/tests
BUILD_DIR_C = $(BUILD_DIR)/server
BUILD_DIR_CPP = $(BUILD_DIR)/client

UNIT_SRC = server/test_server.c \
		   server/test_player_inventory.c

CLIENT_SRC = unit/network/buffer/test_circular_buffer.cpp \
			 unit/network/buffer/test_circular_buffer_enhanced.cpp \
			 unit/network/connection/test_tcp_connection.cpp \
			 unit/network/protocol/test_protocol_parser.cpp \
			 unit/network/utils/test_system_wrapper.cpp \
			 unit/network/networkManager/test_network_manager.cpp \
			 unit/network/networkThread/test_network_thread.cpp


UNIT_OBJ = $(patsubst %.c, $(BUILD_DIR_C)/%.o, $(UNIT_SRC))
CLIENT_OBJ = $(patsubst %.cpp, $(BUILD_DIR_CPP)/%.o, $(CLIENT_SRC))
UNIT_DEP = $(patsubst %.c, $(BUILD_DIR_C)/%.d, $(UNIT_SRC))
CLIENT_DEP = $(patsubst %.cpp, $(BUILD_DIR_CPP)/%.d, $(CLIENT_SRC))

# Directories
SERVER_DIR = ../src/Server
GUI_DIR = ../src/GUI
INCLUDES = -I$(SERVER_DIR)/include -I../src/Shared -I$(GUI_DIR) -I$(GUI_DIR)/network -I../libs -I../libs/RaylibCPP -I../libs/RayGUICPP

# Libraries
SERVER_LIB = ../build/libserver.a
GUI_LIB = ../build/libgui.a

# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra $(INCLUDES)
CXXFLAGS = -Wall -Wextra -std=c++17 $(INCLUDES)
LDFLAGS_C = -L../build -lserver -lcriterion -lgcov --coverage -lstdc++ -pthread -lm -ldl
LDFLAGS_CPP = -L../build -lgui -lraylib_cpp -lraygui_cpp -lcriterion -lgcov --coverage -lstdc++ -pthread -lm -ldl
DEBUG_FLAGS = -g3 -ggdb
TEST_FLAGS = --coverage -fprofile-arcs -ftest-coverage

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

# Main test target
tests_run: setup_dirs display_test_banner $(C_UNIT_TESTS_NAME) $(CPP_UNIT_TESTS_NAME) $(GUI_LIB)
	@echo ""
	@echo "$(MAGENTA)[$(BOLD)TESTING$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Lancement des tests unitaires...$(RESET)"
	@echo ""
	@sleep 0.5
	@./$(C_UNIT_TESTS_NAME)
	@./$(CPP_UNIT_TESTS_NAME)
	@echo ""
	@echo "$(GREEN)[$(BOLD)COVERAGE$(RESET)$(GREEN)]$(RESET) $(BOLD)Génération du rapport de couverture...$(RESET)"
	@gcovr --exclude tests/ --html-details -o coverage.html
	@mkdir -p coverage_html
	@echo ""
	@echo "$(CYAN)╔═══════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(MAGENTA)$(BOLD)R É S U L T A T   C O V E R A G E$(RESET)      $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════════╝$(RESET)"
	@echo ""
	@gcovr --exclude tests/
	@echo ""
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Le rapport de couverture est disponible dans coverage.html$(RESET)"
	@echo "$(BLUE)[$(BOLD)INFO$(RESET)$(BLUE)]$(RESET) $(BOLD)Ouvrez coverage.html dans un navigateur pour voir les détails.$(RESET)"
	@echo "$(CYAN)╔═══════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Génération rapport HTML...$(RESET)       $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════════╝$(RESET)"
	@gcovr --exclude tests/ --html -o coverage_html/index.html
	@echo "$(CYAN)╔═══════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Tests unitaires terminés avec succès !$(RESET)       $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════════╝$(RESET)"
	@echo "$(GREEN)[$(BOLD)FINISHED$(RESET)$(GREEN)]$(RESET) $(BOLD)Tous les tests unitaires ont été exécutés avec succès !$(RESET)"

# Functional tests target - runs Python scripts in tests/client/network
functional_tests: setup_dirs
	@echo ""
	@echo "$(MAGENTA)[$(BOLD)TESTING$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Lancement des tests fonctionnels...$(RESET)"
	@echo ""
	@if command -v python3 >/dev/null 2>&1; then \
		echo "$(GREEN)[$(BOLD)INFO$(RESET)$(GREEN)]$(RESET) $(BOLD)Python 3 trouvé. Exécution des tests...$(RESET)"; \
		python_scripts=$$(find client -name "*.py"); \
		for script in $$python_scripts; do \
			echo "$(CYAN)[$(BOLD)RUNNING$(RESET)$(CYAN)]$(RESET) $(BOLD)$$script$(RESET)"; \
			python3 $$script; \
			if [ $$? -eq 0 ]; then \
				echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Test réussi !$(RESET)"; \
			else \
				echo "$(RED)[$(BOLD)FAILED$(RESET)$(RED)]$(RESET) $(BOLD)Le test a échoué !$(RESET)"; \
			fi; \
			echo ""; \
		done; \
	else \
		echo "$(RED)[$(BOLD)ERROR$(RESET)$(RED)]$(RESET) $(BOLD)Python 3 n'est pas installé.$(RESET)"; \
		exit 1; \
	fi
	@echo "$(CYAN)╔═══════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Tests fonctionnels terminés !$(RESET)        $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════════╝$(RESET)"

# Create the necessary build directories
setup_dirs:
	@mkdir -p $(BUILD_DIR)/server
	@mkdir -p $(BUILD_DIR)/client
	@mkdir -p $(BUILD_DIR)/unit/network/buffer
	@mkdir -p $(BUILD_DIR)/unit/network/connection
	@mkdir -p $(BUILD_DIR)/unit/network/networkManager
	@mkdir -p $(BUILD_DIR)/unit/network/networkThread
	@mkdir -p $(BUILD_DIR)/unit/network/protocol
	@mkdir -p $(BUILD_DIR)/unit/network/utils
	@mkdir -p ../build

# Display fancy test banner
display_test_banner:
	@clear
	@echo "$(CYAN)╔═══════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(BG_BLACK)$(YELLOW)$(BOLD)Z A P P Y   T E S T   S U I T E$(RESET)  $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════╝$(RESET)"
	@echo ""
	@echo "$(BLUE)[$(BOLD)INFO$(RESET)$(BLUE)]$(RESET) $(BOLD)Préparation de l'environnement de test...$(RESET)"
	@sleep 0.3

# Ensure GUI library is built
$(GUI_LIB):
	@echo "$(BLUE)[$(BOLD)BUILD$(RESET)$(BLUE)]$(RESET) $(BOLD)Construction de la bibliothèque GUI...$(RESET)"
	@$(MAKE) -C ../src/GUI

# Build c unit tests
$(C_UNIT_TESTS_NAME): check_criterion $(SERVER_LIB) $(UNIT_OBJ)
	@echo ""
	@echo "$(GREEN)[$(BOLD)BUILDING$(RESET)$(GREEN)]$(RESET) $(BOLD)Compilation des tests unitaires C...$(RESET)"
	@for i in `seq 1 21`; do \
		progress=$$(( i > 20 ? 20 : i )); \
		echo -n "$(GREEN)[$(BOLD)PROGRESS$(RESET)$(GREEN)]$(RESET) $(BOLD)["; \
		for j in `seq 1 $$progress`; do echo -n "▰"; done; \
		for j in `seq $$progress 19`; do echo -n "▱"; done; \
		echo -n "] $$(( i > 20 ? 100 : 5 * i ))%$(RESET)\r"; \
		sleep 0.05; \
	done
	@echo ""
	@$(CC) $(CFLAGS) $(TEST_FLAGS) -o $(C_UNIT_TESTS_NAME) $(UNIT_OBJ) $(LDFLAGS_C)
	@echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Tests unitaires compilés avec succès$(RESET)"
	@echo "$(CYAN)╔═════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Tests serveur prêts à être exécutés !$(RESET)         $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════════╝$(RESET)"
	@echo "$(GREEN)[$(BOLD)READY$(RESET)$(GREEN)]$(RESET) $(BOLD)Tests unitaires compilés avec succès$(RESET)"

# Build cpp unit tests
$(CPP_UNIT_TESTS_NAME): check_criterion $(GUI_LIB) $(CLIENT_OBJ)
	@echo ""
	@echo "$(GREEN)[$(BOLD)BUILDING$(RESET)$(GREEN)]$(RESET) $(BOLD)Compilation des tests unitaires...$(RESET)"
	@for i in `seq 1 21`; do \
		progress=$$(( i > 20 ? 20 : i )); \
		echo -n "$(GREEN)[$(BOLD)PROGRESS$(RESET)$(GREEN)]$(RESET) $(BOLD)["; \
		for j in `seq 1 $$progress`; do echo -n "▰"; done; \
		for j in `seq $$progress 19`; do echo -n "▱"; done; \
		echo -n "] $$(( i > 20 ? 100 : 5 * i ))%$(RESET)\r"; \
		sleep 0.05; \
	done
	@echo ""
	@$(CXX) $(CXXFLAGS) $(TEST_FLAGS) -o $(CPP_UNIT_TESTS_NAME) $(CLIENT_OBJ) $(LDFLAGS_CPP)
	@echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Tests unitaires compilés avec succès$(RESET)"
	@echo "$(CYAN)╔═════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Tests client prêts à être exécutés !$(RESET)         $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════════╝$(RESET)"
	@echo "$(GREEN)[$(BOLD)READY$(RESET)$(GREEN)]$(RESET) $(BOLD)Tests unitaires compilés avec succès$(RESET)"


# Check if criterion is installed
check_criterion:
	@echo -n "$(BLUE)[$(BOLD)CHECK$(RESET)$(BLUE)]$(RESET) $(BOLD)Vérification de la présence de Criterion...$(RESET) "
	@if ! pkg-config --exists criterion; then \
		echo "$(RED)NON TROUVÉ$(RESET)"; \
		echo "$(RED)[$(BOLD)ERROR$(RESET)$(RED)]$(RESET) $(BOLD)La bibliothèque Criterion est requise pour les tests.$(RESET)"; \
		echo "$(YELLOW)[$(BOLD)TIP$(RESET)$(YELLOW)]$(RESET) $(BOLD)Installez-la avec: sudo apt-get install libcriterion-dev$(RESET)"; \
		exit 1; \
	else \
		echo "$(GREEN)OK$(RESET)"; \
	fi

# Rules for object files
$(BUILD_DIR_C)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo -n "$(CYAN)[$(BOLD)COMPILING C$(RESET)$(CYAN)]$(RESET) $(notdir $<)... "
	@$(CC) $(CFLAGS) $(TEST_FLAGS) -MMD -MP -c $< -o $@
	@echo "$(GREEN)✓$(RESET)"

$(BUILD_DIR_CPP)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo -n "$(CYAN)[$(BOLD)COMPILING C++$(RESET)$(CYAN)]$(RESET) $(notdir $<)... "
	@$(CXX) $(CXXFLAGS) $(TEST_FLAGS) -MMD -MP -c $< -o $@
	@echo "$(GREEN)✓$(RESET)"

# Clean object files
clean:
	@echo "$(YELLOW)[$(BOLD)CLEANING$(RESET)$(YELLOW)]$(RESET) $(BOLD)Suppression des fichiers objets des tests...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@rm -f *.gcno *.gcda
	@sleep 0.3
	@for i in `seq 1 11`; do \
		progress=$$(( i > 10 ? 10 : i )); \
		echo -n "$(YELLOW)[$(BOLD)CLEAN$(RESET)$(YELLOW)]$(RESET) $(BOLD)Progression: ["; \
		for j in `seq 1 $$progress`; do echo -n "▓"; done; \
		for j in `seq $$progress 9`; do echo -n "░"; done; \
		echo -n "]$(RESET)\r"; \
		sleep 0.1; \
	done
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Nettoyage effectué !$(RESET)       "

# Clean everything (including binary)
fclean: clean
	@echo "$(RED)[$(BOLD)FULL CLEAN$(RESET)$(RED)]$(RESET) $(BOLD)Suppression des binaires de test...$(RESET)"
	@rm -f $(C_UNIT_TESTS_NAME) $(CPP_UNIT_TESTS_NAME) $(FUNCTIONAL_TESTS_NAME)
	@rm -f coverage.html coverage_cpp.html
	@rm -rf coverage_html
	@sleep 0.3
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Nettoyage complet effectué !$(RESET)"

# Cible spécifique pour les tests C++ uniquement
cpp_tests: setup_dirs display_test_banner $(CPP_UNIT_TESTS_NAME) $(GUI_LIB)
	@echo ""
	@echo "$(MAGENTA)[$(BOLD)TESTING$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Lancement des tests unitaires C++...$(RESET)"
	@echo ""
	@sleep 0.5
	@./$(CPP_UNIT_TESTS_NAME)
	@echo ""
	@echo "$(GREEN)[$(BOLD)COVERAGE$(RESET)$(GREEN)]$(RESET) $(BOLD)Génération du rapport de couverture C++...$(RESET)"
	@gcovr -r .. --exclude-directories='tests|build' --gcov-ignore-errors=no_working_dir_found --html-details -o coverage_cpp.html
	@mkdir -p coverage_html
	@echo ""
	@echo "$(CYAN)╔═════════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(MAGENTA)$(BOLD)R É S U L T A T   C O V E R A G E   C + +$(RESET)  $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════════════╝$(RESET)"
	@echo ""
	@gcovr -r .. --exclude-directories='tests|build' --gcov-ignore-errors=no_working_dir_found --print-summary
	@echo ""
	@echo "$(CYAN)╔═════════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Tests C++ terminés avec succès !$(RESET)          $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════════════╝$(RESET)"
	@echo "$(GREEN)[$(BOLD)FINISHED$(RESET)$(GREEN)]$(RESET) $(BOLD)Tous les tests C++ ont été exécutés !$(RESET)"

# Clean and rebuild
re: fclean tests_run

# Include dependency files
-include $(UNIT_DEP)
-include $(CLIENT_DEP)

.PHONY: tests_run functional_tests cpp_tests clean fclean re debug setup_dirs display_test_banner check_criterion coverage
