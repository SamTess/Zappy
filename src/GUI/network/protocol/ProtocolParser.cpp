/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ProtocolParser
*/

#include "ProtocolParser.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

ProtocolParser::ProtocolParser() {
    _headerHandlers = {
        {MSZ_HEADER, &ProtocolParser::parseMapSize},
        {BCT_HEADER, &ProtocolParser::parseTileContent},
        {MCT_HEADER, &ProtocolParser::parseAllTilesContent},
        {TNA_HEADER, &ProtocolParser::parseTeamNames},
        {PNW_HEADER, &ProtocolParser::parsePlayerConnection},
        {PPO_HEADER, &ProtocolParser::parsePlayerPosition},
        {PLV_HEADER, &ProtocolParser::parsePlayerLevel},
        {PIN_HEADER, &ProtocolParser::parsePlayerInventory},
        {PEX_HEADER, &ProtocolParser::parsePlayerExpulsion},
        {PBC_HEADER, &ProtocolParser::parsePlayerBroadcast},
        {PDR_HEADER, &ProtocolParser::parseRessourceDrop},
        {PGT_HEADER, &ProtocolParser::parseRessourceCollect},
        {PDI_HEADER, &ProtocolParser::parsePlayerDeath},
        {PIC_HEADER, &ProtocolParser::parseIncantationStart},
        {PIE_HEADER, &ProtocolParser::parseIncantationEnd},
        {ENW_HEADER, &ProtocolParser::parseEggDrop},
        {EBO_HEADER, &ProtocolParser::parseEggConnection},
        {EDI_HEADER, &ProtocolParser::parseEggDeath},
        {PFK_HEADER, &ProtocolParser::parseEggLaying},
        {SGT_HEADER, &ProtocolParser::parseTimeUnit},
        {SST_HEADER, &ProtocolParser::parseTimeUnit},
        {SEG_HEADER, &ProtocolParser::parseEndGame},
        {SMG_HEADER, &ProtocolParser::parseServerMessage},
        {SUC_HEADER, &ProtocolParser::parseUnknownCommand},
        {SBP_HEADER, &ProtocolParser::parseUnknownCommand}
    };
}

Message ProtocolParser::parseMessage(const std::string &message) {
    if (message.empty())
        throw ProtocolParserException("Empty message received");
    if (!messageComplete(message))
        throw ProtocolParserException("Incomplete message received: " + message);
    std::string header = getCommandName(message);
    auto handlerIt = _headerHandlers.find(header);
    if (handlerIt != _headerHandlers.end()) {
        ParseFunction parseFunc = handlerIt->second;
        return (this->*parseFunc)(message);
    }
    return parseUnknownCommand(message);
}

std::string ProtocolParser::getCommandName(const std::string &message) {
    std::string header;
    size_t spacePos = message.find(' ');
    size_t newlinePos = message.find('\n');

    if (spacePos != std::string::npos && (newlinePos == std::string::npos || spacePos < newlinePos)) {
        header = message.substr(0, spacePos);
    } else if (newlinePos != std::string::npos) {
        header = message.substr(0, newlinePos);
    } else {
        header = message;
    }
    return header;
}

bool ProtocolParser::isValidHeader(const std::string &message) {
    std::string header;
    size_t spacePos = message.find(' ');
    size_t newlinePos = message.find('\n');

    if (spacePos != std::string::npos && (newlinePos == std::string::npos || spacePos < newlinePos)) {
        header = message.substr(0, spacePos);
    } else if (newlinePos != std::string::npos) {
        header = message.substr(0, newlinePos);
    } else {
        header = message;
    }
    return _headerHandlers.find(header) != _headerHandlers.end();
}

bool ProtocolParser::messageComplete(const std::string &buffer) {
    if (buffer.empty() || !isValidHeader(buffer))
        return false;
    return buffer.back() == '\n';
}

std::vector<std::string> ProtocolParser::splitMessage(const std::string &message) {
    std::vector<std::string> params;
    std::string::size_type start = 0;
    std::string::size_type end = message.find(' ');

    while (end != std::string::npos) {
        params.push_back(message.substr(start, end - start));
        start = end + 1;
        end = message.find(' ', start);
    }
    params.push_back(message.substr(start));
    return params;
}

std::vector<std::string> ProtocolParser::extractMessageParameters(const std::string &message) {
    std::vector<std::string> params = splitMessage(message);

    if (!params.empty())
        params.erase(params.begin());
    if (!params.empty()) {
        std::string& lastParam = params.back();
        if (!lastParam.empty() && lastParam.back() == '\n') {
            lastParam.pop_back();
        }
    }
    return params;
}

int ProtocolParser::parseIntParameter(const std::string &param) {
    try {
        return std::stoi(param);
    } catch (const std::invalid_argument &e) {
        throw ProtocolParserException("Invalid integer parameter: " + param);
    } catch (const std::out_of_range &e) {
        throw ProtocolParserException("Integer parameter out of range: " + param);
    }
}

void ProtocolParser::handleProtocol(const std::string &protocol) {
    (void)protocol;
    return;
}

// Parsing des informations de la map
Message ProtocolParser::parseMapSize(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(MSZ_HEADER, "");
}

Message ProtocolParser::parseTileContent(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(BCT_HEADER, "");
}

Message ProtocolParser::parseAllTilesContent(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(MCT_HEADER, "");
}

// Parsing des équipes
Message ProtocolParser::parseTeamNames(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(TNA_HEADER, "");
}

// Parsing des joueurs
Message ProtocolParser::parsePlayerConnection(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PNW_HEADER, "");
}

Message ProtocolParser::parsePlayerPosition(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PPO_HEADER, "");
}

Message ProtocolParser::parsePlayerLevel(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PLV_HEADER, "");
}

Message ProtocolParser::parsePlayerInventory(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PIN_HEADER, "");
}

Message ProtocolParser::parsePlayerExpulsion(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PEX_HEADER, "");
}

Message ProtocolParser::parsePlayerBroadcast(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PBC_HEADER, "");
}

Message ProtocolParser::parsePlayerDeath(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PDI_HEADER, "");
}

// Parsing des ressources
Message ProtocolParser::parseRessourceDrop(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PDR_HEADER, "");
}

Message ProtocolParser::parseRessourceCollect(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PGT_HEADER, "");
}

// Parsing des incantations
Message ProtocolParser::parseIncantationStart(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PIC_HEADER, "");
}

Message ProtocolParser::parseIncantationEnd(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PIE_HEADER, "");
}

// Parsing des oeufs
Message ProtocolParser::parseEggLaying(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(PFK_HEADER, "");
}

Message ProtocolParser::parseEggDrop(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(ENW_HEADER, "");
}

Message ProtocolParser::parseEggConnection(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(EBO_HEADER, "");
}

Message ProtocolParser::parseEggDeath(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(EDI_HEADER, "");
}

// Autres messages
Message ProtocolParser::parseTimeUnit(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(SGT_HEADER, "");
}

Message ProtocolParser::parseEndGame(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(SEG_HEADER, "");
}

Message ProtocolParser::parseServerMessage(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    (void)params;
    //TODO faire la commande
    return Message(SMG_HEADER, "");
}

Message ProtocolParser::parseUnknownCommand(const std::string &message) {
    (void)message;
    //TODO faire la commande
    std::cout << "Unknown command received: " << message << std::endl;
    return Message(SUC_HEADER, "");
}
