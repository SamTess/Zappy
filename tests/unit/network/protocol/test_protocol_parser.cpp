/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour ProtocolParser
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../../../src/GUI/network/protocol/ProtocolParser.hpp"
#include "../../../../src/GUI/network/buffer/CircularBuffer.hpp"
#include "../../../../src/GUI/network/protocol/messageData/MapSizeData.hpp"
#include "../../../../src/GUI/network/protocol/messageData/TileContentData.hpp"
#include "../../../../src/GUI/network/protocol/messageData/PlayerInfoData.hpp"
#include <memory>
#include <string>

Test(ProtocolParser, creation_destruction) {
    ProtocolParser parser;
    // Vérifier simplement que le constructeur et le destructeur s'exécutent correctement
}

Test(ProtocolParser, message_complete) {
    ProtocolParser parser;
    
    // Tester avec des messages complets (terminés par \n)
    cr_assert(parser.messageComplete("WELCOME\n"), "Un message se terminant par \\n doit être considéré comme complet");
    cr_assert(parser.messageComplete("msz 10 20\n"), "Un message se terminant par \\n doit être considéré comme complet");
    
    // Tester avec des messages incomplets (sans \n)
    cr_assert(!parser.messageComplete("WELCOME"), "Un message sans \\n doit être considéré comme incomplet");
    cr_assert(!parser.messageComplete("msz 10 20"), "Un message sans \\n doit être considéré comme incomplet");
    
    // Tester avec des cas particuliers
    cr_assert(parser.messageComplete("\n"), "Un message contenant uniquement \\n doit être considéré comme complet");
    cr_assert(!parser.messageComplete(""), "Un message vide doit être considéré comme incomplet");
}

Test(ProtocolParser, extract_message) {
    ProtocolParser parser;
    auto buffer = std::make_shared<CircularBuffer>(100);
    
    // Ajouter un message au buffer
    buffer->write(std::string("WELCOME\n"));
    
    // Extraire le message
    std::string message = parser.extractMessage(buffer);
    cr_assert_eq(message, "WELCOME\n", "Le message extrait doit correspondre à celui qui a été écrit");
    
    // Vérifier que le buffer est maintenant vide
    cr_assert(buffer->isEmpty(), "Le buffer doit être vide après extraction du message");
    
    // Tester avec un message incomplet
    buffer->write(std::string("msz 10 20"));
    message = parser.extractMessage(buffer);
    cr_assert_eq(message, "", "Aucun message ne doit être extrait si le message est incomplet");
    cr_assert(!buffer->isEmpty(), "Le buffer doit contenir toujours le message incomplet");
    
    // Compléter le message et extraire
    buffer->write(std::string("\n"));
    message = parser.extractMessage(buffer);
    cr_assert_eq(message, "msz 10 20\n", "Le message extrait doit correspondre à celui qui a été écrit");
}

Test(ProtocolParser, split_message) {
    ProtocolParser parser;
    
    // Tester le découpage d'un message simple
    std::vector<std::string> parts = parser.splitMessage("msz 10 20\n");
    cr_assert_eq(parts.size(), 3, "Le message doit être découpé en 3 parties");
    cr_assert_eq(parts[0], "msz", "La première partie doit être 'msz'");
    cr_assert_eq(parts[1], "10", "La deuxième partie doit être '10'");
    cr_assert_eq(parts[2], "20", "La troisième partie doit être '20'");
    
    // Tester avec un message sans espace
    parts = parser.splitMessage("WELCOME\n");
    cr_assert_eq(parts.size(), 1, "Le message doit être découpé en 1 partie");
    cr_assert_eq(parts[0], "WELCOME", "La partie doit être 'WELCOME'");
    
    // Définir manuellement les parties attendues
    std::vector<std::string> expected_parts = {"pnw", "1", "5", "5", "1", "0", "player1"};
    
    // Récupérer les parties réelles
    parts = parser.splitMessage("pnw  1  5  5  1  0 player1\n");
    
    // Afficher les deux ensembles de parties pour le débogage
    printf("Parties attendues (%zu):\n", expected_parts.size());
    for (size_t i = 0; i < expected_parts.size(); ++i) {
        printf("  %zu: '%s'\n", i, expected_parts[i].c_str());
    }
    
    printf("Parties réelles (%zu):\n", parts.size());
    for (size_t i = 0; i < parts.size(); ++i) {
        printf("  %zu: '%s'\n", i, parts[i].c_str());
    }
    
    cr_assert_eq(parts.size(), expected_parts.size(), 
                 "Le nombre de parties doit correspondre à ce qui est attendu (%zu vs %zu)",
                 expected_parts.size(), parts.size());
    
    for (size_t i = 0; i < parts.size() && i < expected_parts.size(); ++i) {
        cr_assert_eq(parts[i], expected_parts[i], 
                     "La partie %zu doit être '%s'", i, expected_parts[i].c_str());
    }
    cr_assert_eq(parts[0], "pnw", "La première partie doit être 'pnw'");
    cr_assert_eq(parts[1], "1", "La deuxième partie doit être '1'");
    cr_assert_eq(parts[2], "5", "La troisième partie doit être '5'");
    cr_assert_eq(parts[3], "5", "La quatrième partie doit être '5'");
    cr_assert_eq(parts[4], "1", "La cinquième partie doit être '1'");
    cr_assert_eq(parts[5], "0", "La sixième partie doit être '0'");
    cr_assert_eq(parts[6], "player1", "La septième partie doit être 'player1'");
}

