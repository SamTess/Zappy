##
## EPITECH PROJECT, 2025
## Zappy AI
## File description:
## AI Makefile
##

PYTHON			=	python3
PIP				=	pip3

# Directories
SRC_DIR			=	.
BUILD_DIR		= ../../build

# Main file
MAIN_FILE		=	main.py

BINARY_NAME		= $(BUILD_DIR)/zappy_ai
REQUIREMENTS	=	requirements.txt

# Colors
RED				=	\033[0;91m
GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m
BLUE			=	\033[0;94m
CYAN			=	\033[0;96m
RESET			=	\033[0m

LIB_AI_NAME = $(BUILD_DIR)/libai.a

all: $(LIB_AI_NAME) $(BINARY_NAME)

$(LIB_AI_NAME):
	@mkdir -p $(BUILD_DIR)
	@echo -e "$(GREEN)Creating dummy AI library...$(RESET)"
	@ar rcs $(LIB_AI_NAME)

$(BINARY_NAME): $(MAIN_FILE) $(LIB_AI_NAME)
	@mkdir -p $(BUILD_DIR)
	@echo -e "$(GREEN)Creating AI script...$(RESET)"
	@echo '#!/usr/bin/env python3' > $(BINARY_NAME)
	@cat $(MAIN_FILE) >> $(BINARY_NAME)
	@chmod +x $(BINARY_NAME)
	@mv $(BINARY_NAME) ../../

install:
	@if [ -f $(REQUIREMENTS) ]; then \
		echo -e "$(CYAN)Installing Python dependencies...$(RESET)"; \
		$(PIP) install -r $(REQUIREMENTS); \
	fi

run: $(BINARY_NAME)
	@echo -e "$(BLUE)Running AI...$(RESET)"
	@$(PYTHON) $(BINARY_NAME)

clean:
	@echo -e "$(YELLOW)Cleaning AI build files...$(RESET)"
	@rm -rf __pycache__
	@find . -name "*.pyc" -delete
	@rm -f $(BINARY_NAME)

fclean: clean
	@echo -e "$(RED)Cleaning AI binary...$(RESET)"
	@rm -f $(BINARY_NAME)

re: fclean all

.PHONY: all install run clean fclean re
