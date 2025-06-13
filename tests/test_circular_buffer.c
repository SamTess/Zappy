/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_circular_buffer
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "circular_buffer.h"
#include <string.h>
#include <stdlib.h>

// Helper function to initialize and fill buffer for tests
static void setup_buffer(circular_buffer_t *cb, const char *data)
{
    init_circular_buffer(cb);
    if (data) {
        for (size_t i = 0; i < strlen(data); ++i) {
            add_to_circular_buffer(cb, data[i]);
        }
    }
}

Test(circular_buffer, init_circular_buffer) {
    circular_buffer_t cb;
    init_circular_buffer(&cb);
    cr_assert_eq(cb.start, 0, "start should be 0 after init");
    cr_assert_eq(cb.end, 0, "end should be 0 after init");
    cr_assert_eq(cb.count, 0, "count should be 0 after init");
}

Test(circular_buffer, add_to_circular_buffer_simple) {
    circular_buffer_t cb;
    init_circular_buffer(&cb);
    int ret = add_to_circular_buffer(&cb, 'a');
    cr_assert_eq(ret, 0, "add_to_circular_buffer should return 0 on success");
    cr_assert_eq(cb.count, 1, "count should be 1 after adding one byte");
    cr_assert_eq(cb.buffer[0], 'a', "buffer should contain 'a' at index 0");
    cr_assert_eq(cb.end, 1, "end should be 1");
}

Test(circular_buffer, add_to_circular_buffer_full) {
    circular_buffer_t cb;
    init_circular_buffer(&cb);
    for (int i = 0; i < BUFFER_SIZE - 1; ++i) {
        add_to_circular_buffer(&cb, 'x');
    }
    cr_assert_eq(cb.count, BUFFER_SIZE - 1, "Buffer should be full");
    int ret = add_to_circular_buffer(&cb, 'y');
    cr_assert_eq(ret, -1, "add_to_circular_buffer should return -1 when buffer is full");
    cr_assert_eq(cb.count, BUFFER_SIZE - 1, "Count should remain BUFFER_SIZE - 1");
}

Test(circular_buffer, add_to_circular_buffer_wrapping) {
    circular_buffer_t cb;
    init_circular_buffer(&cb);
    // Fill the buffer almost to the end
    cb.start = 5;
    cb.end = (cb.start + BUFFER_SIZE - 5) % BUFFER_SIZE; // end is 0 if BUFFER_SIZE is multiple of 5
    cb.count = BUFFER_SIZE - 5;

    for(int i = 0; i < 4; ++i) { // Add 4 more items
        add_to_circular_buffer(&cb, 'a' + i);
    }
    cr_assert_eq(cb.count, BUFFER_SIZE - 1, "Count should be BUFFER_SIZE - 1");
    cr_assert_eq(cb.end, (0 + 4) % BUFFER_SIZE, "End should wrap around correctly");
    cr_assert_eq(cb.buffer[0], 'a');
    cr_assert_eq(cb.buffer[1], 'b');
    cr_assert_eq(cb.buffer[2], 'c');
    cr_assert_eq(cb.buffer[3], 'd');
}

Test(circular_buffer, find_command_end_simple_newline) {
    circular_buffer_t cb;
    setup_buffer(&cb, "command1\ncommand2");
    int len = find_command_end(&cb);
    cr_assert_eq(len, 9, "Should find newline at index 8 (length 9)");
}

Test(circular_buffer, find_command_end_crlf_is_error_case_in_func) {
    circular_buffer_t cb;
    setup_buffer(&cb, "command1\r\ncommand2");
    int len = find_command_end(&cb);
    // The function's logic `(cb->buffer[pos] == '\r' && cb->buffer[pos + 1] == '\n')` returns -1
    cr_assert_eq(len, -1, "Should return -1 for \\r\\n as per current function logic");
}

