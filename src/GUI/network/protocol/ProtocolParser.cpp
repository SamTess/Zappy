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
    std::string processedMessage = message;

    if (!processedMessage.empty() && processedMessage.back() == '\n')
        processedMessage.pop_back();
    while (end != std::string::npos) {
        params.push_back(processedMessage.substr(start, end - start));
        start = end + 1;
        end = processedMessage.find(' ', start);
    }
    params.push_back(processedMessage.substr(start));
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
        return std::stoi(param);
    } catch (const std::invalid_argument &e) {
        throw ProtocolParserException("Invalid integer parameter: " + param);
    } catch (const std::out_of_range &e) {
        throw ProtocolParserException("Integer parameter out of range: " + param);
    }
}

// Parsing des informations de la map
Message ProtocolParser::parseMapSize(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() != 2)
        throw ProtocolParserException("Invalid map size parameters: " + message);
    auto mapSizeData = std::make_shared<MapSizeData>(parseIntParameter(params[0]), parseIntParameter(params[1]));
    return Message(MSZ_HEADER, extractCommandParameter(message), mapSizeData);
}

Message ProtocolParser::parseTileContent(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 8)
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

// Parsing des équipes
Message ProtocolParser::parseTeamNames(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid parameters for team names: " + message);
    auto teamNamesData = std::make_shared<TeamNameData>(params);
    return Message(TNA_HEADER, extractCommandParameter(message), teamNamesData);
}

// Parsing des joueurs
Message ProtocolParser::parsePlayerConnection(const std::string &message) {
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

// Parsing des ressources
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

// Parsing des incantations
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
    auto playerInfoData = std::make_shared<PlayerInfoData>(playerId, 0, 0, 0, 0);
    return Message(PFK_HEADER, extractCommandParameter(message), playerInfoData);
}

Message ProtocolParser::parseEggDrop(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 4)
        throw ProtocolParserException("Invalid egg drop parameters: " + message);
    int eggId = parseIntParameter(params[0]);
    int playerId = parseIntParameter(params[1]);
    int x = parseIntParameter(params[2]);
    int y = parseIntParameter(params[3]);
    auto eggData = std::make_shared<EggData>(eggId, playerId, x, y);
    return Message(ENW_HEADER, extractCommandParameter(message), eggData);
}

Message ProtocolParser::parseEggConnection(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid egg connection parameters: " + message);
    int eggId = parseIntParameter(params[0]);
    auto eggData = std::make_shared<EggData>(eggId, 0, 0, 0);
    return Message(EBO_HEADER, extractCommandParameter(message), eggData);
}

Message ProtocolParser::parseEggDeath(const std::string &message) {
    std::vector<std::string> params = extractMessageParameters(message);
    if (params.size() < 1)
        throw ProtocolParserException("Invalid egg death parameters: " + message);
    int eggId = parseIntParameter(params[0]);
    auto eggData = std::make_shared<EggData>(eggId, 0, 0, 0);
    return Message(EDI_HEADER, extractCommandParameter(message), eggData);
}

// Autres messages
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
