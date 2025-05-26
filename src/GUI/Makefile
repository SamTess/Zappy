##
## EPITECH PROJECT, 2025
## Zappy GUI
## File description:
## GUI Makefile
##

CXX				=	g++
CXXFLAGS		=	-Wextra -Wall -Werror -std=c++20
DEBUGFLAGS		=	-ggdb -fsanitize=address
SFMLFLAGS		=	-lsfml-graphics -lsfml-window -lsfml-system \
					-lsfml-audio -lsfml-network

# Directories
SRC_DIR			=	.
INCLUDE_DIR		=	include
SHARED_INCLUDE	=	../Shared/include

# Source files
SRCS			=	main.cpp \
					help.cpp
OBJS			=	$(SRCS:.cpp=.o)
BINARY_NAME		=	../../zappy_gui

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
	@echo -e "$(GREEN)Compiling GUI binary...$(RESET)"
	@$(CXX) -o $(BINARY_NAME) $(OBJS) $(CXXFLAGS) $(INCLUDES) $(SFMLFLAGS)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(BINARY_NAME)

clean:
	@echo -e "$(YELLOW)Cleaning GUI objects...$(RESET)"
	@rm -f $(OBJS)

fclean: clean
	@echo -e "$(RED)Cleaning GUI binary...$(RESET)"
	@rm -f $(BINARY_NAME)

re: fclean all

.PHONY: all debug clean fclean re
