/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour ProtocolParser
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../src/GUI/network/protocol/ProtocolParser.hpp"
#include "../../src/GUI/network/protocol/Message.hpp"
#include <string>

Test(ProtocolParser, creation) {
    ProtocolParser parser;
    // Test simple de création
    cr_assert(true, "Parser créé avec succès");
}

Test(ProtocolParser, validate_header) {
    ProtocolParser parser;
    
    // Tests de validation des en-têtes
    cr_assert(parser.isValidHeader("msz"), "En-tête msz doit être valide");
    cr_assert(parser.isValidHeader("bct"), "En-tête bct doit être valide");
    cr_assert(parser.isValidHeader("tna"), "En-tête tna doit être valide");
    cr_assert(parser.isValidHeader("pnw"), "En-tête pnw doit être valide");
    
    cr_assert(!parser.isValidHeader("xyz"), "En-tête xyz ne doit pas être valide");
    cr_assert(!parser.isValidHeader(""), "En-tête vide ne doit pas être valide");
}

Test(ProtocolParser, get_command_name) {
    ProtocolParser parser;
    
    cr_assert_str_eq(parser.getCommandName("msz 10 20\n").c_str(), "msz", "Doit extraire correctement l'en-tête msz");
    cr_assert_str_eq(parser.getCommandName("bct 1 2 0 1 2 3 4 5 6\n").c_str(), "bct", "Doit extraire correctement l'en-tête bct");
    cr_assert_str_eq(parser.getCommandName("tna Team1\n").c_str(), "tna", "Doit extraire correctement l'en-tête tna");
    
    // Test avec une chaîne vide
    cr_assert_str_eq(parser.getCommandName("").c_str(), "", "Doit gérer correctement une chaîne vide");
}

Test(ProtocolParser, split_message) {
    ProtocolParser parser;
    
    std::vector<std::string> parts = parser.splitMessage("msz 10 20\n");
    cr_assert_eq(parts.size(), 3, "Message msz doit être divisé en 3 parties");
    cr_assert_str_eq(parts[0].c_str(), "msz", "Première partie doit être msz");
    cr_assert_str_eq(parts[1].c_str(), "10", "Deuxième partie doit être 10");
    cr_assert_str_eq(parts[2].c_str(), "20", "Troisième partie doit être 20");
    
    parts = parser.splitMessage("pnw #1 1 1 1 1 Team1\n");
    cr_assert_eq(parts.size(), 7, "Message pnw doit être divisé en 7 parties");
    cr_assert_str_eq(parts[0].c_str(), "pnw", "Première partie doit être pnw");
    cr_assert_str_eq(parts[1].c_str(), "#1", "Deuxième partie doit être #1");
    cr_assert_str_eq(parts[6].c_str(), "Team1", "Dernière partie doit être Team1");
}

Test(ProtocolParser, extract_message_parameters) {
    ProtocolParser parser;
    
    std::vector<std::string> params = parser.extractMessageParameters("msz 10 20\n");
    cr_assert_eq(params.size(), 2, "Message msz doit avoir 2 paramètres");
    cr_assert_str_eq(params[0].c_str(), "10", "Premier paramètre doit être 10");
    cr_assert_str_eq(params[1].c_str(), "20", "Deuxième paramètre doit être 20");
    
    params = parser.extractMessageParameters("pnw #1 1 1 1 1 Team1\n");
    cr_assert_eq(params.size(), 6, "Message pnw doit avoir 6 paramètres");
    cr_assert_str_eq(params[0].c_str(), "#1", "Premier paramètre doit être #1");
    cr_assert_str_eq(params[5].c_str(), "Team1", "Dernier paramètre doit être Team1");
}

Test(ProtocolParser, parse_int_parameter) {
    ProtocolParser parser;
    
    cr_assert_eq(parser.parseIntParameter("10"), 10, "Doit parser 10 correctement");
    cr_assert_eq(parser.parseIntParameter("0"), 0, "Doit parser 0 correctement");
    cr_assert_eq(parser.parseIntParameter("-5"), -5, "Doit parser -5 correctement");
    
    // Test de valeur non numérique
    try {
        parser.parseIntParameter("abc");
        cr_assert_fail("Parser doit générer une exception pour les valeurs non numériques");
    } catch (const std::exception& e) {
        cr_assert(true, "Exception correctement levée pour valeur non numérique");
    }
}

Test(ProtocolParser, message_complete) {
    ProtocolParser parser;
    
    cr_assert(parser.messageComplete("msz 10 20\n"), "Message avec retour à la ligne doit être complet");
    cr_assert(!parser.messageComplete("msz 10 20"), "Message sans retour à la ligne ne doit pas être complet");
    cr_assert(parser.messageComplete("tna Team1\n"), "Message avec retour à la ligne doit être complet");
}

Test(ProtocolParser, parse_map_size) {
    ProtocolParser parser;
    
    Message message = parser.parseMapSize("msz 10 20\n");
    cr_assert_eq(message.getType(), HeaderMessage::MAP_SIZE, "Type de message doit être MAP_SIZE");
    cr_assert_eq(message.getParameters().size(), 2, "Message doit avoir 2 paramètres");
    cr_assert_eq(message.getIntParam(0), 10, "Premier paramètre doit être 10");
    cr_assert_eq(message.getIntParam(1), 20, "Deuxième paramètre doit être 20");
}
