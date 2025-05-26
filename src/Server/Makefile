##
## EPITECH PROJECT, 2025
## Zappy Server
## File description:
## Server Makefile
##

CC				=	gcc
CFLAGS			=	-Wextra -Wall -Werror -std=gnu17
DEBUGFLAGS		=	-ggdb -fsanitize=address

# Directories
SRC_DIR			=	.
INCLUDE_DIR		=	include
SHARED_INCLUDE	=	../Shared/include

# Source files
SRCS			=	main.c \
					help.c
OBJS			=	$(SRCS:.c=.o)
BINARY_NAME		=	../../zappy_server

# Includes
INCLUDES		=	-I$(INCLUDE_DIR) -I$(SHARED_INCLUDE)

# Colors
RED				=	\033[0;91m
GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m
BLUE			=	\033[0;94m
CYAN			=	\033[0;96m
RESET			=	\033[0m

all: $(BINARY_NAME)

$(BINARY_NAME): $(OBJS)
	@echo -e "$(GREEN)Compiling server binary...$(RESET)"
	@$(CC) -o $(BINARY_NAME) $(OBJS) $(CFLAGS) $(INCLUDES)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

debug: CFLAGS += $(DEBUGFLAGS)
debug: $(BINARY_NAME)

clean:
	@echo -e "$(YELLOW)Cleaning server objects...$(RESET)"
	@rm -f $(OBJS)

fclean: clean
	@echo -e "$(RED)Cleaning server binary...$(RESET)"
	@rm -f $(BINARY_NAME)

re: fclean all

.PHONY: all debug clean fclean re
