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
UNIT_TESTS_NAME=unit_tests
FUNCTIONAL_TESTS_NAME=functional_tests

# Directories
SERVER_DIR=src/Server
GUI_DIR=src/GUI
AI_DIR=src/AI
TESTS_DIR=tests
BUILD_DIR=build
DOCS_DIR=docs
PLUGINS_DIR=plugins
LIBS_DIR=libs

# Documentation
DOCS_NAME=ZappyDocumation

# Colors and animations
RED=\033[31m
GREEN=\033[32m
YELLOW=\033[33m
BLUE=\033[34m
MAGENTA=\033[35m
CYAN=\033[36m
WHITE=\033[37m
BOLD=\033[1m
RESET=\033[0m
BG_BLACK=\033[40m

all: display_banner
	@echo "$(CYAN)[$(BOLD)BUILDING$(RESET)$(CYAN)]$(RESET) $(BOLD)Compilation \
	de tous les composants...$(RESET)"
	@sleep 0.3
	@$(MAKE) -C $(SERVER_DIR)
	@$(MAKE) -C $(LIBS_DIR)
	@$(MAKE) -C $(GUI_DIR)
	@$(MAKE) -C $(AI_DIR)
	@echo ""
	@echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Tous les \
	binaires/bibliothèques ont été construits avec succès !$(RESET)"
	@echo "$(CYAN)╔═══════════════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Le projet Zappy est prêt à être \
	utilisé !$(RESET)        $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════════════╝$(RESET)"

coverage: display_banner
	@echo "$(CYAN)[$(BOLD)COVERAGE$(RESET)$(CYAN)]$(RESET) $(BOLD)Compilation \
	de tous les composants avec couverture...$(RESET)"
	@sleep 0.3
	@$(MAKE) -C $(SERVER_DIR) coverage
	@$(MAKE) -C $(LIBS_DIR) coverage
	@$(MAKE) -C $(GUI_DIR) coverage
	@$(MAKE) -C $(AI_DIR) coverage
	@echo ""
	@echo "$(GREEN)[$(BOLD)SUCCESS$(RESET)$(GREEN)]$(RESET) $(BOLD)Tous les \
	composants ont été construits avec couverture !$(RESET)"
	@echo "$(CYAN)╔═════════════════════════════════════════════════════════\
	════╗$(RESET)"
	@echo "$(CYAN)║  $(YELLOW)$(BOLD)Compilation avec instrumentation de \
	couverture terminée !$(RESET)  $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═════════════════════════════════════════════════════════\
	════╝$(RESET)"

# Display a fancy banner for the build process
display_banner:
	@clear
	@echo "$(CYAN)╔═══════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║  $(BG_BLACK)$(GREEN)$(BOLD)Z A P P Y   M A S T E R   \
	B U I L D E R$(RESET)  $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚═══════════════════════════════════════════╝$(RESET)"
	@echo ""

zappy_server:
	@echo "$(CYAN)[$(BOLD)INFO$(RESET)$(CYAN)]$(RESET) $(BOLD)Compilation \
	du serveur...$(RESET)"
	@$(MAKE) -C $(SERVER_DIR) all
	@echo "$(GREEN)[$(BOLD)OK$(RESET)$(GREEN)]$(RESET) $(BOLD)Serveur \
	compilé avec succès !$(RESET)"

zappy_gui:
	@$(MAKE) -C $(LIBS_DIR)
	@echo "$(CYAN)[$(BOLD)INFO$(RESET)$(CYAN)]$(RESET) $(BOLD)Compilation \
	de la GUI...$(RESET)"
	@$(MAKE) -C $(GUI_DIR) all
	@echo "$(GREEN)[$(BOLD)OK$(RESET)$(GREEN)]$(RESET) $(BOLD)GUI compilée \
	avec succès !$(RESET)"

zappy_ai:
	@echo "$(CYAN)[$(BOLD)INFO$(RESET)$(CYAN)]$(RESET) $(BOLD)Compilation \
	de l'IA...$(RESET)"
	@$(MAKE) -C $(AI_DIR) all
	@echo "$(GREEN)[$(BOLD)OK$(RESET)$(GREEN)]$(RESET) $(BOLD)IA compilée \
	avec succès !$(RESET)"

tests_run: all
	@echo "$(MAGENTA)[$(BOLD)TEST$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Compilation\
	 et exécution des tests unitaires...$(RESET)"
	@$(MAKE) -C $(TESTS_DIR) tests_run
	@echo "$(GREEN)[$(BOLD)OK$(RESET)$(GREEN)]$(RESET) $(BOLD)Tests unitaires\
	 exécutés !$(RESET)"

functional_tests: zappy_server zappy_gui zappy_ai
	@echo "$(MAGENTA)[$(BOLD)TEST$(RESET)$(MAGENTA)]$(RESET) $(BOLD)Compilation\
	 et exécution des tests fonctionnels...$(RESET)"
	@$(MAKE) -C $(TESTS_DIR) functional_tests
	@echo "$(GREEN)[$(BOLD)OK$(RESET)$(GREEN)]$(RESET) $(BOLD)Tests \
	fonctionnels exécutés !$(RESET)"

debug:
	@echo "$(CYAN)[$(BOLD)DEBUG$(RESET)$(CYAN)]$(RESET) $(BOLD)Compilation de\
	 tous les composants en mode debug...$(RESET)"
	@$(MAKE) -C $(SERVER_DIR) debug
	@$(MAKE) -C $(GUI_DIR) debug
	@$(MAKE) -C $(AI_DIR) debug
	@echo "$(GREEN)[$(BOLD)OK$(RESET)$(GREEN)]$(RESET) $(BOLD)Compilation\
	 en mode debug terminée !$(RESET)"

install_ai:
	@echo "$(CYAN)[$(BOLD)INSTALL$(RESET)$(CYAN)]$(RESET) $(BOLD)Installation\
	 des dépendances IA...$(RESET)"
	@$(MAKE) -C $(AI_DIR) install
	@echo "$(GREEN)[$(BOLD)OK$(RESET)$(GREEN)]$(RESET) $(BOLD)Dépendances\
	 IA installées avec succès !$(RESET)"

init:
	@echo "$(CYAN)[$(BOLD)INIT$(RESET)$(CYAN)]$(RESET) $(BOLD)Initialisation\
	 du projet...$(RESET)"
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
	@echo "$(UNIT_TESTS_NAME)" >> .gitignore
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Projet\
	 initialisé avec succès !$(RESET)"

clean:
	@echo "$(YELLOW)[$(BOLD)CLEAN$(RESET)$(YELLOW)]$(RESET)\
	 $(BOLD)Nettoyage des fichiers objets...$(RESET)"
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
	@for i in `seq 1 11`; do \
		progress=$$(( i > 10 ? 10 : i )); \
		echo -n "$(YELLOW)[$(BOLD)CLEAN$(RESET)$(YELLOW)]$(RESET)\
	 $(BOLD)Progression: ["; \
		for j in `seq 1 $$progress`; do echo -n "▓"; done; \
		for j in `seq $$progress 9`; do echo -n "░"; done; \
		echo -n "]$(RESET)\r"; \
		sleep 0.1; \
	done
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET)\
	 $(BOLD)Nettoyage terminé !$(RESET)       "

fclean: clean
	@echo "$(RED)[$(BOLD)FULL CLEAN$(RESET)$(RED)]$(RESET)\
	 $(BOLD)Nettoyage complet des binaires et bibliothèques...$(RESET)"
	@$(MAKE) -C $(SERVER_DIR) fclean
	@$(MAKE) -C $(GUI_DIR) fclean
	@$(MAKE) -C $(AI_DIR) fclean
	@$(MAKE) -C $(TESTS_DIR) fclean
	@rm -f $(SERVER_NAME)
	@rm -f $(GUI_NAME)
	@rm -f $(AI_NAME)
	@rm -f $(UNIT_TESTS_NAME)
	@rm -rf $(DOCS_DIR)/build
	@rm -f $(DOCS_NAME).pdf
	@rm -f *.html
	@rm -f *.css
	@rm -f *.js
	@rm -f *.gcda
	@rm -f *.gcno
	@rm -rf $(PLUGINS_DIR)/*.so
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET)\
	 $(BOLD)Nettoyage complet effectué !$(RESET)"

re: fclean all

docs:
	@echo "$(CYAN)[$(BOLD)DOCS$(RESET)$(CYAN)]$(RESET) $(BOLD)Génération\
	 de la documentation avec Doxygen...$(RESET)"
	@cd $(DOCS_DIR)/Doxygen && doxygen Doxyfile
	@cd $(DOCS_DIR)/Doxygen/build/latex && $(MAKE)
	@mv $(DOCS_DIR)/Doxygen/build/latex/refman.pdf $(DOCS_DIR)/$(DOCS_NAME).pdf
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Documentation\
	 générée avec succès : $(DOCS_DIR)/$(DOCS_NAME).pdf$(RESET)"

docusaurus-build:
	@echo "$(CYAN)[$(BOLD)DOCS$(RESET)$(CYAN)]$(RESET) $(BOLD)Construction\
	 du site statique Docusaurus...$(RESET)"
	@cd $(DOCS_DIR)/docusaurus && npm run build
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Site\
	 Docusaurus construit avec succès$(RESET)"

docusaurus-serve: docusaurus-build
	@echo "$(CYAN)[$(BOLD)DOCS$(RESET)$(CYAN)]$(RESET) $(BOLD)Démarrage\
	 du serveur Docusaurus local...$(RESET)"
	@cd $(DOCS_DIR)/docusaurus && npm run serve

docusaurus-start:
	@echo "$(CYAN)[$(BOLD)DOCS$(RESET)$(CYAN)]$(RESET) $(BOLD)Démarrage\
	 du serveur de développement Docusaurus...$(RESET)"
	@cd $(DOCS_DIR)/docusaurus && npm start

docusaurus-deploy:
	@echo "$(CYAN)[$(BOLD)DOCS$(RESET)$(CYAN)]$(RESET) $(BOLD)Déploiement\
	 de Docusaurus sur GitHub Pages...$(RESET)"
	@cd $(DOCS_DIR)/docusaurus && npm run deploy
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Déploiement\
	 effectué avec succès$(RESET)"

style: fclean
	@echo "$(YELLOW)[$(BOLD)STYLE$(RESET)$(YELLOW)]$(RESET) $(BOLD)Vérification\
	 du style de codage...$(RESET)"
	@coding-style.sh src/ .
	@cat coding-style-reports.log
	@rm -rf coding-style-reports.log
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Vérification\
	 du style terminée$(RESET)"

style_cpp: fclean
	@echo "$(YELLOW)[$(BOLD)STYLE$(RESET)$(YELLOW)]$(RESET) $(BOLD)Vérification\
	 du style avec cpplint...$(RESET)"
	@python3 tools/cpplint.py $(GUI_DIR)
	@echo "$(GREEN)[$(BOLD)DONE$(RESET)$(GREEN)]$(RESET) $(BOLD)Vérification\
	 cpplint terminée$(RESET)"

.PHONY: all zappy_server zappy_gui zappy_ai tests tests_run \
debug install_ai init clean fclean re coverage \
docs docusaurus-build docusaurus-serve docusaurus-start docusaurus-deploy \
style style_cpp test_network_gui