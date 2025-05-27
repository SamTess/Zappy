##
## EPITECH PROJECT, 2025
## Zappy Main Makefile
## File description:
## Main Makefile orchestrating all components
##

# Binary names
SERVER_NAME=zappy_server
GUI_NAME=zappy_gui
AI_NAME=zappy_ai
TESTS_NAME=unit_tests

# Directories
SERVER_DIR=src/Server
GUI_DIR=src/GUI
AI_DIR=src/AI
TESTS_DIR=tests
BUILD_DIR=build
DOCS_DIR=docs

# Documentation
DOCS_NAME=ZappyDocumation

# Colors
RED=\033[31m
GREEN=\033[32m
YELLOW=\033[33m
BLUE=\033[34m
MAGENTA=\033[35m
CYAN=\033[36m
RESET=\033[0m

all: zappy_server zappy_gui zappy_ai
	@echo "$(GREEN)All components built successfully!$(RESET)"

zappy_server:
	@echo "$(CYAN)Building Zappy Server...$(RESET)"
	@$(MAKE) -C $(SERVER_DIR)

zappy_gui:
	@echo "$(CYAN)Building Zappy GUI...$(RESET)"
	@$(MAKE) -C $(GUI_DIR)

zappy_ai:
	@echo "$(CYAN)Building Zappy AI...$(RESET)"
	@$(MAKE) -C $(AI_DIR)

tests:
	@echo "$(CYAN)Building tests...$(RESET)"
	@$(MAKE) -C $(TESTS_DIR)

tests_run: tests
	@echo "$(MAGENTA)Running tests...$(RESET)"
	@$(MAKE) -C $(TESTS_DIR) tests_run

debug:
	@echo "$(CYAN)Building all components in debug mode...$(RESET)"
	@$(MAKE) -C $(SERVER_DIR) debug
	@$(MAKE) -C $(GUI_DIR) debug
	@$(MAKE) -C $(AI_DIR)

install_ai:
	@echo "$(CYAN)Installing AI dependencies...$(RESET)"
	@$(MAKE) -C $(AI_DIR) install

init:
	@echo "$(CYAN)Initializing project...$(RESET)"
	@touch .gitignore
	@echo ".vscode" >> .gitignore
	@echo "*.o" >> .gitignore
	@echo "*.gcda" >> .gitignore
	@echo "*.gcno" >> .gitignore
	@echo "coding-style-reports.log" >> .gitignore
	@echo "$(BUILD_DIR)/" >> .gitignore
	@echo "$(SERVER_NAME)" >> .gitignore
	@echo "$(GUI_NAME)" >> .gitignore
	@echo "$(AI_NAME)" >> .gitignore
	@echo "$(TESTS_NAME)" >> .gitignore

clean:
	@echo "$(YELLOW)Cleaning all components...$(RESET)"
	@$(MAKE) -C $(SERVER_DIR) clean
	@$(MAKE) -C $(GUI_DIR) clean
	@$(MAKE) -C $(AI_DIR) clean
	@$(MAKE) -C $(TESTS_DIR) clean
	@rm -rf $(BUILD_DIR)
	@rm -f *~
	@rm -f *.log
	@rm -f vgcore.*
	@rm -f coverage.html
	@rm -rf coverage_html

fclean: clean
	@echo "$(RED)Full clean of all components...$(RESET)"
	@$(MAKE) -C $(SERVER_DIR) fclean
	@$(MAKE) -C $(GUI_DIR) fclean
	@$(MAKE) -C $(AI_DIR) fclean
	@$(MAKE) -C $(TESTS_DIR) fclean
	@rm -f $(SERVER_NAME)
	@rm -f $(GUI_NAME)
	@rm -f $(AI_NAME)
	@rm -f $(TESTS_NAME)
	@rm -rf $(DOCS_DIR)/build
	@rm -f $(DOCS_NAME).pdf
	@rm -f *.html
	@rm -f *.css
	@rm -f *.js

re: fclean all

docs:
	@echo "$(CYAN)Generating documentation with Doxygen...$(RESET)"
	@cd $(DOCS_DIR)/Doxygen && doxygen Doxyfile
	@cd $(DOCS_DIR)/Doxygen/build/latex && $(MAKE)
	@mv $(DOCS_DIR)/Doxygen/build/latex/refman.pdf $(DOCS_DIR)/$(DOCS_NAME).pdf

docusaurus-build:
	@echo "$(CYAN)Building Docusaurus static site...$(RESET)"
	@cd $(DOCS_DIR)/docusaurus && npm run build

docusaurus-serve: docusaurus-build
	@echo "$(CYAN)Serving Docusaurus locally...$(RESET)"
	@cd $(DOCS_DIR)/docusaurus && npm run serve

docusaurus-start:
	@echo "$(CYAN)Starting Docusaurus development server...$(RESET)"
	@cd $(DOCS_DIR)/docusaurus && npm start

docusaurus-deploy:
	@echo "$(CYAN)Deploying Docusaurus to GitHub Pages...$(RESET)"
	@cd $(DOCS_DIR)/docusaurus && npm run deploy

style: fclean
	@echo "$(YELLOW)Running coding style checks...$(RESET)"
	@coding-style.sh src/ .
	@cat coding-style-reports.log
	@rm -rf coding-style-reports.log

style_cpp: fclean
	@echo "$(YELLOW)Running cpplint checks...$(RESET)"
	@python3 tools/cpplint.py $(GUI_DIR)
	@echo "$(GREEN)Cpplint checks done$(RESET)"

.PHONY: all zappy_server zappy_gui zappy_ai tests tests_run debug install_ai init clean fclean re \
docs docusaurus-build docusaurus-serve docusaurus-start docusaurus-deploy \
style style_cpp
