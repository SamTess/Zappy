##
## EPITECH PROJECT, 2025
## Makefile
## File description:
## Makefile
##

CC			=	gcc
++			=	g++

MAIN_SERVER	=	src/Server/main.c
MAIN_GUI	=	src/GUI/main.cpp
MAIN_AI		=	src/AI/main.c

SRCS_SERVER	=	src/Server/help.c
SRCS_GUI =	src/GUI/help.cpp

TESTS_FILES	=	tests/UnitTests/GUI/test_GUI.cpp \
				tests/UnitTests/server/test_server.c


TESTS_NAME		=	unit_tests


FUNC_MAIN1		=	tests/Functionnal/test_binary_handler.c
FUNC_MAIN2		=	tests/Functionnal/test_server.c
FUNC_MAIN3		=	tests/Functionnal/test_main.c
TEST_MAIN1		=	tests/Functionnal/Test1
TEST_MAIN2		=	tests/Functionnal/Test2
TEST_MAIN3		=	tests/Functionnal/Test3

CFLAGS 			= 	-Wextra -Wall -Werror -std=gnu17
CPPFLAGS		= 	-Wextra -Wall -Werror -std=c++20
DEBUGFLAGS		= 	-ggdb -fsanitize=address
LDFLAGS			= 	-lcriterion --coverage
SFMLFLAGS		= 	-lsfml-graphics -lsfml-window -lsfml-system \
					-lsfml-audio -lsfml-network


OBJ_SERVER 		=	$(SRCS_SERVER:src/%.c=$(BUILD_DIR)/%.o)
OBJ_GUI 		=	$(SRCS_GUI:src/%.cpp=$(BUILD_DIR)/%.o)
OBJ_AI 		    =	$(SRCS_AI:src/%.c=$(BUILD_DIR)/%.o)
LIB_SERVER_NAME	= 	build/libserver.a
LIB_GUI_NAME	= 	build/libgui.a
SERVER_NAME 	= 	zappy_server
GUI_NAME 	= 	zappy_gui
SERVER_INCLUDE	= 	-I./src/Server/include
GUI_INCLUDE	= 	-I./src/GUI/
SHARED_INCLUDE	= 	-I./src/Shared/include
DOCS_NAME	= 	ZappyDocumation
BUILD_DIR	= 	build
OBJ_SERVER_TESTS = $(patsubst tests/UnitTests/%.c,$(BUILD_DIR)/tests/%.o, \
	$(filter %.c,$(TESTS_FILES)))
OBJ_GUI_TESTS = $(patsubst tests/UnitTests/%.cpp,$(BUILD_DIR)/tests/%.o,  \
	$(filter %.cpp,$(TESTS_FILES)))

# Coverage files location
COVERAGE_DIR = $(BUILD_DIR)/tests/coverage

# Coverage-enabled object files for tests
OBJ_SERVER_COV = $(SRCS_SERVER:src/%.c=$(COVERAGE_DIR)/%.o)
OBJ_GUI_COV = $(SRCS_GUI:src/%.cpp=$(COVERAGE_DIR)/%.o)

