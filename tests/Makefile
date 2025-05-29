##
## EPITECH PROJECT, 2025
## Zappy Tests
## File description:
## Tests Makefile
##

CC				=	gcc
CXX				=	g++
CFLAGS			=	-Wextra -Wall -Werror -std=gnu17 --coverage
CXXFLAGS		=	-Wextra -Wall -Werror -std=c++20 --coverage
CPP_TEST_FLAGS	=	--coverage -lcriterion -include cstdint
LDFLAGS			=	-lcriterion --coverage
SFMLFLAGS		=	-lsfml-graphics -lsfml-window -lsfml-system \
					-lsfml-audio -lsfml-network

BUILD_DIR		=	../build/tests
SERVER_LIB		=	../build/libserver.a
GUI_LIB			=	../build/libgui.a

SERVER_TESTS	=	server/test_server.c server/test_player_inventory.c
CLIENT_TESTS	=	client/test_client.cpp
ALL_TESTS		=	$(SERVER_TESTS) $(CLIENT_TESTS)

SERVER_TEST_OBJS = $(SERVER_TESTS:%.c=$(BUILD_DIR)/%.o)
CLIENT_TEST_OBJS = $(CLIENT_TESTS:%.cpp=$(BUILD_DIR)/%.o)
ALL_TEST_OBJS	=	$(SERVER_TEST_OBJS) $(CLIENT_TEST_OBJS)

SERVER_INCLUDE	=	-I../src/Server/include
GUI_INCLUDE		=	-I../src/GUI/include
SHARED_INCLUDE	=	-I../src/Shared/include
ALL_INCLUDES	=	$(SERVER_INCLUDE) $(GUI_INCLUDE) $(SHARED_INCLUDE)

SERVER_TESTS_NAME = ../server_tests
CLIENT_TESTS_NAME = ../client_tests
COVERAGE_DIR	=	$(BUILD_DIR)/coverage

# Colors
RED				=	\033[0;91m
GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m
BLUE			=	\033[0;94m
MAGENTA			=	\033[0;95m
CYAN			=	\033[0;96m
RESET			=	\033[0m

all: $(SERVER_TESTS_NAME) $(CLIENT_TESTS_NAME)

$(SERVER_TESTS_NAME): $(SERVER_TEST_OBJS) $(SERVER_LIB)
	@echo -e "$(GREEN)Compiling server test binary...$(RESET)"
	@$(CC) -o $(SERVER_TESTS_NAME) $(SERVER_TEST_OBJS) $(SERVER_LIB) \
		$(CFLAGS) $(SERVER_INCLUDE) $(SHARED_INCLUDE) $(LDFLAGS)

$(CLIENT_TESTS_NAME): $(GUI_LIB)
	@echo -e "$(GREEN)Compiling client test binary...$(RESET)"
	@mkdir -p $(dir $(BUILD_DIR)/client/)
	@$(CXX) -o $(CLIENT_TESTS_NAME) $(CLIENT_TESTS) $(GUI_LIB) \
		$(CPP_TEST_FLAGS) $(GUI_INCLUDE) $(SHARED_INCLUDE) $(SFMLFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo -e "$(BLUE)Compiling C test file $< into $@...$(RESET)"
	@$(CC) $(CFLAGS) -fprofile-abs-path $(SERVER_INCLUDE) $(SHARED_INCLUDE) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo -e "$(BLUE)Compiling C++ test file $< into $@...$(RESET)"
	@$(CXX) $(CXXFLAGS) -fprofile-abs-path $(GUI_INCLUDE) $(SHARED_INCLUDE) -c $< -o $@

$(SERVER_LIB):
	@echo -e "$(CYAN)Building server library for tests...$(RESET)"
	@$(MAKE) ../../build/libserver.a -C ../src/Server

$(GUI_LIB):
	@echo -e "$(CYAN)Building GUI library for tests...$(RESET)"
	@$(MAKE) ../../build/libgui.a -C  ../src/GUI

tests_run: $(SERVER_TESTS_NAME) $(CLIENT_TESTS_NAME)
	@echo -e "$(MAGENTA)Running server unit tests...$(RESET)"
	@./$(SERVER_TESTS_NAME)
	@echo -e "$(MAGENTA)Running client unit tests...$(RESET)"
	@./$(CLIENT_TESTS_NAME)
	@echo -e "$(CYAN)Generating coverage report...$(RESET)"
	@mkdir -p $(COVERAGE_DIR)

	@echo -e "$(GREEN)Coverage report generated at $(COVERAGE_DIR)/coverage.html$(RESET)"

server_tests: $(SERVER_TESTS_NAME)
	@echo -e "$(MAGENTA)Running server unit tests only...$(RESET)"
	@./$(SERVER_TESTS_NAME)

client_tests: $(CLIENT_TESTS_NAME)
	@echo -e "$(MAGENTA)Running client unit tests only...$(RESET)"
	@./$(CLIENT_TESTS_NAME)

clean:
	@echo -e "$(YELLOW)Cleaning test objects...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@rm -f *.gcda *.gcno
	@rm -f */*.gcda */*.gcno
	@rm -f ../client_tests-*.gc*
	@rm -f ../server_tests-*.gc*

fclean: clean
	@echo -e "$(RED)Cleaning test binaries...$(RESET)"
	@rm -f $(SERVER_TESTS_NAME) $(CLIENT_TESTS_NAME)

re: fclean all

.PHONY: all tests_run server_tests client_tests clean fclean re