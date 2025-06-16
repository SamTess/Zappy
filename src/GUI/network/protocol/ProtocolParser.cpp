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

ProtocolParser::ProtocolParser(std::shared_ptr<Logger> logger) : _logger(logger) {
    _logger->logInfo("ProtocolParser initialized with shared logger");
    _headerHandlers = std::map<std::string, ParseFunction>{
        {MSZ_HEADER, [this](const std::string& msg) { return parseMapSize(msg); }},
        {BCT_HEADER, [this](const std::string& msg) { return parseTileContent(msg); }},
        {MCT_HEADER, [this](const std::string& msg) { return parseAllTilesContent(msg); }},
        {TNA_HEADER, [this](const std::string& msg) { return parseTeamNames(msg); }},
        {PNW_HEADER, [this](const std::string& msg) { return parsePlayerConnection(msg); }},
        {PPO_HEADER, [this](const std::string& msg) { return parsePlayerPosition(msg); }},
        {PLV_HEADER, [this](const std::string& msg) { return parsePlayerLevel(msg); }},
        {PIN_HEADER, [this](const std::string& msg) { return parsePlayerInventory(msg); }},
        {PEX_HEADER, [this](const std::string& msg) { return parsePlayerExpulsion(msg); }},
        {PBC_HEADER, [this](const std::string& msg) { return parsePlayerBroadcast(msg); }},
        {PDR_HEADER, [this](const std::string& msg) { return parseRessourceDrop(msg); }},
        {PGT_HEADER, [this](const std::string& msg) { return parseRessourceCollect(msg); }},
        {PDI_HEADER, [this](const std::string& msg) { return parsePlayerDeath(msg); }},
        {PIC_HEADER, [this](const std::string& msg) { return parseIncantationStart(msg); }},
        {PIE_HEADER, [this](const std::string& msg) { return parseIncantationEnd(msg); }},
        {ENW_HEADER, [this](const std::string& msg) { return parseEggDrop(msg); }},
        {EBO_HEADER, [this](const std::string& msg) { return parseEggConnection(msg); }},
        {EDI_HEADER, [this](const std::string& msg) { return parseEggDeath(msg); }},
        {PFK_HEADER, [this](const std::string& msg) { return parseEggLaying(msg); }},
        {SGT_HEADER, [this](const std::string& msg) { return parseTimeUnit(msg); }},
        {SST_HEADER, [this](const std::string& msg) { return parseTimeUnit(msg); }},
        {SEG_HEADER, [this](const std::string& msg) { return parseEndGame(msg); }},
        {SMG_HEADER, [this](const std::string& msg) { return parseServerMessage(msg); }},
        {SUC_HEADER, [this](const std::string& msg) { return parseUnknownCommand(msg); }},
        {SBP_HEADER, [this](const std::string& msg) { return parseUnknownCommand(msg); }}
    };
}

Message ProtocolParser::parseMessage(const std::string &message) {
    _logger->logDebug("Parsing message: " + message.substr(0, message.length()-1));
    if (message.empty()) {
        _logger->logError("Empty message received");
        throw ProtocolParserException("Empty message received");
    }
    if (!messageComplete(message)) {
        _logger->logWarning("Incomplete message received: " + message);
        throw ProtocolParserException("Incomplete message received: " + message);
    }
    std::string header = getCommandName(message);
    _logger->logDebug("Extracted header: " + header);
    auto handlerIt = _headerHandlers.find(header);
    if (handlerIt != _headerHandlers.end()) {
        _logger->logDebug("Found handler for header: " + header);
        try {
            Message result = handlerIt->second(message);
            _logger->logDebug("Successfully parsed message with header: " + header);
            return result;
        } catch (const std::exception& e) {
            _logger->logError("Error parsing message with header " + header + ": " + e.what());
            throw;
        }
    }
    _logger->logWarning("No handler found for header: " + header + ", treating as unknown command");
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
        _logger->logError("Failed to parse integer parameter: " + param + " - " + e.what());
        throw ProtocolParserException("Invalid integer parameter: " + param);
    }
}

Message ProtocolParser::parseMapSize(const std::string &message) {
    _logger->logDebug("Parsing map size message: " + message.substr(0, message.length()-1));

    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() != 2) {
        _logger->logError("Invalid map size parameters count: " + std::to_string(params.size()) + " (expected 2)");
        throw ProtocolParserException("Invalid map size parameters: " + message);
    }

    int width = parseIntParameter(params[0]);
    int height = parseIntParameter(params[1]);

    _logger->logInfo("Parsed map size: " + std::to_string(width) + "x" + std::to_string(height));

    auto mapSizeData = std::make_shared<MapSizeData>(width, height);
    return Message(MSZ_HEADER, extractCommandParameter(message), mapSizeData);
}

