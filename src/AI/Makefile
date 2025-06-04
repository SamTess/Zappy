##
## EPITECH PROJECT, 2025
## Zappy AI Makefile
## File description:
## Builds the Zappy AI client
##

# Binary name
NAME = ../../zappy_ai
BUILD_DIR = ../../build/AI

# Python files
PY_MAIN = main.py
PY_MODULES = logger/logger.py \
			 utils/input.py \
			 utils/zappy.py \
			 agent/agent.py \
			 parser/paringArgsClass.py \
			 defs/zappy.py

# Build script
BUILD_SCRIPT = build_zipapp/build_zipapp.py

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
BG_WHITE = \033[47m

# Main target
all: setup_dirs display_info $(NAME)

# Create the necessary build directories
setup_dirs:
	@mkdir -p $(BUILD_DIR)

# Display fancy introduction
display_info:
	@clear
	@echo "$(CYAN)╔═══════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(BG_BLACK)$(GREEN)    $(BOLD)Z A P P Y   A I   \
	B U I L D E R    $(RESET)  $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════╝$(RESET)"
	@echo ""
	@sleep 0.5
	@echo "$(BLUE)[$(BOLD)INFO$(RESET)$(BLUE)]$(RESET) $(BOLD)Préparation \
	de l'environnement Python...$(RESET)"
	@sleep 0.5

# Build the AI client
$(NAME): check_python
	@echo "$(MAGENTA)[$(BOLD)BUILDING$(RESET)$(MAGENTA)]$(RESET) $(BOLD)\
	Création du client AI...$(RESET)"
	@echo ""
	@for i in `seq 1 21`; do \
		progress=$$(( i > 20 ? 20 : i )); \
		echo -n "$(GREEN)[$(BOLD)PROGRESS$(RESET)$(GREEN)]$(RESET) $(BOLD)["; \
		for j in `seq 1 $$progress`; do echo -n "▰"; done; \
		for j in `seq $$progress 19`; do echo -n "▱"; done; \
		echo -n "] $$(( i > 20 ? 100 : 5 * i ))%$(RESET)\r"; \
		sleep 0.05; \
	done
	@echo ""
	@cd build_zipapp && python3 build_zipapp.py
	@mkdir -p ../../build
	@cp -r . ../../build/AI/src
	@echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Client \
	AI compilé avec succès : $(NAME)$(RESET)"
	@sleep 0.2
	@echo ""
	@echo "$(CYAN)╔═════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Le client AI est prêt à être utilisé \
	!$(RESET)     $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════════╝$(RESET)"

# Check Python installation
check_python:
	@which python3 > /dev/null 2>&1 || (echo "$(RED)[$(BOLD)ERROR\
	$(RESET)$(RED)]$(RESET) $(BOLD)Python 3 n'est pas installé.$(RESET)" \
	&& exit 1)
	@echo "$(GREEN)[$(BOLD)OK$(RESET)$(GREEN)]$(RESET) $(BOLD)Python \
	3 trouvé.$(RESET)"

# Install dependencies
install:
	@echo "$(BLUE)[$(BOLD)INSTALL$(RESET)$(BLUE)]$(RESET) $(BOLD)Installation \
	des dépendances...$(RESET)"
	@pip3 install --upgrade pip
	@pip3 install wheel setuptools
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Dépendances \
	installées.$(RESET)"

# Clean build files
clean:
	@echo "$(YELLOW)[$(BOLD)CLEANING$(RESET)$(YELLOW)]$(RESET) \
	$(BOLD)Nettoyage des fichiers temporaires...$(RESET)"
	@find . -name "*.pyc" -delete
	@find . -name "__pycache__" -delete
	@rm -rf $(BUILD_DIR)
	@rm -rf *.egg-info
	@rm -rf build/
	@rm -rf dist/
	@sleep 0.3
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) \
	$(BOLD)Nettoyage effectué.$(RESET)"

# Full clean
fclean: clean
	@echo "$(RED)[$(BOLD)FULL CLEAN$(RESET)$(RED)]$(RESET) \
	$(BOLD)Suppression du client AI...$(RESET)"
	@rm -f $(NAME)
	@sleep 0.3
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) \
	$(BOLD)Nettoyage complet effectué.$(RESET)"

# Clean and rebuild
re: fclean all

# Debug (Just for compatibility with other Makefiles)
debug: all
	@echo "$(MAGENTA)[$(BOLD)NOTE$(RESET)$(MAGENTA)]$(RESET) \
	$(BOLD)Pas de mode debug spécifique pour le client Python.$(RESET)"

# Coverage build for tests
coverage: all
	@echo "$(MAGENTA)[$(BOLD)COVERAGE$(RESET)$(MAGENTA)]$(RESET) \
	$(BOLD)Mode couverture pour Python...$(RESET)"
	@echo "$(CYAN)╔═════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Coverage avec Python \
	(via pytest-cov)$(RESET)      $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════════╝$(RESET)"
	@echo "$(BLUE)[$(BOLD)INFO$(RESET)$(BLUE)]$(RESET) \
	$(BOLD)Pour exécuter les tests avec couverture, utilisez:$(RESET)"
	@echo "$(GREEN)python3 -m pytest --cov=. tests/$(RESET)"

.PHONY: all clean fclean re install debug check_python setup_dirs display_info
