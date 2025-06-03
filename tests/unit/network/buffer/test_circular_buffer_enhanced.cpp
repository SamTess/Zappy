/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires complémentaires pour CircularBuffer
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../../../src/GUI/network/buffer/CircularBuffer.hpp"
#include <string>
#include <memory>
#include <cstring>

// Tests directs sur la classe C++ CircularBuffer

Test(CircularBufferCPP, create_destroy) {
    // Test de création et destruction simple
    CircularBuffer buffer(100);
    // Vérifier que le buffer est initialement vide
    cr_assert(buffer.isEmpty(), "Le buffer doit être vide à sa création");
    cr_assert(!buffer.isFull(), "Le buffer ne doit pas être plein à sa création");
}

Test(CircularBufferCPP, write_read_string) {
    CircularBuffer buffer(100);
    std::string test_data = "Hello, world!";
    
    // Écrire des données
    buffer.write(test_data);
    cr_assert(!buffer.isEmpty(), "Le buffer ne doit pas être vide après écriture");
    cr_assert_eq(buffer.available(), test_data.size(), "La taille disponible doit correspondre aux données écrites");
    
    // Lire des données
    std::string read_data;
    size_t read_size = buffer.read(read_data, test_data.size());
    
    cr_assert_eq(read_size, test_data.size(), "La taille lue doit correspondre à la taille écrite");
    cr_assert_eq(read_data, test_data, "Les données lues doivent correspondre aux données écrites");
    cr_assert(buffer.isEmpty(), "Le buffer doit être vide après lecture");
}

Test(CircularBufferCPP, read_as_string) {
    CircularBuffer buffer(100);
    std::string test_data = "Hello, read as string!";
    
    // Écrire des données
    buffer.write(test_data);
    
    // Lire comme chaîne
    std::string read_data = buffer.readAsString(test_data.size());
    
    cr_assert_eq(read_data, test_data, "readAsString doit retourner les données écrites");
    cr_assert(buffer.isEmpty(), "Le buffer doit être vide après lecture");
}

Test(CircularBufferCPP, write_string_view) {
    CircularBuffer buffer(100);
    std::string_view test_data = "Hello, string_view!";
    
    // Écrire des données
    buffer.write(test_data);
    cr_assert_eq(buffer.available(), test_data.size(), "La taille disponible doit correspondre aux données écrites");
    
    // Lire comme chaîne
    std::string read_data = buffer.readAsString(test_data.size());
    
    cr_assert_eq(read_data, test_data, "Les données lues doivent correspondre aux données écrites");
}

Test(CircularBufferCPP, partial_read_write) {
    CircularBuffer buffer(100);
    
    // Écrire des données en plusieurs fois
    buffer.write(std::string("First"));
    buffer.write(std::string(" Second"));
    
    cr_assert_eq(buffer.available(), 12, "La taille disponible doit être la somme des écritures");
    
    // Lire une partie des données
    std::string read_data;
    size_t read_size = buffer.read(read_data, 6);
    
    cr_assert_eq(read_size, 6, "La taille lue doit correspondre à la demande");
    cr_assert_eq(read_data, "First ", "Les données lues doivent correspondre");
    cr_assert_eq(buffer.available(), 6, "Le buffer doit contenir le reste des données");
    
    // Lire le reste
    read_data.clear();
    read_size = buffer.read(read_data, 10);
    cr_assert_eq(read_size, 6, "La taille lue doit correspondre aux données restantes");
    cr_assert_eq(read_data, "Second", "Les données lues doivent correspondre");
}

Test(CircularBufferCPP, buffer_full_empty) {
    // Créer un petit buffer
    CircularBuffer buffer(10);
    
    // Vérifier initialement vide
    cr_assert(buffer.isEmpty(), "Le buffer doit être vide initialement");
    cr_assert(!buffer.isFull(), "Le buffer ne doit pas être plein initialement");
    
    // Remplir complètement le buffer
    buffer.write(std::string("1234567890"));
    
    // Vérifier qu'il est plein
    cr_assert(!buffer.isEmpty(), "Le buffer ne doit pas être vide après remplissage");
    cr_assert(buffer.isFull(), "Le buffer doit être plein");
    
    // Lire partiellement
    std::string read_data;
    buffer.read(read_data, 5);
    
    // Vérifier qu'il n'est ni vide ni plein
    cr_assert(!buffer.isEmpty(), "Le buffer ne doit pas être vide après lecture partielle");
    cr_assert(!buffer.isFull(), "Le buffer ne doit pas être plein après lecture partielle");
    
    // Vider le buffer
    read_data.clear();
    buffer.read(read_data, 5);
    
    // Vérifier qu'il est vide
    cr_assert(buffer.isEmpty(), "Le buffer doit être vide après lecture complète");
    cr_assert(!buffer.isFull(), "Le buffer ne doit pas être plein après lecture complète");
}

Test(CircularBufferCPP, clear_operation) {
    CircularBuffer buffer(100);
    buffer.write(std::string("Test data"));
    
    cr_assert(!buffer.isEmpty(), "Le buffer doit contenir des données");
    
    // Effacer le buffer
    buffer.clear();
    
    cr_assert(buffer.isEmpty(), "Le buffer doit être vide après clear()");
    cr_assert_eq(buffer.available(), 0, "Aucune donnée ne doit être disponible après clear()");
}

Test(CircularBufferCPP, resize_operation) {
    CircularBuffer buffer(10);
    
    // Écrire quelques données
    buffer.write(std::string("12345"));
    
    // Redimensionner à une taille plus grande
    buffer.resize(20);
    
    // Vérifier que les données sont toujours présentes
    std::string read_data = buffer.readAsString(5);
    cr_assert_eq(read_data, "12345", "Les données doivent être préservées après redimensionnement");
    
    // Écrire plus de données que la taille originale
    buffer.write(std::string("123456789012345"));
    
    // Vérifier que ça fonctionne
    cr_assert_eq(buffer.available(), 15, "Le buffer redimensionné doit accepter plus de données");
}

Test(CircularBufferCPP, read_safe_buffer) {
    CircularBuffer buffer(100);
    std::string test_data = "Safe Buffer Test";
    
    // Écrire des données
    buffer.write(test_data);
    
    // Lire comme SafeBuffer
    SystemWrapper::SafeBuffer safe_buffer = buffer.readSafeBuffer(test_data.size());
    
    // Vérifier le contenu
    cr_assert_eq(safe_buffer.toString(), test_data, "readSafeBuffer doit retourner les données dans un SafeBuffer");
    
    // Vérifier que le buffer est vide
    cr_assert(buffer.isEmpty(), "Le buffer doit être vide après lecture avec readSafeBuffer");
}
