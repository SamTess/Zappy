##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile
##

NAME = ../../zappy_ai
BUILD_DIR = ../../build/AI

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

all: setup_dirs display_info $(NAME)

setup_dirs:
	@mkdir -p $(BUILD_DIR)

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
	@chmod +x ./main.py
	@ln -sf ./src/AI/main.py ../../zappy_ai
	@echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Client \
	AI compilé avec succès : $(NAME)$(RESET)"
	@sleep 0.2
	@echo ""
	@echo "$(CYAN)╔═════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Le client AI est prêt à être utilisé \
	!$(RESET)     $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════════╝$(RESET)"

check_python:
	@which python3 > /dev/null 2>&1 || (echo "$(RED)[$(BOLD)ERROR\
	$(RESET)$(RED)]$(RESET) $(BOLD)Python 3 n'est pas installé.$(RESET)" \
	&& exit 1)
	@echo "$(GREEN)[$(BOLD)OK$(RESET)$(GREEN)]$(RESET) $(BOLD)Python \
	3 trouvé.$(RESET)"

install:
	@echo "$(BLUE)[$(BOLD)INSTALL$(RESET)$(BLUE)]$(RESET) $(BOLD)Installation \
	des dépendances...$(RESET)"
	@pip3 install --upgrade pip
	@pip3 install wheel setuptools
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Dépendances \
	installées.$(RESET)"

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

fclean: clean
	@echo "$(RED)[$(BOLD)FULL CLEAN$(RESET)$(RED)]$(RESET) \
	$(BOLD)Suppression du client AI...$(RESET)"
	@rm -f $(NAME)
	@sleep 0.3
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) \
	$(BOLD)Nettoyage complet effectué.$(RESET)"

re: fclean all

debug: all
	@echo "$(MAGENTA)[$(BOLD)NOTE$(RESET)$(MAGENTA)]$(RESET) \
	$(BOLD)Pas de mode debug spécifique pour le client Python.$(RESET)"

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
