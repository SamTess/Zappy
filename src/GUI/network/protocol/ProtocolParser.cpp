/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ProtocolParser
*/

#include "ProtocolParser.hpp"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <sstream>

ProtocolParser::ProtocolParser() {
    _headerHandlers = std::map<std::string, ParseFunction>{
        {MSZ_HEADER, [this](const std::string& msg) { return this->parseMapSize(msg); }},
        {BCT_HEADER, [this](const std::string& msg) { return this->parseTileContent(msg); }},
        {MCT_HEADER, [this](const std::string& msg) { return this->parseAllTilesContent(msg); }},
        {TNA_HEADER, [this](const std::string& msg) { return this->parseTeamNames(msg); }},
        {PNW_HEADER, [this](const std::string& msg) { return this->parsePlayerConnection(msg); }},
        {PPO_HEADER, [this](const std::string& msg) { return this->parsePlayerPosition(msg); }},
        {PLV_HEADER, [this](const std::string& msg) { return this->parsePlayerLevel(msg); }},
        {PIN_HEADER, [this](const std::string& msg) { return this->parsePlayerInventory(msg); }},
        {PEX_HEADER, [this](const std::string& msg) { return this->parsePlayerExpulsion(msg); }},
        {PBC_HEADER, [this](const std::string& msg) { return this->parsePlayerBroadcast(msg); }},
        {PDR_HEADER, [this](const std::string& msg) { return this->parseRessourceDrop(msg); }},
        {PGT_HEADER, [this](const std::string& msg) { return this->parseRessourceCollect(msg); }},
        {PDI_HEADER, [this](const std::string& msg) { return this->parsePlayerDeath(msg); }},
        {PIC_HEADER, [this](const std::string& msg) { return this->parseIncantationStart(msg); }},
        {PIE_HEADER, [this](const std::string& msg) { return this->parseIncantationEnd(msg); }},
        {ENW_HEADER, [this](const std::string& msg) { return this->parseEggDrop(msg); }},
        {EBO_HEADER, [this](const std::string& msg) { return this->parseEggConnection(msg); }},
        {EDI_HEADER, [this](const std::string& msg) { return this->parseEggDeath(msg); }},
        {PFK_HEADER, [this](const std::string& msg) { return this->parseEggLaying(msg); }},
        {SGT_HEADER, [this](const std::string& msg) { return this->parseTimeUnit(msg); }},
        {SST_HEADER, [this](const std::string& msg) { return this->parseTimeUnit(msg); }},
        {SEG_HEADER, [this](const std::string& msg) { return this->parseEndGame(msg); }},
        {SMG_HEADER, [this](const std::string& msg) { return this->parseServerMessage(msg); }},
        {SUC_HEADER, [this](const std::string& msg) { return this->parseUnknownCommand(msg); }},
        {SBP_HEADER, [this](const std::string& msg) { return this->parseUnknownCommand(msg); }}
    };
}

Message ProtocolParser::parseMessage(const std::string &message) {
    if (message.empty())
        throw ProtocolParserException("Empty message received");
    if (!messageComplete(message))
        throw ProtocolParserException("Incomplete message received: " + message);
    std::string header = getCommandName(message);
    auto handlerIt = _headerHandlers.find(header);
    if (handlerIt != _headerHandlers.end())
        return handlerIt->second(message);
    return parseUnknownCommand(message);
}

std::pair<std::string, std::string> ProtocolParser::parseMessage(const std::string &message) const {
    std::string header;
    std::string data;
    size_t spacePos = message.find(' ');
    size_t newlinePos = message.find('\n');

    if (spacePos != std::string::npos && (newlinePos == std::string::npos || spacePos < newlinePos)) {
        header = message.substr(0, spacePos);
        if (newlinePos != std::string::npos)
            data = message.substr(spacePos + 1, newlinePos - spacePos - 1);
        else
            data = message.substr(spacePos + 1);
    } else if (newlinePos != std::string::npos) {
        header = message.substr(0, newlinePos);
        data = "";
    } else {
        header = message;
        data = "";
    }
    return std::make_pair(header, data);
}

