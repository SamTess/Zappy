/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour les buffers réseau
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/logging.h>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>
#include "../../src/GUI/network/buffer/CircularBuffer.hpp"
#include "../../src/GUI/network/buffer/MessageQueue.hpp"
#include "../../src/GUI/shared/exception/AException.hpp"

// Configuration pour rediriger stdout/stderr pour les tests
static void redirect_buffer_std(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// === Tests pour CircularBuffer ===

Test(CircularBuffer, EmptyBufferOperations, .init = redirect_buffer_std) {
    CircularBuffer buffer(10);

    cr_assert(buffer.isEmpty(), "New buffer should be empty");
    cr_assert(!buffer.isFull(), "New buffer should not be full");
    cr_assert_eq(buffer.available(), 0U, "New buffer should have 0 available bytes");

    char readBuffer[5];
    size_t bytesRead = buffer.read(readBuffer, 5);
    cr_assert_eq(bytesRead, 0U, "Reading from empty buffer should return 0 bytes");
}

Test(CircularBuffer, BasicWriteReadOperations, .init = redirect_buffer_std) {
    CircularBuffer buffer(10);

    const char* testData = "12345";
    buffer.write(testData, 5);

    cr_assert(!buffer.isEmpty(), "Buffer should not be empty after write");
    cr_assert_eq(buffer.available(), 5U, "Buffer should have 5 available bytes");

    char readBuffer[10] = {0};
    size_t bytesRead = buffer.read(readBuffer, 10);

    cr_assert_eq(bytesRead, 5U, "Should read 5 bytes from buffer");
    cr_assert_str_eq(readBuffer, testData, "Read data should match written data");
    cr_assert(buffer.isEmpty(), "Buffer should be empty after reading all data");
}

Test(CircularBuffer, CircularWriteReadOperations, .init = redirect_buffer_std) {
    CircularBuffer buffer(10);

    // Écrire et lire partiellement pour décaler les indices head/tail
    const char* firstData = "12345";
    buffer.write(firstData, 5);
    
    char readBuffer[3] = {0};
    buffer.read(readBuffer, 3);
    cr_assert_eq(std::string(readBuffer, 3), "123", "First partial read should match");
    
    // Écrire plus de données
    const char* secondData = "67890ABCDE";
    buffer.write(secondData, 10);
    
    // Lire toutes les données restantes
    char fullReadBuffer[20] = {0};
    size_t bytesRead = buffer.read(fullReadBuffer, 20);
    
    // Devrait contenir le reste du premier write (45) et une partie du deuxième (67890)
    cr_assert_eq(bytesRead, 12U, "Should read the remaining 2 bytes from first write + 10 bytes from second write");
    cr_assert_eq(std::string(fullReadBuffer, bytesRead), "4567890ABCDE", "Data should wrap around correctly");
}

Test(CircularBuffer, ResizeOperations, .init = redirect_buffer_std) {
    CircularBuffer buffer(5);
    
    const char* testData = "12345";
    buffer.write(testData, 5);
    
    buffer.resize(10); // Agrandir le buffer
    
    cr_assert(!buffer.isFull(), "Buffer should not be full after resize");
    cr_assert_eq(buffer.available(), 5U, "Available data should remain the same after resize");
    
    const char* moreData = "67890";
    buffer.write(moreData, 5);
    
    cr_assert(buffer.isFull(), "Buffer should be full after writing to new capacity");
    
    char readBuffer[15] = {0};
    size_t bytesRead = buffer.read(readBuffer, 15);
    
    cr_assert_eq(bytesRead, 10U, "Should read all 10 bytes from buffer");
    cr_assert_str_eq(readBuffer, "1234567890", "Read data should match all written data");
}

Test(CircularBuffer, ClearOperation, .init = redirect_buffer_std) {
    CircularBuffer buffer(5);
    
    const char* testData = "12345";
    buffer.write(testData, 5);
    
    cr_assert(buffer.isFull(), "Buffer should be full");
    
    buffer.clear();
    
    cr_assert(buffer.isEmpty(), "Buffer should be empty after clear");
    cr_assert_eq(buffer.available(), 0U, "Available bytes should be 0 after clear");
}

// === Tests pour MessageQueue ===

Test(MessageQueue, EmptyQueueOperations, .init = redirect_buffer_std) {
    MessageQueue queue;
    
    cr_assert(queue.isEmpty(), "New queue should be empty");
    
    std::string message = queue.dequeue();
    cr_assert(message.empty(), "Dequeue from empty queue should return empty string");
}

Test(MessageQueue, EnqueueDequeueOperations, .init = redirect_buffer_std) {
    MessageQueue queue;
    
    queue.enqueue("Message 1");
    queue.enqueue("Message 2");
    
    cr_assert(!queue.isEmpty(), "Queue should not be empty after enqueue");
    
    std::string msg1 = queue.dequeue();
    cr_assert_str_eq(msg1.c_str(), "Message 1", "First dequeued message should match");
    
    std::string msg2 = queue.dequeue();
    cr_assert_str_eq(msg2.c_str(), "Message 2", "Second dequeued message should match");
    
    cr_assert(queue.isEmpty(), "Queue should be empty after dequeuing all messages");
}

Test(MessageQueue, MultithreadedOperations, .init = redirect_buffer_std) {
    MessageQueue queue;
    std::atomic<int> messagesProduced(0);
    std::atomic<int> messagesConsumed(0);
    bool producerDone = false;
    bool consumerDone = false;
    
    // Thread producteur
    std::thread producer([&]() {
        for (int i = 0; i < 100; i++) {
            queue.enqueue("Message " + std::to_string(i));
            messagesProduced++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        producerDone = true;
    });
    
    // Thread consommateur
    std::thread consumer([&]() {
        while (!producerDone || !queue.isEmpty()) {
            std::string msg = queue.dequeue();
            if (!msg.empty()) {
                messagesConsumed++;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        consumerDone = true;
    });
    
    producer.join();
    consumer.join();
    
    cr_assert_eq(messagesProduced.load(), messagesConsumed.load(), "All produced messages should be consumed");
    cr_assert(queue.isEmpty(), "Queue should be empty after all messages are processed");
}

Test(MessageQueue, ClearOperation, .init = redirect_buffer_std) {
    MessageQueue queue;
    
    queue.enqueue("Message 1");
    queue.enqueue("Message 2");
    queue.enqueue("Message 3");
    
    queue.clear();
    
    cr_assert(queue.isEmpty(), "Queue should be empty after clear");
    std::string msg = queue.dequeue();
    cr_assert(msg.empty(), "Dequeue after clear should return empty string");
}
