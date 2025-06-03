/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test unitaires pour le CircularBuffer
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../../../src/GUI/network/buffer/CircularBuffer.hpp"

// Mock de la classe CircularBuffer pour les tests
extern "C" {
    // Interface C pour tester la classe C++
    void *create_circular_buffer(size_t capacity);
    void destroy_circular_buffer(void *buffer);
    void circular_buffer_write(void *buffer, const void *data, size_t size);
    size_t circular_buffer_read(void *buffer, void *dst, size_t size);
    size_t circular_buffer_available(void *buffer);
    bool circular_buffer_is_empty(void *buffer);
    bool circular_buffer_is_full(void *buffer);
    void circular_buffer_clear(void *buffer);
}

// Implémentation de l'interface C pour la classe CircularBuffer
void *create_circular_buffer(size_t capacity)
{
    return new CircularBuffer(capacity);
}

void destroy_circular_buffer(void *buffer)
{
    delete static_cast<CircularBuffer *>(buffer);
}

void circular_buffer_write(void *buffer, const void *data, size_t size)
{
    std::string str_data(static_cast<const char*>(data), size);
    static_cast<CircularBuffer *>(buffer)->write(str_data);
}

size_t circular_buffer_read(void *buffer, void *dst, size_t size)
{
    std::string str_data;
    size_t bytesRead = static_cast<CircularBuffer *>(buffer)->read(str_data, size);
    if (bytesRead > 0 && dst) {
        std::memcpy(dst, str_data.data(), bytesRead);
    }
    return bytesRead;
}

size_t circular_buffer_available(void *buffer)
{
    return static_cast<CircularBuffer *>(buffer)->available();
}

bool circular_buffer_is_empty(void *buffer)
{
    return static_cast<CircularBuffer *>(buffer)->isEmpty();
}

bool circular_buffer_is_full(void *buffer)
{
    return static_cast<CircularBuffer *>(buffer)->isFull();
}

void circular_buffer_clear(void *buffer)
{
    static_cast<CircularBuffer *>(buffer)->clear();
}

Test(circular_buffer, creation_destruction)
{
    void *buffer = create_circular_buffer(1024);
    cr_assert_not_null(buffer, "Le buffer circulaire n'a pas été créé correctement");
    destroy_circular_buffer(buffer);
}

Test(circular_buffer, write_read_basic)
{
    void *buffer = create_circular_buffer(1024);
    const char *test_data = "Hello World";
    size_t data_size = strlen(test_data) + 1;
    char read_buffer[1024] = {0};
    
    circular_buffer_write(buffer, test_data, data_size);
    cr_assert_eq(circular_buffer_available(buffer), data_size, "La taille des données disponibles est incorrecte");
    
    size_t read_size = circular_buffer_read(buffer, read_buffer, data_size);
    cr_assert_eq(read_size, data_size, "La taille des données lues est incorrecte");
    cr_assert_str_eq(read_buffer, test_data, "Les données lues ne correspondent pas aux données écrites");
    
    destroy_circular_buffer(buffer);
}

Test(circular_buffer, write_overflow)
{
    void *buffer = create_circular_buffer(10);
    const char *test_data = "This is a test string that is longer than the buffer capacity";
    size_t data_size = strlen(test_data) + 1;
    
    circular_buffer_write(buffer, test_data, data_size);
    // La nouvelle implémentation redimensionne automatiquement le buffer pour accueillir les données
    cr_assert(!circular_buffer_is_empty(buffer), 
             "Le buffer doit contenir des données après l'écriture des données plus grandes que la capacité initiale");
    cr_assert_eq(circular_buffer_available(buffer), data_size, 
                "La taille des données disponibles doit correspondre à la taille des données écrites");
    
    destroy_circular_buffer(buffer);
}

Test(circular_buffer, multiple_write_read)
{
    void *buffer = create_circular_buffer(1024);
    const char *test_data1 = "First message";
    const char *test_data2 = "Second message";
    size_t data_size1 = strlen(test_data1) + 1;
    size_t data_size2 = strlen(test_data2) + 1;
    char read_buffer[1024] = {0};
    
    circular_buffer_write(buffer, test_data1, data_size1);
    circular_buffer_write(buffer, test_data2, data_size2);
    
    cr_assert_eq(circular_buffer_available(buffer), data_size1 + data_size2, 
                "La taille des données disponibles est incorrecte");
    
    size_t read_size1 = circular_buffer_read(buffer, read_buffer, data_size1);
    cr_assert_eq(read_size1, data_size1, "La taille des données lues est incorrecte");
    cr_assert_str_eq(read_buffer, test_data1, "Les premières données lues ne correspondent pas");
    
    memset(read_buffer, 0, sizeof(read_buffer));
    size_t read_size2 = circular_buffer_read(buffer, read_buffer, data_size2);
    cr_assert_eq(read_size2, data_size2, "La taille des secondes données lues est incorrecte");
    cr_assert_str_eq(read_buffer, test_data2, "Les secondes données lues ne correspondent pas");
    
    destroy_circular_buffer(buffer);
}

Test(circular_buffer, clear_test)
{
    void *buffer = create_circular_buffer(1024);
    const char *test_data = "Test data";
    size_t data_size = strlen(test_data) + 1;
    
    circular_buffer_write(buffer, test_data, data_size);
    cr_assert_eq(circular_buffer_available(buffer), data_size, "Données incorrectement écrites");
    
    circular_buffer_clear(buffer);
    cr_assert_eq(circular_buffer_available(buffer), 0, "Le buffer n'a pas été vidé");
    cr_assert(circular_buffer_is_empty(buffer), "Le buffer doit être vide après clear");
    
    destroy_circular_buffer(buffer);
}

Test(circular_buffer, circular_wraparound)
{
    void *buffer = create_circular_buffer(20);
    const char *data1 = "ABCDEFGHIJ"; // 10 chars + null = 11
    const char *data2 = "1234567890"; // 10 chars + null = 11
    char read_buffer[21] = {0};
    
    // Écrire et lire partiellement pour déplacer les pointeurs
    circular_buffer_write(buffer, data1, strlen(data1) + 1);
    circular_buffer_read(buffer, read_buffer, 5); // Lire "ABCDE"
    
    // Maintenant le buffer a 6 octets utilisés (FGHIJ + null) et devrait avoir le début libre
    circular_buffer_write(buffer, data2, strlen(data2) + 1);
    
    // Lire les deux blocs de données
    memset(read_buffer, 0, sizeof(read_buffer));
    circular_buffer_read(buffer, read_buffer, 6); // Lire le reste de data1
    cr_assert_str_eq(read_buffer, "FGHIJ", "Première partie des données incorrecte après wraparound");
    
    memset(read_buffer, 0, sizeof(read_buffer));
    circular_buffer_read(buffer, read_buffer, strlen(data2) + 1); // Lire data2
    cr_assert_str_eq(read_buffer, data2, "Seconde partie des données incorrecte après wraparound");
    
    destroy_circular_buffer(buffer);
}
