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
    const char* writeData = "test";
    size_t writeSize = strlen(writeData);
    
    buffer.write(writeData, writeSize);
    
    cr_assert_eq(buffer.available(), writeSize, "Buffer doit avoir les données écrites disponibles");
    cr_assert(!buffer.isEmpty(), "Buffer ne doit pas être vide après écriture");
    
    char readBuffer[10] = {0};
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, writeSize, "Nombre d'octets lus doit correspondre à ce qui a été écrit");
    cr_assert_str_eq(readBuffer, writeData, "Données lues doivent être identiques aux données écrites");
    cr_assert(buffer.isEmpty(), "Buffer doit être vide après lecture complète");
}

Test(CircularBuffer, write_past_capacity) {
    CircularBuffer buffer(5);
    const char* writeData = "123456789";
    size_t writeSize = strlen(writeData);
    
    buffer.write(writeData, writeSize);
    
    cr_assert_eq(buffer.available(), writeSize, "Buffer doit avoir les données écrites disponibles");
    cr_assert(!buffer.isEmpty(), "Buffer ne doit pas être vide après écriture");
    
    char readBuffer[10] = {0};
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, writeSize, "Nombre d'octets lus doit correspondre à ce qui a été écrit");
    cr_assert_str_eq(readBuffer, writeData, "Données lues doivent être identiques aux données écrites");
}

Test(CircularBuffer, partial_read) {
    CircularBuffer buffer(10);
    const char* writeData = "0123456789";
    size_t writeSize = strlen(writeData);
    
    buffer.write(writeData, writeSize);
    
    char readBuffer[5] = {0};
    size_t bytesRead = buffer.read(readBuffer, 4);
    
    cr_assert_eq(bytesRead, 4, "Nombre d'octets lus doit correspondre à la taille demandée");
    cr_assert_str_eq(readBuffer, "0123", "Les 4 premiers caractères doivent être lus correctement");
    cr_assert_eq(buffer.available(), writeSize - 4, "Buffer doit avoir moins de données disponibles après lecture partielle");
    
    bytesRead = buffer.read(readBuffer, 4);
    cr_assert_eq(bytesRead, 4, "Nombre d'octets lus doit correspondre à la taille demandée");
    cr_assert_str_eq(readBuffer, "4567", "Les 4 caractères suivants doivent être lus correctement");
}

Test(CircularBuffer, clear) {
    CircularBuffer buffer(10);
    const char* writeData = "test";
    
    buffer.write(writeData, strlen(writeData));
    cr_assert(!buffer.isEmpty(), "Buffer ne doit pas être vide après écriture");
    
    buffer.clear();
    cr_assert(buffer.isEmpty(), "Buffer doit être vide après clear");
    cr_assert_eq(buffer.available(), 0, "Buffer doit avoir 0 données disponibles après clear");
}

Test(CircularBuffer, resize) {
    CircularBuffer buffer(5);
    const char* writeData = "12345";
    
    buffer.write(writeData, strlen(writeData));
    cr_assert(buffer.isFull(), "Buffer doit être plein");
    
    buffer.resize(10);
    cr_assert(!buffer.isFull(), "Buffer ne doit pas être plein après redimensionnement");
    
    const char* moreData = "67890";
    buffer.write(moreData, strlen(moreData));
    
    char readBuffer[11] = {0};
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, 10, "Nombre d'octets lus doit être 10");
    cr_assert_str_eq(readBuffer, "1234567890", "Toutes les données doivent être lues dans l'ordre correct");
}

Test(CircularBuffer, wrap_around) {
    CircularBuffer buffer(5);
    
    // Écrire et lire partiellement pour décaler l'index de tête
    buffer.write("123", 3);
    char temp[2] = {0};
    buffer.read(temp, 2);
    
    // Maintenant écrire pour forcer le buffer à se chevaucher
    buffer.write("4567", 4);
    
    char readBuffer[10] = {0};
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, 5, "Doit lire 5 octets (1 restant + 4 nouveaux)");
    cr_assert_str_eq(readBuffer, "34567", "Données doivent être lues correctement après chevauchement");
}
