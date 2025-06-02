/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour CircularBuffer
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../src/GUI/network/buffer/CircularBuffer.hpp"
#include <cstring>

Test(CircularBuffer, creation) {
    CircularBuffer buffer(10);
    cr_assert(buffer.isEmpty(), "Nouveau buffer doit être vide");
    cr_assert_eq(buffer.available(), 0, "Nouveau buffer doit avoir 0 données disponibles");
    cr_assert(!buffer.isFull(), "Nouveau buffer ne doit pas être plein");
}

Test(CircularBuffer, write_read_data) {
    CircularBuffer buffer(10);
    std::string writeData = "test";
    size_t writeSize = writeData.size();
    
    buffer.write(writeData);
    
    cr_assert_eq(buffer.available(), writeSize, "Buffer doit avoir les données écrites disponibles");
    cr_assert(!buffer.isEmpty(), "Buffer ne doit pas être vide après écriture");
    
    std::string readBuffer;
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, writeSize, "Nombre d'octets lus doit correspondre à ce qui a été écrit");
    cr_assert_str_eq(readBuffer.c_str(), writeData.c_str(), "Données lues doivent être identiques aux données écrites");
    cr_assert(buffer.isEmpty(), "Buffer doit être vide après lecture complète");
}

Test(CircularBuffer, write_past_capacity) {
    CircularBuffer buffer(5);
    std::string writeData = "123456789";
    size_t writeSize = writeData.size();
    
    buffer.write(writeData);
    
    cr_assert_eq(buffer.available(), writeSize, "Buffer doit avoir les données écrites disponibles");
    cr_assert(!buffer.isEmpty(), "Buffer ne doit pas être vide après écriture");
    
    std::string readBuffer;
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, writeSize, "Nombre d'octets lus doit correspondre à ce qui a été écrit");
    cr_assert_str_eq(readBuffer.c_str(), writeData.c_str(), "Données lues doivent être identiques aux données écrites");
}

Test(CircularBuffer, partial_read) {
    CircularBuffer buffer(10);
    std::string writeData = "0123456789";
    size_t writeSize = writeData.size();
    
    buffer.write(writeData);
    
    std::string readBuffer;
    size_t bytesRead = buffer.read(readBuffer, 4);
    
    cr_assert_eq(bytesRead, 4, "Nombre d'octets lus doit correspondre à la taille demandée");
    cr_assert_str_eq(readBuffer.c_str(), "0123", "Les 4 premiers caractères doivent être lus correctement");
    cr_assert_eq(buffer.available(), writeSize - 4, "Buffer doit avoir moins de données disponibles après lecture partielle");
    
    bytesRead = buffer.read(readBuffer, 4);
    cr_assert_eq(bytesRead, 4, "Nombre d'octets lus doit correspondre à la taille demandée");
    cr_assert_str_eq(readBuffer.c_str(), "4567", "Les 4 caractères suivants doivent être lus correctement");
}

Test(CircularBuffer, clear) {
    CircularBuffer buffer(10);
    std::string writeData = "test";
    
    buffer.write(writeData);
    cr_assert(!buffer.isEmpty(), "Buffer ne doit pas être vide après écriture");
    
    buffer.clear();
    cr_assert(buffer.isEmpty(), "Buffer doit être vide après clear");
    cr_assert_eq(buffer.available(), 0, "Buffer doit avoir 0 données disponibles après clear");
}

Test(CircularBuffer, resize) {
    CircularBuffer buffer(5);
    std::string writeData = "12345";
    
    buffer.write(writeData);
    cr_assert(buffer.isFull(), "Buffer doit être plein");
    
    buffer.resize(10);
    cr_assert(!buffer.isFull(), "Buffer ne doit pas être plein après redimensionnement");
    
    std::string moreData = "67890";
    buffer.write(moreData);
    
    std::string readBuffer;
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, 10, "Nombre d'octets lus doit être 10");
    cr_assert_str_eq(readBuffer, "1234567890", "Toutes les données doivent être lues dans l'ordre correct");
}

Test(CircularBuffer, wrap_around) {
    CircularBuffer buffer(5);
    
    // Écrire et lire partiellement pour décaler l'index de tête
    buffer.write(std::string_view("123"));
    std::string temp;
    buffer.read(temp, 2);
    
    // Maintenant écrire pour forcer le buffer à se chevaucher
    buffer.write(std::string_view("4567"));
    
    std::string readBuffer;
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, 5, "Doit lire 5 octets (1 restant + 4 nouveaux)");
    cr_assert_str_eq(readBuffer.c_str(), "34567", "Données doivent être lues correctement après chevauchement");
}
