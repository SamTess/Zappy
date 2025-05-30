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
LDFLAGS			=	-lcriterion --coverage
SFMLFLAGS		=	-lsfml-graphics -lsfml-window -lsfml-system \
					-lsfml-audio -lsfml-network

BUILD_DIR		=	../build/tests
SERVER_LIB		=	../build/libserver.a
GUI_LIB			=	../build/libgui.a

SERVER_TESTS	=	server/test_server.c
CLIENT_TESTS	=	client/test_client.cpp \
					client/test_circular_buffer.cpp \
					client/test_message_queue.cpp \
					client/test_network_manager.cpp \
					client/test_protocol_parser.cpp \
					client/test_tcp_connection.cpp \
					client/test_network_thread.cpp \
					client/test_network_manager_integration.cpp \
					client/test_network_manager_detailed.cpp \
					client/test_network_manager_mocking.cpp
ALL_TESTS		=	$(SERVER_TESTS) $(CLIENT_TESTS)

SERVER_TEST_OBJS = $(SERVER_TESTS:%.c=$(BUILD_DIR)/%.o)
CLIENT_TEST_OBJS = $(CLIENT_TESTS:%.cpp=$(BUILD_DIR)/%.o)
ALL_TEST_OBJS	=	$(SERVER_TEST_OBJS) $(CLIENT_TEST_OBJS)

SERVER_INCLUDE	=	-I../src/Server/include
GUI_INCLUDE		=	-I../src/GUI
SHARED_INCLUDE	=	-I../src/Shared
ALL_INCLUDES	=	$(SERVER_INCLUDE) $(GUI_INCLUDE) $(SHARED_INCLUDE)

TESTS_NAME		=	../unit_tests
COVERAGE_DIR	=	$(BUILD_DIR)/coverage

# Colors
RED				=	\033[0;91m
GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m
BLUE			=	\033[0;94m
MAGENTA			=	\033[0;95m
CYAN			=	\033[0;96m
RESET			=	\033[0m

all: $(TESTS_NAME)

$(TESTS_NAME): $(ALL_TEST_OBJS) $(SERVER_LIB) $(GUI_LIB)
	@echo -e "$(GREEN)Compiling test binary...$(RESET)"
	@$(CXX) -o $(TESTS_NAME) $(ALL_TEST_OBJS) $(SERVER_LIB) $(GUI_LIB) \
		$(CXXFLAGS) $(ALL_INCLUDES) $(LDFLAGS) $(SFMLFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo -e "$(BLUE)Compiling test file $< into $@...$(RESET)"
	@$(CC) $(CFLAGS) -fprofile-abs-path $(ALL_INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo -e "$(BLUE)Compiling test file $< into $@...$(RESET)"
	@$(CXX) $(CXXFLAGS) -fprofile-abs-path $(ALL_INCLUDES) -c $< -o $@

$(SERVER_LIB):
	@echo -e "$(CYAN)Building server library for tests...$(RESET)"
	@$(MAKE) ../../build/libserver.a -C ../src/Server

$(GUI_LIB):
	@echo -e "$(CYAN)Building GUI library for tests...$(RESET)"
	@$(MAKE) ../../build/libgui.a -C  ../src/GUI

tests_run: $(TESTS_NAME)
	@echo -e "$(MAGENTA)Running unit tests...$(RESET)"
	@./$(TESTS_NAME)
	@echo -e "$(CYAN)Generating coverage report...$(RESET)"
	@mkdir -p $(COVERAGE_DIR)
	@cd .. && gcovr --exclude tests/ --exclude build/ --exclude 'tests/.*' --exclude 'tests/client/.*' --exclude 'tests/server/.*' -r . --object-directory build/tests
	@cd .. && gcovr --exclude tests/ --exclude build/ --exclude 'tests/.*' --exclude 'tests/client/.*' --exclude 'tests/server/.*' -r . --object-directory build/tests \
		--html --html-details -o build/tests/coverage/coverage.html
	@echo -e "$(GREEN)Coverage report generated at $(COVERAGE_DIR)/coverage.html$(RESET)"

clean:
	@echo -e "$(YELLOW)Cleaning test objects...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@rm -f *.gcda *.gcno
	@rm -f */*.gcda */*.gcno

fclean: clean
	@echo -e "$(RED)Cleaning test binary...$(RESET)"
	@rm -f $(TESTS_NAME)

re: fclean all

.PHONY: all tests_run clean fclean re