Test(circular_buffer, find_command_end_cr_null_is_error_case_in_func) {
    circular_buffer_t cb;
    init_circular_buffer(&cb);
    add_to_circular_buffer(&cb, 'c');
    add_to_circular_buffer(&cb, 'm');
    add_to_circular_buffer(&cb, 'd');
    add_to_circular_buffer(&cb, '1');
    add_to_circular_buffer(&cb, '\r');
    add_to_circular_buffer(&cb, '\0');
    add_to_circular_buffer(&cb, 'x');
    int len = find_command_end(&cb);
    // The function's logic `(cb->buffer[pos] == '\r' && cb->buffer[pos + 1] == '\0')` returns -1
    cr_assert_eq(len, -1, "Should return -1 for \\r\\0 as per current function logic");
}

Test(circular_buffer, find_command_end_no_newline) {
    circular_buffer_t cb;
    setup_buffer(&cb, "command without newline");
    int len = find_command_end(&cb);
    cr_assert_eq(len, -1, "Should return -1 if no newline is found");
}

Test(circular_buffer, find_command_end_empty_buffer) {
    circular_buffer_t cb;
    init_circular_buffer(&cb);
    int len = find_command_end(&cb);
    cr_assert_eq(len, -1, "Should return -1 for an empty buffer");
}

Test(circular_buffer, find_command_end_newline_at_start) {
    circular_buffer_t cb;
    setup_buffer(&cb, "\ncommand");
    int len = find_command_end(&cb);
    cr_assert_eq(len, 1, "Should find newline at the beginning");
}

Test(circular_buffer, find_command_end_wraps_around) {
    circular_buffer_t cb;
    if (BUFFER_SIZE < 10) {
        cr_skip_test("BUFFER_SIZE too small for this wrap-around test.");
        return;
    }
    // Fill buffer: "defg\nabc" where 'd' is at cb.start
    init_circular_buffer(&cb);
    cb.start = BUFFER_SIZE - 4;
    cb.buffer[cb.start] = 'd';
    cb.buffer[(cb.start + 1) % BUFFER_SIZE] = 'e';
    cb.buffer[(cb.start + 2) % BUFFER_SIZE] = 'f';
    cb.buffer[(cb.start + 3) % BUFFER_SIZE] = 'g';
    cb.buffer[(cb.start + 4) % BUFFER_SIZE] = '\n';
    cb.buffer[(cb.start + 5) % BUFFER_SIZE] = 'a';
    cb.buffer[(cb.start + 6) % BUFFER_SIZE] = 'b';
    cb.buffer[(cb.start + 7) % BUFFER_SIZE] = 'c';
    cb.end = (cb.start + 8) % BUFFER_SIZE;
    cb.count = 8;
    int len = find_command_end(&cb);
    cr_assert_eq(len, 5, "Should find newline after wrapping (d,e,f,g,\\n)");
}

// Tests for extract_command function (currently uncovered)

Test(circular_buffer, extract_command_simple) {
    circular_buffer_t cb;
    setup_buffer(&cb, "command1\nextra");
    int cmd_len = find_command_end(&cb);
    cr_assert_eq(cmd_len, 9);
    char *cmd = extract_command(&cb, cmd_len);
    cr_assert_not_null(cmd, "Extracted command should not be NULL");
    cr_assert_str_eq(cmd, "command1\n", "Extracted command is incorrect");
    cr_assert_eq(cb.count, 5, "Count should be reduced by cmd_len");
    cr_assert_eq(cb.start, (0 + cmd_len) % BUFFER_SIZE, "Start should advance by cmd_len");
    free(cmd);
}

Test(circular_buffer, extract_command_full_buffer_match) {
    circular_buffer_t cb;
    char data[BUFFER_SIZE];
    memset(data, 'a', BUFFER_SIZE - 2);
    data[BUFFER_SIZE - 2] = '\n';
    data[BUFFER_SIZE - 1] = '\0';
    setup_buffer(&cb, data);
    int cmd_len = find_command_end(&cb);
    cr_assert_eq(cmd_len, BUFFER_SIZE - 1, "Command length should be BUFFER_SIZE - 1");
    char *cmd = extract_command(&cb, cmd_len);
    cr_assert_not_null(cmd, "Extracted command should not be NULL");
    data[BUFFER_SIZE - 1] = '\0';
    cr_assert_str_eq(cmd, data, "Extracted command is incorrect");
    cr_assert_eq(cb.count, 0, "Count should be 0 after extracting full command");
    cr_assert_eq(cb.start, (0 + cmd_len) % BUFFER_SIZE, "Start should advance correctly");
    free(cmd);
}