# Colors
RED		= \033[31m
GREEN	= \033[32m
YELLOW	= \033[33m
BLUE	= \033[34m
MAGENTA	= \033[35m
CYAN	= \033[36m
RESET	= \033[0m

all: $(SERVER_NAME) $(GUI_NAME)

GUI: $(GUI_NAME)

server: $(SERVER_NAME)

$(SERVER_NAME): $(OBJ_SERVER)
	@echo "$(CYAN)Archiving object files into $(LIB_SERVER_NAME)...$(RESET)"
	@ar rcs $(LIB_SERVER_NAME) $(OBJ_SERVER)
	@echo "$(GREEN)Compiling the binary $(SERVER_NAME)...$(RESET)"
	@$(CC) -o $(SERVER_NAME) $(MAIN_SERVER) $(OBJ_SERVER) $(LIB_SERVER_NAME) \
		$(CFLAGS) $(SERVER_INCLUDE) $(SHARED_INCLUDE)

$(GUI_NAME): $(OBJ_GUI)
	@echo "$(CYAN)Archiving object files into $(LIB_GUI_NAME)...$(RESET)"
	@ar rcs $(LIB_GUI_NAME) $(OBJ_GUI)
	@echo "$(GREEN)Compiling the binary $(GUI_NAME)...$(RESET)"
	@$(++) -o $(GUI_NAME) $(MAIN_GUI) $(OBJ_GUI) $(LIB_GUI_NAME) \
		$(CPPFLAGS) $(GUI_INCLUDE) $(SHARED_INCLUDE) $(SFMLFLAGS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)/$(dir $*)
	@echo "$(BLUE)Compiling source file $< into $@...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)/$(dir $*)
	@echo "$(BLUE)Compiling source file $< into $@...$(RESET)"
	@$(++) $(CPPFLAGS) $(GUI_INCLUDE) -c $< -o $@

$(COVERAGE_DIR)/%.o: src/%.c
	@mkdir -p $(COVERAGE_DIR)/$(dir $*)
	@echo "$(BLUE)Compiling source file $< with coverage into $@...$(RESET)"
	@$(CC) $(CFLAGS) --coverage -c $< -o $@

$(COVERAGE_DIR)/%.o: src/%.cpp
	@mkdir -p $(COVERAGE_DIR)/$(dir $*)
	@echo "$(BLUE)Compiling source file $< with coverage into $@...$(RESET)"
	@$(++) $(CPPFLAGS) --coverage $(GUI_INCLUDE) -c $< -o $@

$(BUILD_DIR)/tests/%.o: tests/UnitTests/%.c
	@mkdir -p $(dir $@)
	@echo "$(BLUE)Compiling test file $< into $@...$(RESET)"
	@$(CC) $(CFLAGS) --coverage $(SERVER_INCLUDE) $(SHARED_INCLUDE) -c $< -o $@

$(BUILD_DIR)/tests/%.o: tests/UnitTests/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(BLUE)Compiling test file $< into $@...$(RESET)"
	@$(++) $(CPPFLAGS) --coverage $(GUI_INCLUDE) \
		$(SHARED_INCLUDE) -c $< -o $@


tests: $(OBJ_SERVER_TESTS) $(OBJ_GUI_TESTS) $(OBJ_SERVER_COV) \
	$(OBJ_GUI_COV)
	@echo "$(GREEN)Compiling the combined test binary $(TESTS_NAME)...$(RESET)"
	@mkdir -p $(BUILD_DIR)
	@$(++) -o $(TESTS_NAME) $(OBJ_SERVER_TESTS) $(OBJ_GUI_TESTS) \
		$(OBJ_SERVER_COV) $(OBJ_GUI_COV) \
		$(CPPFLAGS) $(SERVER_INCLUDE) $(GUI_INCLUDE) \
		$(SHARED_INCLUDE) $(LDFLAGS) $(SFMLFLAGS)

tests_run: tests
	@echo "$(MAGENTA)Running the unit tests...$(RESET)"
	@./$(TESTS_NAME)
	@echo "$(CYAN)Generating coverage report...$(RESET)"
	@gcovr --exclude tests/ --exclude build/ -r . --object-directory \
		$(COVERAGE_DIR)
	@gcovr --exclude tests/ --exclude build/ -r . --object-directory \
		$(COVERAGE_DIR) --html --html-details -o $(COVERAGE_DIR)/coverage.html
	@echo "$(GREEN)Coverage report generated at $(COVERAGE_DIR)$(RESET)"
	@echo "$(GREEN)Units tests job done!$(RESET)"

debug: $(OBJ_SERVER) $(OBJ_GUI)
	@echo "$(CYAN)Archiving object files into $(LIB_SERVER_NAME)...$(RESET)"
	@ar rcs $(LIB_SERVER_NAME) $(OBJ_SERVER)
	@echo "$(CYAN)Compiling the server in debug mode...$(RESET)"
	@$(CC) -o $(SERVER_NAME) $(MAIN_SERVER) $(OBJ_SERVER) $(LIB_SERVER_NAME) \
		$(CFLAGS) $(DEBUGFLAGS) $(SERVER_INCLUDE) $(SHARED_INCLUDE)
	@echo "$(CYAN)Archiving object files into $(LIB_GUI_NAME)...$(RESET)"
	@ar rcs $(LIB_GUI_NAME) $(OBJ_GUI)
	@echo "$(CYAN)Compiling the GUI in debug mode...$(RESET)"
	@$(++) -o $(GUI_NAME) $(MAIN_GUI) $(OBJ_GUI) $(LIB_GUI_NAME) \
		$(CPPFLAGS) $(DEBUGFLAGS) $(GUI_INCLUDE) $(SHARED_INCLUDE) \
		$(SFMLFLAGS)

init:
	@echo "$(CYAN)Initializing .gitignore...$(RESET)"
	@touch .gitignore
	@echo ".vscode" >> .gitignore
	@echo "*.o" >> .gitignore
	@echo "coding-style-reports.log" >> .gitignore
	@echo $(NAME) >> .gitignore

clean:
	@echo "$(YELLOW)Cleaning up temporary files...$(RESET)"
	@rm -f *~
	@rm -rf $(BUILD_DIR)
	@rm -f *.gcda *.gcno
	@rm -f src/*.gcda src/*.gcno
	@rm -f src/*/*.gcda src/*/*.gcno
	@rm -f *.log
	@rm -f vgcore.*
	@rm -f coverage.html
	@rm -rf coverage_html

fclean: clean
	@echo "$(RED)Cleaning up build artifacts...$(RESET)"
	@rm -f $(SERVER_NAME)
	@rm -f $(GUI_NAME)
	@rm -f $(LIB_SERVER_NAME)
	@rm -f $(LIB_GUI_NAME)
	@rm -f $(TESTS_NAME)
	@rm -rf docs/build
	@rm -rf build
	@rm -f $(DOCS_NAME).pdf
	@rm -f *.html
	@rm -f *.css
	@rm -f *.js


re: fclean all

docs:
	@echo "$(CYAN)Generating documentation with Doxygen...$(RESET)"
	@cd docs/Doxygen && doxygen Doxyfile
	@cd docs/Doxygen/build/latex && $(MAKE)
	@mv docs/Doxygen/build/latex/refman.pdf docs/$(DOCS_NAME).pdf

docusaurus-build:
	@echo "$(CYAN)Building Docusaurus static site...$(RESET)"
	@cd docs/docusaurus && npm run build

docusaurus-serve: docusaurus-build
	@echo "$(CYAN)Serving Docusaurus locally...$(RESET)"
	@cd docs/docusaurus && npm run serve

docusaurus-start:
	@echo "$(CYAN)Starting Docusaurus development server...$(RESET)"
	@cd docs/docusaurus && npm start

docusaurus-deploy:
	@echo "$(CYAN)Deploying Docusaurus to GitHub Pages...$(RESET)"
	@cd docs/docusaurus && npm run deploy

style: fclean
	@echo "$(YELLOW)Running coding style checks...$(RESET)"
	@coding-style.sh . .
	@cat coding-style-reports.log
	@rm -rf coding-style-reports.log

style_cpp: fclean
	@echo "$(YELLOW)Running cpplint checks...$(RESET)"
	@python3 tools/cpplint.py src/GUI
	@echo "$(GREEN)Cpplint checks done$(RESET)"

.PHONY: all tests tests_run fclean re all init docs tests_func_server \
	style_cpp server GUI docusaurus-build docusaurus-serve docusaurus-start docusaurus-deploy
