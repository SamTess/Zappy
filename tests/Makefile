##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile
##

RED=\033[31m
GREEN=\033[32m
YELLOW=\033[33m
MAGENTA=\033[35m
CYAN=\033[36m
RESET=\033[0m
BOLD=\033[1m

UNIT_TESTS_NAME = unit_tests

SERVER_SRC_DIR = ../src/Server
SERVER_INCLUDE_DIR = $(SERVER_SRC_DIR)/include

CC = gcc
CFLAGS_COMMON = -Wall -Wextra -g
TEST_CFLAGS = $(CFLAGS_COMMON) --coverage -I$(SERVER_INCLUDE_DIR) $(shell pkg-config --cflags criterion)
TEST_LDFLAGS = --coverage $(shell pkg-config --libs criterion) \
	-Wl,--wrap=write_command_output \
	-Wl,--wrap=send_ppo_command \
	-Wl,--wrap=command_pex \
	-Wl,--wrap=send_edi_command \
	-Wl,--wrap=tile_add_player \
	-Wl,--wrap=tile_remove_player \
	-Wl,--wrap=remove_egg \
	-Wl,--wrap=server_err \
	-Wl,--wrap=command_pie \
	-Wl,--wrap=command_seg \
	-Wl,--wrap=send_plv_to_all \
	-Wl,--wrap=can_start_incantation \
	-Wl,--wrap=command_pic \
	-Wl,--wrap=create_egg \
	-Wl,--wrap=add_egg \
	-Wl,--wrap=send_enw_command \
	-Wl,--wrap=command_pgt \
	-Wl,--wrap=command_pdr \
	-Wl,--wrap=arr_len

TESTS_SRC = $(SERVER_SRC_DIR)/buffer/circular_buffer.c \
	$(SERVER_SRC_DIR)/command/connect_nbr.c \
	$(SERVER_SRC_DIR)/command/eject.c \
	$(SERVER_SRC_DIR)/command/finish_incantation.c \
	$(SERVER_SRC_DIR)/command/fork.c \
	$(SERVER_SRC_DIR)/command/forward.c \
	$(SERVER_SRC_DIR)/command/inventory.c \
	$(SERVER_SRC_DIR)/command/left.c \
	$(SERVER_SRC_DIR)/command/look_utils.c \
	$(SERVER_SRC_DIR)/command/look.c \
	$(SERVER_SRC_DIR)/command/parse_command_utils_bis.c \
	$(SERVER_SRC_DIR)/command/parse_command_utils.c \
	$(SERVER_SRC_DIR)/command/parse_command.c \
	$(SERVER_SRC_DIR)/command/pending_cmd_utils.c \
	$(SERVER_SRC_DIR)/command/right.c \
	$(SERVER_SRC_DIR)/command/set_object.c \
	$(SERVER_SRC_DIR)/command/start_incantation.c \
	$(SERVER_SRC_DIR)/command/take_object.c \
	$(SERVER_SRC_DIR)/command/tile_to_str.c \
	$(SERVER_SRC_DIR)/graphical_command/command_bct.c \
	$(SERVER_SRC_DIR)/graphical_command/command_ebo.c \
	$(SERVER_SRC_DIR)/graphical_command/command_edi.c \
	$(SERVER_SRC_DIR)/graphical_command/command_enw.c \
	$(SERVER_SRC_DIR)/graphical_command/command_msz.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pbc.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pdi.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pdr.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pex.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pfk.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pgt.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pic.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pie.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pin.c \
	$(SERVER_SRC_DIR)/graphical_command/command_plv.c \
	$(SERVER_SRC_DIR)/graphical_command/command_pnw.c \
	$(SERVER_SRC_DIR)/graphical_command/command_ppo.c \
	$(SERVER_SRC_DIR)/graphical_command/command_seg.c \
	$(SERVER_SRC_DIR)/graphical_command/command_sgt.c \
	$(SERVER_SRC_DIR)/graphical_command/command_smg.c \
	$(SERVER_SRC_DIR)/graphical_command/command_sst.c \
	$(SERVER_SRC_DIR)/graphical_command/command_tna.c \
	$(SERVER_SRC_DIR)/map/egg.c \
	$(SERVER_SRC_DIR)/map/map_creation.c \
	$(SERVER_SRC_DIR)/map/resources_map.c \
	$(SERVER_SRC_DIR)/map/tick_update.c \
	$(SERVER_SRC_DIR)/map/tile.c \
	$(SERVER_SRC_DIR)/network/array_function.c \
	$(SERVER_SRC_DIR)/network/client_node.c \
	$(SERVER_SRC_DIR)/network/connection_err.c \
	$(SERVER_SRC_DIR)/network/connection_utils.c \
	$(SERVER_SRC_DIR)/network/connection.c \
	$(SERVER_SRC_DIR)/network/graphical_client.c \
	$(SERVER_SRC_DIR)/network/queue_connection.c \
	$(SERVER_SRC_DIR)/network/server_run.c \
	$(SERVER_SRC_DIR)/player/player_death.c \
	$(SERVER_SRC_DIR)/player/player_init.c \
	$(SERVER_SRC_DIR)/player/player_inventory_display.c \
	$(SERVER_SRC_DIR)/player/player_inventory_utils.c \
	$(SERVER_SRC_DIR)/player/player_inventory.c \
	$(SERVER_SRC_DIR)/free.c \
	$(SERVER_SRC_DIR)/help.c \
	$(SERVER_SRC_DIR)/parsing_err.c \
	$(SERVER_SRC_DIR)/parsing.c \
	./test_circular_buffer.c \
	./test_broadcast.c \
	./test_connect_nbr.c \
	./test_eject.c \
	./test_finish_incantation.c \
	./test_fork.c \
	./test_movement_commands.c \
	./test_mocks.c \
	# ./test_left_right.c \
	./test_inventory.c \
	./test_look.c \
	./test_take_set_object.c \
	./test_start_incantation.c \
	./test_parse_command.c \
	# ./test_tile_pending_utils.c \
	./test_forward.c \


all: tests_run

tests_run: fclean
	@echo "$(MAGENTA)[TESTS]$(RESET) Building and running unit tests..."
	@if [ -z "$(TESTS_SRC)" ]; then \
		echo "$(YELLOW)[WARNING]$(RESET) TESTS_SRC is empty in tests/Makefile. No tests to run."; \
		echo "Please define your test source files in tests/Makefile."; \
		exit 0; \
	fi
	@mkdir -p coverage
	@echo "$(CYAN)[COMPILE]$(RESET) $(CC) $(TEST_CFLAGS) $(TESTS_SRC) -o $(UNIT_TESTS_NAME) $(TEST_LDFLAGS)"
	@$(CC) $(TEST_CFLAGS) $(TESTS_SRC) -o $(UNIT_TESTS_NAME) $(TEST_LDFLAGS)
	@echo "$(CYAN)[RUN]$(RESET) ./$(UNIT_TESTS_NAME)"
	@./$(UNIT_TESTS_NAME)
	@echo "$(GREEN)[SUCCESS]$(RESET) Unit tests finished."
	@echo "$(CYAN)[COVERAGE]$(RESET) Moving coverage data files..."
	@mv -f ./*.gcda ./*.gcno coverage/ 2>/dev/null || echo "$(YELLOW)[INFO]$(RESET) No .gcda/.gcno files found to move."
	@$(MAKE) coverage_report

coverage_report:
	@if [ -z "$(TESTS_SRC)" ]; then \
		exit 0; \
	fi
	@echo "$(CYAN)[COVERAGE]$(RESET) Generating GCOVR HTML report..."
	@gcovr -r .. coverage/ \
		--html --html-details -o coverage/coverage.html \
		-e ".*tests/.*" \
		-e ".*/tests/.*" \
		-e "$(SERVER_SRC_DIR)/main.c" \
		--gcov-exclude="tests/*.c" \
		--gcov-exclude=".*test.*\\.c" \
		--gcov-exclude=".*_test\\.c" \
		--gcov-exclude=".*criterion.*" \
		--gcov-exclude="/usr/include/.*"
	@echo "$(CYAN)[COVERAGE]$(RESET) Generating GCOVR XML report..."
	@gcovr -r .. coverage/ \
		--xml -o coverage/coverage.xml \
		-e ".*tests/.*" \
		-e ".*/tests/.*" \
		-e "$(SERVER_SRC_DIR)/main.c" \
		--gcov-exclude="tests/*.c" \
		--gcov-exclude=".*test.*\\.c" \
		--gcov-exclude=".*_test\\.c" \
		--gcov-exclude=".*criterion.*" \
		--gcov-exclude="/usr/include/.*"
	@echo "$(CYAN)[COVERAGE]$(RESET) Generating GCOVR summary..."
	@gcovr -r .. coverage/ \
		--txt \
		-e ".*tests/.*" \
		-e ".*/tests/.*" \
		-e "$(SERVER_SRC_DIR)/main.c" \
		--gcov-exclude="tests/*.c" \
		--gcov-exclude=".*test.*\\.c" \
		--gcov-exclude=".*_test\\.c" \
		--gcov-exclude=".*criterion.*" \
		--gcov-exclude="/usr/include/.*"
	@echo "$(GREEN)[SUCCESS]$(RESET) Coverage report generated in tests/coverage/"

clean:
	@echo "$(YELLOW)[CLEAN]$(RESET) Cleaning test objects and coverage data from tests/ directory..."
	@rm -rf coverage
	@rm -f ./*.gcno ./*.gcda

fclean: clean
	@echo "$(RED)[FCLEAN]$(RESET) Full cleaning test binary from tests/ directory..."
	@rm -f $(UNIT_TESTS_NAME)

.PHONY: all tests_run clean fclean coverage_report