bool ProtocolParser::messageComplete(const std::string &buffer) {
    return buffer.find('\n') != std::string::npos;
}

std::string ProtocolParser::getCommandName(const std::string &message) {
    size_t spacePos = message.find(' ');
    size_t newlinePos = message.find('\n');
    size_t endPos = std::min(spacePos, newlinePos);
    if (endPos == std::string::npos)
        endPos = message.length();
    return message.substr(0, endPos);
}

std::vector<std::string> ProtocolParser::splitMessage(const std::string &message) {
    std::vector<std::string> parts;
    std::string cleanMessage = message;
    if (!cleanMessage.empty() && cleanMessage.back() == '\n')
        cleanMessage.pop_back();
    std::istringstream ss(cleanMessage);
    std::string token;
    while (ss >> token)
        parts.push_back(token);
    return parts;
}

std::vector<std::string> ProtocolParser::extractMessageParameters(const std::string &message) {
    auto parts = splitMessage(message);
    if (!parts.empty())
        parts.erase(parts.begin());
    return parts;
}

std::string ProtocolParser::extractCommandParameter(const std::string &message) {
    size_t spacePos = message.find(' ');
    if (spacePos == std::string::npos)
        return "";
    size_t newlinePos = message.find('\n', spacePos);
    if (newlinePos == std::string::npos)
        newlinePos = message.length();
    return message.substr(spacePos + 1, newlinePos - spacePos - 1);
}

int ProtocolParser::parseIntParameter(const std::string &param) {
    try {
        std::string cleanParam = param;
        if (!cleanParam.empty() && cleanParam[0] == '#') {
            cleanParam = cleanParam.substr(1);
        }
        return std::stoi(cleanParam);
    } catch (const std::exception &e) {
        throw ProtocolParserException("Failed to parse integer parameter: " + param);
    }
}

Message ProtocolParser::parseMapSize(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() != 2)
        throw ProtocolParserException("Invalid map size parameters: " + message);
    auto mapSizeData = std::make_shared<MapSizeData>(parseIntParameter(params[0]), parseIntParameter(params[1]));
    return Message(MSZ_HEADER, extractCommandParameter(message), mapSizeData);
}

Message ProtocolParser::parseTileContent(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 9)
        throw ProtocolParserException("Invalid tile content parameters: " + message);
    int x = parseIntParameter(params[0]);
    int y = parseIntParameter(params[1]);
    int food = parseIntParameter(params[2]);
    int linemate = parseIntParameter(params[3]);
    int deraumere = parseIntParameter(params[4]);
    int sibur = parseIntParameter(params[5]);
    int mendiane = parseIntParameter(params[6]);
    int phiras = parseIntParameter(params[7]);
    int thystame = parseIntParameter(params[8]);

    auto tileContentData = std::make_shared<TileContentData>(x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame);
    return Message(BCT_HEADER, extractCommandParameter(message), tileContentData);
}

Message ProtocolParser::parseAllTilesContent(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (!params.empty())
        throw ProtocolParserException("Invalid parameters for all tiles content: " + message);
    return Message(MCT_HEADER, extractCommandParameter(message), nullptr);
}

Message ProtocolParser::parseTeamNames(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid parameters for team names: " + message);
    auto teamNamesData = std::make_shared<TeamNameData>(params);
    return Message(TNA_HEADER, extractCommandParameter(message), teamNamesData);
}

