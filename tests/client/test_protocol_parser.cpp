/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests pour le parser de protocole
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/logging.h>
#include <string>
#include <thread>
#include <chrono>
#include "../../src/GUI/network/protocol/ProtocolParser.hpp"
#include "../../src/GUI/network/protocol/HeaderMessage.hpp"
#include "../../src/GUI/network/protocol/Message.hpp"

// Tests pour le ProtocolParser
Test(protocol_parser, initialization) {
    ProtocolParser parser;
    
    // Vérifie que le parser est correctement initialisé avec les headers valides
    bool hasMsz = false;
    
    // Utilisation d'une commande de test pour voir si elle est valide
    std::string validMessage = MSZ_HEADER;
    std::string invalidMessage = "INVALID";
    
    hasMsz = parser.isValidMessage(validMessage);
    
    cr_assert(hasMsz, "Parser should recognize MSZ as valid header");
    cr_assert(parser.isValidMessage(PNW_HEADER), "Parser should recognize PNW as valid header");
    cr_assert_not(parser.isValidMessage(invalidMessage), "Parser should reject invalid header");
}

Test(protocol_parser, parse_message) {
    ProtocolParser parser;
    
    // Test avec un message MSZ
    std::string mszMessage = MSZ_HEADER;
    Message msg = parser.parseMessage(mszMessage);
    
    cr_assert_str_eq(msg.getHeader().c_str(), MSZ_HEADER, "Parsed message should have MSZ header");
    
    // Test avec un message PNW
    std::string pnwMessage = PNW_HEADER;
    Message pnwMsg = parser.parseMessage(pnwMessage);
    
    cr_assert_str_eq(pnwMsg.getHeader().c_str(), PNW_HEADER, "Parsed message should have PNW header");
}

Test(protocol_parser, split_message) {
    ProtocolParser parser;
    
    // Test de la fonction split pour un message MSZ
    std::string message = "msz 10 20";
    std::vector<std::string> parts = parser.splitMessage(message);
    
    cr_assert_eq(parts.size(), 3, "Split message should have 3 parts");
    cr_assert_str_eq(parts[0].c_str(), "msz", "First part should be msz");
    cr_assert_str_eq(parts[1].c_str(), "10", "Second part should be 10");
    cr_assert_str_eq(parts[2].c_str(), "20", "Third part should be 20");
}

Test(protocol_parser, parse_int_parameter) {
    ProtocolParser parser;
    
    // Test de conversion de string en int
    int value = parser.parseIntParameter("42");
    cr_assert_eq(value, 42, "Parse int should return 42 for string '42'");
    
    // Test avec valeur négative
    value = parser.parseIntParameter("-10");
    cr_assert_eq(value, -10, "Parse int should return -10 for string '-10'");
    
    // Test avec une valeur invalide
    bool exceptionThrown = false;
    try {
        parser.parseIntParameter("abc");
    } catch (...) {
        exceptionThrown = true;
    }
    cr_assert(exceptionThrown, "Exception should be thrown for non-numeric string");
}

// Tests pour Message
Test(message, initialization) {
    // Test constructeur par défaut
    Message emptyMessage;
    cr_assert_str_eq(emptyMessage.getMessage().c_str(), "", "Default message should be empty");
    cr_assert_str_eq(emptyMessage.getHeader().c_str(), "", "Default header should be empty");
    cr_assert_str_eq(emptyMessage.getData().c_str(), "", "Default data should be empty");
    
    // Test constructeur avec paramètres
    Message fullMessage("MSZ", "10 20");
    cr_assert_str_eq(fullMessage.getHeader().c_str(), "MSZ", "Header should be MSZ");
    cr_assert_str_eq(fullMessage.getData().c_str(), "10 20", "Data should be 10 20");
    cr_assert_str_eq(fullMessage.getMessage().c_str(), "MSZ 10 20", "Full message should be MSZ 10 20");
}

Test(message, setters) {
    Message msg;
    
    // Test des setters
    msg.setHeader("PIN");
    msg.setData("1 10 20 30");
    msg.setMessage("PIN 1 10 20 30");
    
    cr_assert_str_eq(msg.getHeader().c_str(), "PIN", "Header should be PIN after setter");
    cr_assert_str_eq(msg.getData().c_str(), "1 10 20 30", "Data should be 1 10 20 30 after setter");
    cr_assert_str_eq(msg.getMessage().c_str(), "PIN 1 10 20 30", "Message should be PIN 1 10 20 30 after setter");
}
