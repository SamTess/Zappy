/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ProtocolParser
*/

#ifndef PROTOCOLPARSER_HPP_
#define PROTOCOLPARSER_HPP_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <utility>
#include <functional>
#include "Message.hpp"
#include "HeaderMessage.hpp"
#include "messageData/MessageDataAll.hpp"
#include "../../shared/exception/AException.hpp"

class ProtocolParser {
    public:
        typedef std::function<Message(const std::string&)> ParseFunction;

        ProtocolParser();
        ~ProtocolParser() = default;

        Message parseMessage(const std::string &message);
        std::pair<std::string, std::string> parseMessage(const std::string &message) const;
        std::string getCommandName(const std::string &message);
        bool messageComplete(const std::string &buffer);
        std::vector<std::string> splitMessage(const std::string &message);
        std::string extractCommandParameter(const std::string &message);
        std::vector<std::string> extractMessageParameters(const std::string &message);
        int parseIntParameter(const std::string &param);

        void handleProtocol(const std::string &protocol);
        Message parseMapSize(const std::string &message);          // msz X Y\n
        Message parseTileContent(const std::string &message);      // bct X Y q0-q6
        Message parseAllTilesContent(const std::string &message);  // mct\n

        // Parsing des équipes
        Message parseTeamNames(const std::string &message);        // tna\n

        // Parsing des joueurs
        Message parsePlayerConnection(const std::string &message); // pnw
        Message parsePlayerPosition(const std::string &message);   // ppo
        Message parsePlayerLevel(const std::string &message);      // plv
        Message parsePlayerInventory(const std::string &message);  // pin
        Message parsePlayerExpulsion(const std::string &message);  // pex
        Message parsePlayerBroadcast(const std::string &message);  // pbc
        Message parsePlayerDeath(const std::string &message);      // pdi

        // Parsing des ressources
        Message parseRessourceDrop(const std::string &message);     // pdr
        Message parseRessourceCollect(const std::string &message);  // pgt

        // Parsing des incantations
        Message parseIncantationStart(const std::string &message); // pic
        Message parseIncantationEnd(const std::string &message);   // pie

        // Parsing des oeufs
        Message parseEggDrop(const std::string &message);         // enw
        Message parseEggConnection(const std::string &message);    // ebo
        Message parseEggDeath(const std::string &message);        // edi
        Message parseEggLaying(const std::string &message);        // pfk

        // Parsing du temps
        Message parseTimeUnit(const std::string &message);        // sgt/sst

        // Parsing de la fin de partie
        Message parseEndGame(const std::string &message);         // seg

        // Messages serveur
        Message parseServerMessage(const std::string &message);   // smg
        Message parseUnknownCommand(const std::string &message);  // suc/sbp

    private:
        std::map<std::string, ParseFunction> _headerHandlers;
        class ProtocolParserException : public AException {
            public:
                explicit ProtocolParserException(const std::string &message)
                    : AException("ProtocolParserException", message) {}
        };
};

#endif /* !PROTOCOLPARSER_HPP_ */