Test(circular_buffer, extract_command_wraps_around) {
    circular_buffer_t cb;
    if (BUFFER_SIZE < 5) {
        cr_skip_test("BUFFER_SIZE too small for this wrap-around extract test.");
        return;
    }
    // Command is "Data\n" where buffer wraps around
    init_circular_buffer(&cb);
    cb.start = BUFFER_SIZE - 2;
    cb.buffer[cb.start] = 'D';
    cb.buffer[(cb.start + 1) % BUFFER_SIZE] = 'a';
    cb.buffer[(cb.start + 2) % BUFFER_SIZE] = 't';
    cb.buffer[(cb.start + 3) % BUFFER_SIZE] = 'a';
    cb.buffer[(cb.start + 4) % BUFFER_SIZE] = '\n';
    cb.end = (cb.start + 5) % BUFFER_SIZE;
    cb.count = 5;
    int cmd_len = find_command_end(&cb);
    cr_assert_eq(cmd_len, 5, "Command length for wrapped buffer incorrect");
    char *cmd = extract_command(&cb, cmd_len);
    cr_assert_not_null(cmd, "Extracted command should not be NULL");
    cr_assert_str_eq(cmd, "Data\n", "Extracted wrapped command is incorrect");
    cr_assert_eq(cb.count, 0, "Count should be 0 after extraction");
    cr_assert_eq(cb.start, cb.end, "Start should equal end after extracting whole buffer");
    free(cmd);
}

Test(circular_buffer, extract_command_zero_length) {
    circular_buffer_t cb;
    init_circular_buffer(&cb);
    char *cmd = extract_command(&cb, 0);
    cr_assert_not_null(cmd, "Command should not be null for 0 length");
    cr_assert_str_eq(cmd, "", "Command should be empty string for 0 length");
    free(cmd);
}

Test(circular_buffer, extract_command_partial_buffer) {
    circular_buffer_t cb;
    setup_buffer(&cb, "hello\nworld\n");
    // First extract "hello\n"
    int cmd_len = find_command_end(&cb);
    cr_assert_eq(cmd_len, 6);
    char *cmd = extract_command(&cb, cmd_len);
    cr_assert_not_null(cmd, "Extracted command should not be NULL");
    cr_assert_str_eq(cmd, "hello\n", "First extracted command is incorrect");
    cr_assert_eq(cb.count, 6, "Count should be reduced correctly");
    free(cmd);
    
    // Then extract "world\n"
    cmd_len = find_command_end(&cb);
    cr_assert_eq(cmd_len, 6);
    cmd = extract_command(&cb, cmd_len);
    cr_assert_not_null(cmd, "Second extracted command should not be NULL");
    cr_assert_str_eq(cmd, "world\n", "Second extracted command is incorrect");
    cr_assert_eq(cb.count, 0, "Count should be 0 after extracting all");
    free(cmd);
}

Test(circular_buffer, extract_command_single_char) {
    circular_buffer_t cb;
    setup_buffer(&cb, "\n");
    int cmd_len = find_command_end(&cb);
    cr_assert_eq(cmd_len, 1);
    char *cmd = extract_command(&cb, cmd_len);
    cr_assert_not_null(cmd, "Extracted command should not be NULL");
    cr_assert_str_eq(cmd, "\n", "Single char command is incorrect");
    cr_assert_eq(cb.count, 0, "Count should be 0 after extraction");
    free(cmd);
}

Test(circular_buffer, extract_command_malloc_failure) {
    // This test is hard to reliably trigger without deeper hooks into malloc
    // For now, we'll skip this test as it requires mocking malloc
    cr_skip_test("Skipping malloc failure test for extract_command: hard to mock reliably.");
}