Message ProtocolParser::parseTileContent(const std::string &message) {
    _logger->logDebug("Parsing tile content message: " + message.substr(0, message.length()-1));

    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 9) {
        _logger->logError("Invalid tile content parameters count: " + std::to_string(params.size()) + " (expected 9)");
        throw ProtocolParserException("Invalid tile content parameters: " + message);
    }

    int x = parseIntParameter(params[0]);
    int y = parseIntParameter(params[1]);
    int food = parseIntParameter(params[2]);
    int linemate = parseIntParameter(params[3]);
    int deraumere = parseIntParameter(params[4]);
    int sibur = parseIntParameter(params[5]);
    int mendiane = parseIntParameter(params[6]);
    int phiras = parseIntParameter(params[7]);
    int thystame = parseIntParameter(params[8]);

    _logger->logDebug("Parsed tile content at (" + std::to_string(x) + "," + std::to_string(y) +
        ") with " + std::to_string(food) + " food and " +
        std::to_string(linemate + deraumere + sibur + mendiane + phiras + thystame) + " stones");

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
    _logger->logDebug("Parsing team names message: " + message.substr(0, message.length()-1));
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1) {
        _logger->logError("Invalid team names parameters count: " + std::to_string(params.size()) + " (expected at least 1)");
        throw ProtocolParserException("Invalid team names parameters: " + message);
    }

    std::string teamList = "";
    for (size_t i = 0; i < params.size(); ++i) {
        teamList += params[i];
        if (i < params.size() - 1) teamList += ", ";
    }
    _logger->logInfo("Parsed team names: [" + teamList + "]");

    auto teamNamesData = std::make_shared<TeamNameData>(params);
    return Message(TNA_HEADER, extractCommandParameter(message), teamNamesData);
}

Message ProtocolParser::parsePlayerConnection(const std::string &message) {
    _logger->logDebug("Parsing player connection message: " + message.substr(0, message.length()-1));
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 6) {
        _logger->logError("Invalid player connection parameters count: " + std::to_string(params.size()) + " (expected 6)");
        throw ProtocolParserException("Invalid player connection parameters: " + message);
    }

    int id = parseIntParameter(params[0]);
    int x = parseIntParameter(params[1]);
    int y = parseIntParameter(params[2]);
    int orientation = parseIntParameter(params[3]);
    int level = parseIntParameter(params[4]);
    std::string teamName = params[5];
    _logger->logInfo("Parsed player connection: ID=" + std::to_string(id) +
        " pos=(" + std::to_string(x) + "," + std::to_string(y) +
        ") orientation=" + std::to_string(orientation) +
        " level=" + std::to_string(level) +
        " team=" + teamName);

    auto playerInfoData = std::make_shared<PlayerInfoData>(id, x, y, orientation, level, teamName);
    return Message(PNW_HEADER, extractCommandParameter(message), playerInfoData);
}

Message ProtocolParser::parsePlayerPosition(const std::string &message) {
    _logger->logDebug("Parsing player position message: " + message.substr(0, message.length()-1));
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 4) {
        _logger->logError("Invalid player position parameters count: " + std::to_string(params.size()) + " (expected 4)");
        throw ProtocolParserException("Invalid player position parameters: " + message);
    }

    int id = parseIntParameter(params[0]);
    int x = parseIntParameter(params[1]);
    int y = parseIntParameter(params[2]);
    int orientation = parseIntParameter(params[3]);
    _logger->logDebug("Parsed player position: ID=" + std::to_string(id) +
        " pos=(" + std::to_string(x) + "," + std::to_string(y) +
        ") orientation=" + std::to_string(orientation));

    auto playerInfoData = std::make_shared<PlayerInfoData>(id, x, y, orientation, 0);
    return Message(PPO_HEADER, extractCommandParameter(message), playerInfoData);
}

Message ProtocolParser::parsePlayerLevel(const std::string &message) {
    _logger->logDebug("Parsing player level message: " + message.substr(0, message.length()-1));
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 2) {
        _logger->logError("Invalid player level parameters count: " + std::to_string(params.size()) + " (expected 2)");
        throw ProtocolParserException("Invalid player level parameters: " + message);
    }

    int id = parseIntParameter(params[0]);
    int level = parseIntParameter(params[1]);
    _logger->logDebug("Parsed player level: ID=" + std::to_string(id) + " level=" + std::to_string(level));

    auto playerInfoData = std::make_shared<PlayerInfoData>(id, 0, 0, 0, level);
    return Message(PLV_HEADER, extractCommandParameter(message), playerInfoData);
}

Message ProtocolParser::parsePlayerInventory(const std::string &message) {
    _logger->logDebug("Parsing player inventory message: " + message.substr(0, message.length()-1));
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 10) {
        _logger->logError("Invalid player inventory parameters count: " + std::to_string(params.size()) + " (expected 10)");
        throw ProtocolParserException("Invalid player inventory parameters: " + message);
    }

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
    _logger->logDebug("Parsed player inventory: ID=" + std::to_string(id) +
        " pos=(" + std::to_string(x) + "," + std::to_string(y) +
        ") food=" + std::to_string(food) +
        " stones=" + std::to_string(linemate + deraumere + sibur + mendiane + phiras + thystame));

    auto playerInventoryData = std::make_shared<PlayerInventoryData>(id, x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame);
    return Message(PIN_HEADER, extractCommandParameter(message), playerInventoryData);
}

Message ProtocolParser::parsePlayerExpulsion(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid player expulsion parameters: " + message);
    int id = parseIntParameter(params[0]);

    auto playerExpulsionData = std::make_shared<PlayerExpulsionData>(id);
    return Message(PEX_HEADER, extractCommandParameter(message), playerExpulsionData);
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
    _logger->logWarning("Unknown command received: " + message.substr(0, message.length()-1));
    std::cout << "Unknown command received: " << message << std::endl;

    auto serverMessageData = std::make_shared<ServerMessageData>("Unknown command: " + message);
    return Message(SUC_HEADER, "", serverMessageData);
}