Message ProtocolParser::parsePlayerConnection(const std::string &message) {
    printf("Parsing player connection message: %s\n", message.c_str());
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 6)
        throw ProtocolParserException("Invalid player connection parameters: " + message);
    int id = parseIntParameter(params[0]);
    int x = parseIntParameter(params[1]);
    int y = parseIntParameter(params[2]);
    int orientation = parseIntParameter(params[3]);
    int level = parseIntParameter(params[4]);
    std::string teamName = params[5];

    auto playerInfoData = std::make_shared<PlayerInfoData>(id, x, y, orientation, level, teamName);
    return Message(PNW_HEADER, extractCommandParameter(message), playerInfoData);
}

Message ProtocolParser::parsePlayerPosition(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 4)
        throw ProtocolParserException("Invalid player position parameters: " + message);
    int id = parseIntParameter(params[0]);
    int x = parseIntParameter(params[1]);
    int y = parseIntParameter(params[2]);
    int orientation = parseIntParameter(params[3]);

    auto playerInfoData = std::make_shared<PlayerInfoData>(id, x, y, orientation, 0);
    return Message(PPO_HEADER, extractCommandParameter(message), playerInfoData);
}

Message ProtocolParser::parsePlayerLevel(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 2)
        throw ProtocolParserException("Invalid player level parameters: " + message);
    int id = parseIntParameter(params[0]);
    int level = parseIntParameter(params[1]);

    auto playerInfoData = std::make_shared<PlayerInfoData>(id, 0, 0, 0, level);
    return Message(PLV_HEADER, extractCommandParameter(message), playerInfoData);
}

Message ProtocolParser::parsePlayerInventory(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 10)
        throw ProtocolParserException("Invalid player inventory parameters: " + message);
    int id = parseIntParameter(params[0]);
    int x = parseIntParameter(params[1]);
    int y = parseIntParameter(params[2]);
    int food = parseIntParameter(params[3]);
    int linemate = parseIntParameter(params[4]);
    int deraumere = parseIntParameter(params[5]);
    int sibur = parseIntParameter(params[6]);
    int mendiane = parseIntParameter(params[7]);
    int phiras = parseIntParameter(params[8]);
    int thystame = parseIntParameter(params[9]);

    auto inventoryData = std::make_shared<PlayerInventoryData>(id, x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame);
    return Message(PIN_HEADER, extractCommandParameter(message), inventoryData);
}

Message ProtocolParser::parsePlayerExpulsion(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid player expulsion parameters: " + message);
    int id = parseIntParameter(params[0]);

    auto playerInfoData = std::make_shared<PlayerInfoData>(id, 0, 0, 0, 0);
    return Message(PEX_HEADER, extractCommandParameter(message), playerInfoData);
}

Message ProtocolParser::parsePlayerBroadcast(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 2)
        throw ProtocolParserException("Invalid player broadcast parameters: " + message);
    int id = parseIntParameter(params[0]);
    std::string broadcastMessage = params[1];

    auto broadcastData = std::make_shared<BroadcastData>(id, broadcastMessage);
    return Message(PBC_HEADER, extractCommandParameter(message), broadcastData);
}

Message ProtocolParser::parsePlayerDeath(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid player death parameters: " + message);
    int id = parseIntParameter(params[0]);

    auto playerInfoData = std::make_shared<PlayerInfoData>(id, 0, 0, 0, 0);
    return Message(PDI_HEADER, extractCommandParameter(message), playerInfoData);
}

Message ProtocolParser::parseRessourceDrop(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 2)
        throw ProtocolParserException("Invalid resource drop parameters: " + message);
    int playerId = parseIntParameter(params[0]);
    int resourceType = parseIntParameter(params[1]);

    auto resourceData = std::make_shared<ResourceData>(playerId, resourceType);
    return Message(PDR_HEADER, extractCommandParameter(message), resourceData);
}

Message ProtocolParser::parseRessourceCollect(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 2)
        throw ProtocolParserException("Invalid resource collection parameters: " + message);
    int playerId = parseIntParameter(params[0]);
    int resourceType = parseIntParameter(params[1]);

    auto resourceData = std::make_shared<ResourceData>(playerId, resourceType);
    return Message(PGT_HEADER, extractCommandParameter(message), resourceData);
}

