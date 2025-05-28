/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ProtocolParser
*/

#include "ProtocolParser.hpp"
#include <algorithm>
#include <stdexcept>

ProtocolParser::ProtocolParser() {
    _validHeaders = {
        MSZ_HEADER,
        BCT_HEADER,
        MCT_HEADER,
        TNA_HEADER,
        PNW_HEADER,
        PPO_HEADER,
        PLV_HEADER,
        PIN_HEADER,
        PEX_HEADER,
        PBC_HEADER,
        PDR_HEADER,
        PGT_HEADER,
        PDI_HEADER,
        PIC_HEADER,
        PIE_HEADER,
        ENW_HEADER,
        EBO_HEADER,
        EDI_HEADER,
        PFK_HEADER,
        SGT_HEADER,
        SST_HEADER,
        SEG_HEADER,
        SMG_HEADER,
        SUC_HEADER,
        SBP_HEADER
    };
}

Message ProtocolParser::parseMessage(const std::string &message) {
    // a faire mieux
    std::string header = getCommandName(message);
    Message msg(header, message.substr(header.length()));
    return msg;
}

std::string ProtocolParser::getCommandName(const std::string &message) {
    (void)message;
    return message;
}

bool ProtocolParser::isValidMessage(const std::string &message) {
    // la ca check juste le header si il existe ceppendant il faut aussi check sur le contenue du message est ok avec le header
    std::string header = getCommandName(message);
    return std::find(_validHeaders.begin(), _validHeaders.end(), header) != _validHeaders.end();
}

bool ProtocolParser::messageComplete(const std::string &buffer) {
    if (buffer.empty() || !isValidMessage(buffer))
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
    // return un truc mieux pour les paramètres
    return splitMessage(message);
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
    (void)message;
    return Message(MSZ_HEADER, "");
}

Message ProtocolParser::parseTileContent(const std::string &message) {
    (void)message;
    return Message(BCT_HEADER, "");
}

Message ProtocolParser::parseAllTilesContent(const std::string &message) {
    (void)message;
    return Message(MCT_HEADER, "");
}

// Parsing des équipes
Message ProtocolParser::parseTeamNames(const std::string &message) {
    (void)message;
    return Message(TNA_HEADER, "");
}

// Parsing des joueurs
Message ProtocolParser::parsePlayerConnection(const std::string &message) {
    (void)message;
    return Message(PNW_HEADER, "");
}

Message ProtocolParser::parsePlayerPosition(const std::string &message) {
    (void)message;
    return Message(PPO_HEADER, "");
}

Message ProtocolParser::parsePlayerLevel(const std::string &message) {
    (void)message;
    return Message(PLV_HEADER, "");
}

Message ProtocolParser::parsePlayerInventory(const std::string &message) {
    (void)message;
    return Message(PIN_HEADER, "");
}

Message ProtocolParser::parsePlayerExpulsion(const std::string &message) {
    (void)message;
    return Message(PEX_HEADER, "");
}

Message ProtocolParser::parsePlayerBroadcast(const std::string &message) {
    (void)message;
    return Message(PBC_HEADER, "");
}

Message ProtocolParser::parsePlayerDeath(const std::string &message) {
    (void)message;
    return Message(PDI_HEADER, "");
}

// Parsing des incantations
Message ProtocolParser::parseIncantationStart(const std::string &message) {
    (void)message;
    return Message(PIC_HEADER, "");
}

Message ProtocolParser::parseIncantationEnd(const std::string &message) {
    (void)message;
    return Message(PIE_HEADER, "");
}

// Parsing des oeufs
Message ProtocolParser::parseEggLaying(const std::string &message) {
    (void)message;
    return Message(PFK_HEADER, "");
}

Message ProtocolParser::parseEggDrop(const std::string &message) {
    (void)message;
    return Message(ENW_HEADER, "");
}

Message ProtocolParser::parseEggConnection(const std::string &message) {
    (void)message;
    return Message(EBO_HEADER, "");
}

Message ProtocolParser::parseEggDeath(const std::string &message) {
    (void)message;
    return Message(EDI_HEADER, "");
}

// Autres messages
Message ProtocolParser::parseTimeUnit(const std::string &message) {
    (void)message;
    return Message(SGT_HEADER, "");
}

Message ProtocolParser::parseEndGame(const std::string &message) {
    (void)message;
    return Message(SEG_HEADER, "");
}

Message ProtocolParser::parseServerMessage(const std::string &message) {
    (void)message;
    return Message(SMG_HEADER, "");
}

Message ProtocolParser::parseUnknownCommand(const std::string &message) {
    (void)message;
    return Message(SUC_HEADER, "");
}