Test(ProtocolParser, parse_int_parameter) {
    ProtocolParser parser;
    
    // Tester la conversion d'une chaîne en entier
    cr_assert_eq(parser.parseIntParameter("10"), 10, "La chaîne '10' doit être convertie en entier 10");
    cr_assert_eq(parser.parseIntParameter("0"), 0, "La chaîne '0' doit être convertie en entier 0");
    cr_assert_eq(parser.parseIntParameter("-5"), -5, "La chaîne '-5' doit être convertie en entier -5");
    
    // Tester avec une valeur invalide
    bool exception_thrown = false;
    try {
        parser.parseIntParameter("invalid");
    } catch (const std::exception &) {
        exception_thrown = true;
    }
    cr_assert(exception_thrown, "Une exception doit être levée pour une chaîne non numérique");
}

Test(ProtocolParser, extract_command_parameter) {
    ProtocolParser parser;
    
    // Tester l'extraction des paramètres d'une commande
    std::string params = parser.extractCommandParameter("msz 10 20\n");
    cr_assert_eq(params, "10 20", "Les paramètres extraits doivent être '10 20'");
    
    params = parser.extractCommandParameter("WELCOME\n");
    cr_assert_eq(params, "", "Une commande sans paramètre doit retourner une chaîne vide");
    
    params = parser.extractCommandParameter("pnw 1 5 5 1 0 player1\n");
    cr_assert_eq(params, "1 5 5 1 0 player1", "Les paramètres extraits doivent être '1 5 5 1 0 player1'");
}

Test(ProtocolParser, parse_map_size) {
    ProtocolParser parser;
    
    // Tester le parsing d'un message de taille de carte
    Message msg = parser.parseMapSize("msz 10 20\n");
    
    cr_assert_eq(msg.getHeader(), "msz", "L'en-tête doit être 'msz'");
    
    // Vérifier que le message a des données structurées
    cr_assert(msg.hasStructuredData(), "Le message doit avoir des données structurées");
    
    // Pour tester le contenu, nous devons accéder aux données structurées
    auto mapSizeData = std::dynamic_pointer_cast<MapSizeData>(msg.getStructuredData());
    cr_assert(mapSizeData != nullptr, "Les données structurées doivent être de type MapSizeData");
    cr_assert_eq(mapSizeData->getWidth(), 10, "La largeur doit être 10");
    cr_assert_eq(mapSizeData->getHeight(), 20, "La hauteur doit être 20");
    
    // Tester avec un format invalide
    bool exception_thrown = false;
    try {
        parser.parseMapSize("msz 10\n");
    } catch (const std::exception &) {
        exception_thrown = true;
    }
    cr_assert(exception_thrown, "Une exception doit être levée pour un format invalide");
}

Test(ProtocolParser, parse_tile_content) {
    ProtocolParser parser;
    
    // Tester le parsing d'un message de contenu de tuile
    Message msg = parser.parseTileContent("bct 3 4 0 1 2 3 4 5 6\n");
    
    cr_assert_eq(msg.getHeader(), "bct", "L'en-tête doit être 'bct'");
    
    auto tileData = std::dynamic_pointer_cast<TileContentData>(msg.getStructuredData());
    cr_assert(tileData != nullptr, "Les données structurées doivent être de type TileContentData");
    
    cr_assert_eq(tileData->getX(), 3, "La coordonnée X doit être 3");
    cr_assert_eq(tileData->getY(), 4, "La coordonnée Y doit être 4");
    
    // Vérifier les ressources q0-q6
    int resources[] = {0, 1, 2, 3, 4, 5, 6};
    cr_assert_eq(tileData->getFood(), resources[0], "La quantité de nourriture doit être 0");
    cr_assert_eq(tileData->getLinemate(), resources[1], "La quantité de linemate doit être 1");
    cr_assert_eq(tileData->getDeraumere(), resources[2], "La quantité de deraumere doit être 2");
    cr_assert_eq(tileData->getSibur(), resources[3], "La quantité de sibur doit être 3");
    cr_assert_eq(tileData->getMendiane(), resources[4], "La quantité de mendiane doit être 4");
    cr_assert_eq(tileData->getPhiras(), resources[5], "La quantité de phiras doit être 5");
    cr_assert_eq(tileData->getThystame(), resources[6], "La quantité de thystame doit être 6");
}

Test(ProtocolParser, parse_player_connection) {
    ProtocolParser parser;
    
    // Tester le parsing d'un message de connexion de joueur
    Message msg = parser.parsePlayerConnection("pnw 1 5 5 1 0 player1\n");
    
    cr_assert_eq(msg.getHeader(), "pnw", "L'en-tête doit être 'pnw'");
    
    auto playerData = std::dynamic_pointer_cast<PlayerInfoData>(msg.getStructuredData());
    cr_assert(playerData != nullptr, "Les données structurées doivent être de type PlayerInfoData");
    
    cr_assert_eq(playerData->getId(), 1, "L'ID du joueur doit être 1");
    cr_assert_eq(playerData->getX(), 5, "La coordonnée X doit être 5");
    cr_assert_eq(playerData->getY(), 5, "La coordonnée Y doit être 5");
    cr_assert_eq(playerData->getOrientation(), 1, "L'orientation doit être 1");
    cr_assert_eq(playerData->getLevel(), 0, "Le niveau doit être 0");
    cr_assert_eq(playerData->getTeamName(), "player1", "Le nom de l'équipe doit être 'player1'");
}