Message ProtocolParser::parseIncantationStart(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 4)
        throw ProtocolParserException("Invalid incantation start parameters: " + message);

    int x = parseIntParameter(params[0]);
    int y = parseIntParameter(params[1]);
    int level = parseIntParameter(params[2]);
    std::vector<int> playerIds;

    for (size_t i = 3; i < params.size(); i++)
        playerIds.push_back(parseIntParameter(params[i]));
    auto incantationData = std::make_shared<IncantationData>(x, y, level, playerIds);
    return Message(PIC_HEADER, extractCommandParameter(message), incantationData);
}

Message ProtocolParser::parseIncantationEnd(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 3)
        throw ProtocolParserException("Invalid incantation end parameters: " + message);

    int x = parseIntParameter(params[0]);
    int y = parseIntParameter(params[1]);
    bool result = parseIntParameter(params[2]);

    auto incantationEndData = std::make_shared<IncantationEndData>(x, y, result);
    return Message(PIE_HEADER, extractCommandParameter(message), incantationEndData);
}

// Parsing des oeufs
Message ProtocolParser::parseEggLaying(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid egg laying parameters: " + message);
    int playerId = parseIntParameter(params[0]);

    auto eggData = std::make_shared<EggData>(0, playerId, 0, 0, EggData::EggAction::Laying);
    return Message(PFK_HEADER, extractCommandParameter(message), eggData);
}

Message ProtocolParser::parseEggDrop(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 4)
        throw ProtocolParserException("Invalid egg drop parameters: " + message);
    int eggId = parseIntParameter(params[0]);
    int playerId = parseIntParameter(params[1]);
    int x = parseIntParameter(params[2]);
    int y = parseIntParameter(params[3]);

    auto eggData = std::make_shared<EggData>(eggId, playerId, x, y, EggData::EggAction::Drop);
    return Message(ENW_HEADER, extractCommandParameter(message), eggData);
}

Message ProtocolParser::parseEggConnection(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid egg connection parameters: " + message);
    int eggId = parseIntParameter(params[0]);

    auto eggData = std::make_shared<EggData>(eggId, 0, 0, 0, EggData::EggAction::Connection);
    return Message(EBO_HEADER, extractCommandParameter(message), eggData);
}

Message ProtocolParser::parseEggDeath(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid egg death parameters: " + message);
    int eggId = parseIntParameter(params[0]);

    auto eggData = std::make_shared<EggData>(eggId, 0, 0, 0, EggData::EggAction::Death);
    return Message(EDI_HEADER, extractCommandParameter(message), eggData);
}

Message ProtocolParser::parseTimeUnit(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid time unit parameters: " + message);
    int timeUnit = parseIntParameter(params[0]);
    std::string header = getCommandName(message);

    auto timeUnitData = std::make_shared<TimeUnitData>(timeUnit);
    return Message(header, extractCommandParameter(message), timeUnitData);
}

Message ProtocolParser::parseEndGame(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid end game parameters: " + message);
    std::string teamName = params[0];

    auto endGameData = std::make_shared<EndGameData>(teamName);
    return Message(SEG_HEADER, extractCommandParameter(message), endGameData);
}

Message ProtocolParser::parseServerMessage(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.empty())
        throw ProtocolParserException("Invalid server message parameters: " + message);
    std::string serverMessage = params[0];

    auto serverMessageData = std::make_shared<ServerMessageData>(serverMessage);
    return Message(SMG_HEADER, extractCommandParameter(message), serverMessageData);
}

Message ProtocolParser::parseUnknownCommand(const std::string &message) {
    std::cout << "Unknown command received: " << message << std::endl;

    auto serverMessageData = std::make_shared<ServerMessageData>("Unknown command: " + message);
    return Message(SUC_HEADER, "", serverMessageData);
}
