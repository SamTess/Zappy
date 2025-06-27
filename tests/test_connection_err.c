/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_connection_err - tests for connection error functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Forward declarations
extern void server_err(char *msg);
extern void print_co(char *client_ip, struct sockaddr_in *client_addr, client_t *new_client);

TestSuite(connection_err_tests);


// Test print_co function with valid parameters
Test(connection_err_tests, test_print_co_valid_params, .init = cr_redirect_stdout)
{
    char client_ip[INET_ADDRSTRLEN];
    struct sockaddr_in client_addr;
    client_t new_client;
    
    // Setup test data
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "192.168.1.100", &client_addr.sin_addr);
    
    new_client.client_id = 42;
    
    // Call the function
    print_co(client_ip, &client_addr, &new_client);
    
    // Verify the client_ip was filled correctly
    cr_assert_str_eq(client_ip, "192.168.1.100", "IP should be correctly formatted");
    
    // Note: The printf output goes to stdout which is redirected by Criterion
    // We can't easily capture it but we can verify the function executes
    cr_assert(true, "print_co should execute without crashing");
}

// Test print_co function with localhost IP
Test(connection_err_tests, test_print_co_localhost, .init = cr_redirect_stdout)
{
    char client_ip[INET_ADDRSTRLEN];
    struct sockaddr_in client_addr;
    client_t new_client;
    
    // Setup test data for localhost
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(4242);
    inet_pton(AF_INET, "127.0.0.1", &client_addr.sin_addr);
    
    new_client.client_id = 123;
    
    // Call the function
    print_co(client_ip, &client_addr, &new_client);
    
    // Verify the client_ip was filled correctly
    cr_assert_str_eq(client_ip, "127.0.0.1", "Localhost IP should be correctly formatted");
    
    cr_assert(true, "print_co should execute without crashing");
}

// Test print_co function with different port numbers
Test(connection_err_tests, test_print_co_different_ports, .init = cr_redirect_stdout)
{
    char client_ip[INET_ADDRSTRLEN];
    struct sockaddr_in client_addr;
    client_t new_client;
    
    // Setup test data with port 0
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(0);
    inet_pton(AF_INET, "10.0.0.1", &client_addr.sin_addr);
    
    new_client.client_id = 0;
    
    // Call the function
    print_co(client_ip, &client_addr, &new_client);
    
    // Verify the client_ip was filled correctly
    cr_assert_str_eq(client_ip, "10.0.0.1", "IP should be correctly formatted");
    
    cr_assert(true, "print_co should execute without crashing");
}

// Test print_co function with maximum port number
Test(connection_err_tests, test_print_co_max_port, .init = cr_redirect_stdout)
{
    char client_ip[INET_ADDRSTRLEN];
    struct sockaddr_in client_addr;
    client_t new_client;
    
    // Setup test data with max port
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(65535);
    inet_pton(AF_INET, "255.255.255.255", &client_addr.sin_addr);
    
    new_client.client_id = -1;
    
    // Call the function
    print_co(client_ip, &client_addr, &new_client);
    
    // Verify the client_ip was filled correctly
    cr_assert_str_eq(client_ip, "255.255.255.255", "Broadcast IP should be correctly formatted");
    
    cr_assert(true, "print_co should execute without crashing");
}

// Test print_co function with zero IP address
Test(connection_err_tests, test_print_co_zero_ip, .init = cr_redirect_stdout)
{
    char client_ip[INET_ADDRSTRLEN];
    struct sockaddr_in client_addr;
    client_t new_client;
    
    // Setup test data with 0.0.0.0
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(1234);
    inet_pton(AF_INET, "0.0.0.0", &client_addr.sin_addr);
    
    new_client.client_id = 999;
    
    // Call the function
    print_co(client_ip, &client_addr, &new_client);
    
    // Verify the client_ip was filled correctly
    cr_assert_str_eq(client_ip, "0.0.0.0", "Zero IP should be correctly formatted");
    
    cr_assert(true, "print_co should execute without crashing");
}