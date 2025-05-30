/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ProtocolParser
*/

#include "ProtocolParser.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <string>
#include <utility>

ProtocolParser::ProtocolParser() {
    using std::placeholders::_1;

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

std::string ProtocolParser::extractMessage(std::shared_ptr<CircularBuffer> buffer) {
    if (!buffer || buffer->isEmpty())
        return "";
    std::string tempBuffer = buffer->readAsString(buffer->available());
    size_t newlinePos = tempBuffer.find('\n');
    if (newlinePos == std::string::npos) {
        buffer->write(tempBuffer);
        return "";
    }
    std::string message = tempBuffer.substr(0, newlinePos + 1);
    if (newlinePos + 1 < tempBuffer.size()) {
        buffer->write(tempBuffer.substr(newlinePos + 1));
    }
    return message;
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
    size_t start = 0;
    size_t end = message.find(' ');
    while (end != std::string::npos) {
        parts.push_back(message.substr(start, end - start));
        start = end + 1;
        end = message.find(' ', start);
    }
    std::string lastPart = message.substr(start);
    if (!lastPart.empty() && lastPart.back() == '\n')
        lastPart.pop_back();
    if (!lastPart.empty())
        parts.push_back(lastPart);
    return parts;
}

std::vector<std::string> ProtocolParser::extractMessageParameters(const std::string &message) {
    auto parts = splitMessage(message);
    if (!parts.empty())
        parts.erase(parts.begin());
    return parts;
}

int ProtocolParser::parseIntParameter(const std::string &param) {
    try {
        return std::stoi(param);
    } catch (const std::exception &e) {
        throw ProtocolParserException("Failed to parse integer parameter: " + param);
    }
}

Message ProtocolParser::parseMapSize(const std::string &message) {
    auto params = extractMessageParameters(message);
    if (params.size() < 2)
        throw ProtocolParserException("Invalid map size message: " + message);
    int width = parseIntParameter(params[0]);
    int height = parseIntParameter(params[1]);
    Message msg(Message::MAP_SIZE);
    msg.addIntParam(width);
    msg.addIntParam(height);
    return msg;
}

Message ProtocolParser::parseTileContent(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::TILE_CONTENT);
    return msg;
}

Message ProtocolParser::parseAllTilesContent(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::ALL_TILES_CONTENT);
    return msg;
}

Message ProtocolParser::parseTeamNames(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::TEAM_NAMES);
    return msg;
}

Message ProtocolParser::parsePlayerConnection(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::PLAYER_CONNECTED);
    return msg;
}

Message ProtocolParser::parsePlayerPosition(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::PLAYER_POSITION);
    return msg;
}

Message ProtocolParser::parsePlayerLevel(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::PLAYER_LEVEL);
    return msg;
}

Message ProtocolParser::parsePlayerInventory(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::PLAYER_INVENTORY);
    return msg;
}

Message ProtocolParser::parsePlayerExpulsion(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::PLAYER_EXPELLED);
    return msg;
}

Message ProtocolParser::parsePlayerBroadcast(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::PLAYER_BROADCAST);
    return msg;
}

Message ProtocolParser::parsePlayerDeath(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::PLAYER_DIED);
    return msg;
}

Message ProtocolParser::parseRessourceDrop(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::RESOURCE_DROPPED);
    return msg;
}

Message ProtocolParser::parseRessourceCollect(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::RESOURCE_COLLECTED);
    return msg;
}

Message ProtocolParser::parseIncantationStart(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::INCANTATION_STARTED);
    return msg;
}

Message ProtocolParser::parseIncantationEnd(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::INCANTATION_ENDED);
    return msg;
}

Message ProtocolParser::parseEggDrop(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::EGG_LAID);
    return msg;
}

Message ProtocolParser::parseEggConnection(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::EGG_HATCHED);
    return msg;
}

Message ProtocolParser::parseEggDeath(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::EGG_DIED);
    return msg;
}

Message ProtocolParser::parseEggLaying(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::EGG_LAYING);
    return msg;
}

Message ProtocolParser::parseTimeUnit(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::TIME_UNIT);
    return msg;
}

Message ProtocolParser::parseEndGame(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::GAME_ENDED);
    return msg;
}

Message ProtocolParser::parseServerMessage(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::SERVER_MESSAGE);
    return msg;
}

Message ProtocolParser::parseUnknownCommand(const std::string &message) {
    // TODO(Marin): Implement full parsing logic
    Message msg(Message::UNKNOWN_COMMAND);
    return msg;
}

void ProtocolParser::handleProtocol(const std::string &protocol) {
    // TODO(Marin): Implement protocol handling
}
