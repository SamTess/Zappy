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
BUILD_DIR		= ../../build

MAIN_FILE 		= 	main.cpp
# Source files
SRCS			=	shared/exception/AException.cpp \
					parsing/ParsingCLI.cpp \

OBJ_FILES		= $(SRCS:%.cpp=$(BUILD_DIR)/GUI/%.o)
LIB_NAME		= $(BUILD_DIR)/libgui.a
BINARY_NAME		= ../../zappy_gui

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
	@echo -e "$(GREEN)Compiling GUI binary...$(RESET)"
	@$(CXX) -o $(BINARY_NAME) $(MAIN_FILE) $(OBJ_FILES) $(LIB_NAME) \
		$(CXXFLAGS) $(INCLUDES) $(SFMLFLAGS)

$(BUILD_DIR)/GUI/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo -e "$(BLUE)Compiling $< into $@...$(RESET)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all

clean:
	@echo -e "$(YELLOW)Cleaning GUI objects...$(RESET)"
	@rm -f $(BUILD_DIR)/GUI/*.o

fclean: clean
	@echo -e "$(RED)Cleaning GUI binary and library...$(RESET)"
	@rm -f $(BINARY_NAME)
	@rm -f $(LIB_NAME)

re: fclean all

.PHONY: all debug clean fclean re
