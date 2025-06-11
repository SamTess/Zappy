##
## EPITECH PROJECT, 2025
## Zappy Server Makefile
## File description:
## Builds the Zappy server binary
##

# Binary name
NAME = ../../zappy_server
BUILD_DIR = ../../build/Server

MAIN = main.c
# Source files
SRC = 	help.c								\
		free.c								\
		parsing_err.c						\
		parsing.c							\
		network/connection_err.c			\
		network/connection.c				\
		network/connection_utils.c			\
		network/server_run.c				\
		network/queue_connection.c			\
		network/client_node.c 				\
		network/graphical_client.c			\
		map/map_creation.c					\
		map/tile.c							\
		map/tick_update.c					\
		map/resources_map.c					\
		command/parse_command.c				\
		command/parse_command_utils.c		\
		command/forward.c					\
		command/right.c						\
		command/left.c						\
		command/inventory.c					\
		command/look.c						\
		command/look_utils.c				\
		command/tile_to_str.c				\
		command/eject.c						\
		command/connect_nbr.c				\
		command/take_object.c				\
		command/set_object.c				\
		command/fork.c						\
		command/start_incantation.c			\
		command/finish_incantation.c		\
		command/broadcast.c					\
		command/pending_cmd_utils.c			\
		map/egg.c							\
		player/player_init.c				\
		player/player_inventory.c			\
		player/player_inventory_utils.c		\
		player/player_inventory_display.c	\
		player/player_death.c				\
		buffer/circular_buffer.c			\
		graphical_command/command_bct.c		\
		graphical_command/command_msz.c		\
		graphical_command/command_pin.c		\
		graphical_command/command_plv.c		\
		graphical_command/command_pnw.c		\
		graphical_command/command_sgt.c		\
		graphical_command/command_tna.c		\
		graphical_command/command_ebo.c		\
		graphical_command/command_ppo.c		\
		graphical_command/command_pie.c		\
		graphical_command/command_pex.c		\
		graphical_command/command_pfk.c		\
		graphical_command/command_pdr.c		\
		graphical_command/command_enw.c		\
		graphical_command/command_edi.c		\
		graphical_command/command_pic.c		\
		graphical_command/command_sst.c		\
		graphical_command/command_pdi.c		\
		graphical_command/command_pbc.c		\
		graphical_command/command_pgt.c		\
		graphical_command/command_seg.c		\

MAIN_OBJ = $(patsubst %.c, $(BUILD_DIR)/%.o, $(MAIN))
MAIN_DEP = $(patsubst %.c, $(BUILD_DIR)/%.d, $(MAIN))
OBJ = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC))
DEP = $(patsubst %.c, $(BUILD_DIR)/%.d, $(SRC))

# Directories
INCLUDE_DIR = include

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR)
LDFLAGS =
DEBUG_FLAGS = -g3 -ggdb
COVERAGE_FLAGS = --coverage

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

# Animation frames
COMPILE_MSG = "$(BLUE)[$(BOLD)COMPILING$(RESET)$(BLUE)]$(RESET)"
LINKING_MSG = "$(GREEN)[$(BOLD)LINKING$(RESET)$(GREEN)]$(RESET)"
SUCCESS_MSG = "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET)"
BG_BLACK = \033[40m

# Main target
all: setup_dirs display_banner $(NAME)

# Create the necessary build directories
setup_dirs:
	@mkdir -p $(BUILD_DIR)/player
	@mkdir -p $(BUILD_DIR)/network
	@mkdir -p $(BUILD_DIR)/map
	@mkdir -p ../../build

# Display a fancy banner for the build process
display_banner:
	@clear
	@echo "$(CYAN)╔═══════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(BG_BLACK)$(GREEN)$(BOLD)Z A P P Y   S E R V E R   \
	B U I L D E R$(RESET)  $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════╝$(RESET)"
	@echo ""
	@echo "$(BLUE)[$(BOLD)INFO$(RESET)$(BLUE)]$(RESET) $(BOLD)Préparation \
	du serveur Zappy...$(RESET)"
	@sleep 0.3

# Build the server
$(NAME): $(OBJ) $(MAIN_OBJ)
	@echo ""
	@echo "$(GREEN)[$(BOLD)BUILDING$(RESET)$(GREEN)]$(RESET) $(BOLD)Liaison \
	du serveur Zappy...$(RESET)"
	@for i in `seq 1 21`; do \
		progress=$$(( i > 20 ? 20 : i )); \
		echo -n "$(GREEN)[$(BOLD)PROGRESS$(RESET)$(GREEN)]$(RESET) $(BOLD)["; \
		for j in `seq 1 $$progress`; do echo -n "▰"; done; \
		for j in `seq $$progress 19`; do echo -n "▱"; done; \
		echo -n "] $$(( i > 20 ? 100 : 5 * i ))%$(RESET)\r"; \
		sleep 0.05; \
	done
	@echo ""
	@$(CC) $(CFLAGS) -o $(NAME) $(MAIN_OBJ) $(OBJ) $(LDFLAGS)
	@echo "$(CYAN)[$(BOLD)LIB$(RESET)$(CYAN)]$(RESET) $(BOLD)Création de \
	la bibliothèque statique...$(RESET)"
	@ar rc ../../build/libserver.a $(OBJ)
	@ranlib ../../build/libserver.a
	@sleep 0.2
	@echo ""
	@echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Serveur \
	compilé avec succès : $(NAME)$(RESET)"
	@echo "$(CYAN)╔════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Le serveur est prêt à être \
	utilisé !$(RESET)      $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚════════════════════════════════════════════╝$(RESET)"

# Rule for object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo -n "$(BLUE)[$(BOLD)COMPILING$(RESET)$(BLUE)]$(RESET)\
	 $(notdir $<)... "
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "$(GREEN)✓$(RESET)"

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all
	@echo "$(MAGENTA)[$(BOLD)DEBUG$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Mode \
	debug activé !$(RESET)"

# Coverage build for tests
coverage: CFLAGS += $(COVERAGE_FLAGS)
coverage: LDFLAGS += $(COVERAGE_FLAGS)
coverage: clean all
	@echo "$(MAGENTA)[$(BOLD)COVERAGE$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Mode \
	couverture activé pour le serveur !$(RESET)"
	@echo "$(CYAN)╔═════════════════════════════════════════\
	═════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Serveur compilé avec \
	instrumentation$(RESET)        $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════\
	═════╝$(RESET)"

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
	@rm -f ../../build/libserver.a
	@sleep 0.3
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) \
	$(BOLD)Nettoyage complet effectué !$(RESET)"

# Clean and rebuild
re: fclean all

# Include dependency files
-include $(DEP)
-include $(MAIN_DEP)

.PHONY: all clean fclean re debug coverage setup_dirs
