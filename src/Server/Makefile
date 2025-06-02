##
## EPITECH PROJECT, 2025
## Zappy Server
## File description:
## Server Makefile
##

CC				=	gcc
CFLAGS			=	-Wextra -Wall -Werror -std=gnu17 -g
DEBUGFLAGS		=	-ggdb -fsanitize=address

# Directories
SRC_DIR			=	.
INCLUDE_DIR		=	include
SHARED_INCLUDE	=	../Shared/include
BUILD_DIR		= ../../build

# Library and binary
LIB_NAME		= $(BUILD_DIR)/libserver.a
BINARY_NAME		= ../../zappy_server

# Source files
MAIN_FILE 		= 	main.c

SRCS			=	help.c								\
					parsing_err.c						\
					parsing.c							\
					network/connection_err.c			\
					network/connection.c				\
					network/server_run.c				\
					map/map_creation.c					\
					map/tile.c							\
					command/parse_command.c				\
					command/parse_command_utils.c		\
					command/forward.c					\
					command/right.c						\
					command/left.c						\
					command/inventory.c					\
					player/player_init.c				\
					player/player_inventory.c			\
					player/player_inventory_utils.c		\
					player/player_inventory_display.c	\

OBJ_FILES		= $(SRCS:%.c=$(BUILD_DIR)/Server/%.o)

# Includes
INCLUDES		=	-Iinclude -I../Shared/include

# Colors
RED				=	\033[0;91m
GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m
BLUE			=	\033[0;94m
CYAN			=	\033[0;96m
RESET			=	\033[0m

all: $(LIB_NAME) $(BINARY_NAME)

$(LIB_NAME): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	@ar rcs $(LIB_NAME) $(OBJ_FILES)

$(BINARY_NAME): $(OBJ_FILES) $(LIB_NAME)
	@echo -e "$(GREEN)Compiling server binary...$(RESET)"
	@$(CC) -o $(BINARY_NAME) $(MAIN_FILE) $(OBJ_FILES) $(LIB_NAME) \
		$(CFLAGS) $(INCLUDES)

$(BUILD_DIR)/Server/%.o: %.c
	@mkdir -p $(dir $@)
	@echo -e "$(BLUE)Compiling $< into $@...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

debug: CFLAGS += $(DEBUGFLAGS)
debug: all

clean:
	@echo -e "$(YELLOW)Cleaning server objects...$(RESET)"
	@rm -f $(BUILD_DIR)/Server/*.o

fclean: clean
	@echo -e "$(RED)Cleaning server binary and library...$(RESET)"
	@rm -f $(BINARY_NAME)
	@rm -f $(LIB_NAME)

re: fclean all

.PHONY: all debug clean fclean re
