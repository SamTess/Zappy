##
## EPITECH PROJECT, 2025
## Zappy GUI
## File description:
## GUI Makefile
##

CXX					= 	g++
CXXFLAGS			=	-Wextra -Wall -Werror -std=c++20
DEBUGFLAGS			=	-ggdb -fsanitize=address
SFMLFLAGS			=	-lsfml-graphics -lsfml-window -lsfml-system \
						-lsfml-audio -lsfml-network

# Directories
SRC_DIR				=	.
INCLUDE_DIR			=	include
SHARED_INCLUDE		=	../Shared/include
BUILD_DIR			= ../../build
PLUGINS_DIR			= ../../plugins

MAIN_FILE 		= 	main.cpp

# Source files
SRCS			=	shared/exception/AException.cpp \
					parsing/ParsingCLI.cpp \
					network/buffer/CircularBuffer.cpp \
					network/buffer/MessageQueue.cpp \
					network/connection/TcpConnection.cpp \
					network/networkManager/NetworkManager.cpp \
					network/networkThread/NetworkThread.cpp \
					network/protocol/Message.cpp \
					network/protocol/ProtocolParser.cpp \
					graphicalContext/GraphicalContext.cpp \

OBJ_FILES			= $(SRCS:%.cpp=$(BUILD_DIR)/GUI/%.o)
LIB_NAME			= $(BUILD_DIR)/libgui.a
BINARY_NAME			= ../../zappy_gui
GRAPHIC_LIB_NAME	= $(PLUGINS_DIR)/libraylibcpp.so
GUI_LIB_NAME		= $(PLUGINS_DIR)/libraygui.so

INCLUDES			=	-Iinclude -I../Shared/include

RED					=	\033[0;91m
GREEN				=	\033[0;92m
YELLOW				=	\033[0;93m
BLUE				=	\033[0;94m
CYAN				=	\033[0;96m
RESET				=	\033[0m

all: libs_so $(LIB_NAME) $(BINARY_NAME)

libs_so:
	@$(MAKE) -C ../../libs


$(BUILD_DIR)/Shared/%.o: ../Shared/%.cpp
	@mkdir -p $(BUILD_DIR)/Shared
	@echo -e "$(BLUE)Compiling $< into $@...$(RESET)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(LIB_NAME): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	@ar rcs $(LIB_NAME) $(OBJ_FILES)

$(BINARY_NAME): $(LIB_NAME)
	@echo -e "$(GREEN)Compiling GUI binary...$(RESET)"
	@$(CXX) -o $(BINARY_NAME) $(MAIN_FILE) $(LIB_NAME) \
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
