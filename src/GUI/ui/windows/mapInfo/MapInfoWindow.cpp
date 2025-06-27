/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MapInfoWindow
*/

#include <sstream>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include "MapInfoWindow.hpp"

namespace GUI {

MapInfoWindow::MapInfoWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Informations de la carte") {
}

void MapInfoWindow::renderContent() {
    if (!_dataProvider) {
        _guiLib->DrawLabel(_position.x + 10, _position.y + 40, _dimensions.x - 20,
            30, "Données non disponibles");
        return;
    }
    if (_dataProvider->getMapWidth() == 0 || _dataProvider->getMapHeight() == 0) {
        _guiLib->DrawLabel(_position.x + 10, _position.y + 40, _dimensions.x - 20,
            30, "Carte non initialisée");
        return;
    }
    const float lineHeight = 20.0f;
    const float panelWidth = _dimensions.x - 20;
    const float panelHeight = _dimensions.y - 40;
    const float contentWidth = panelWidth - 20;
    ZappyTypes::Rectangle view = {_position.x + 10, _position.y + 30, panelWidth, panelHeight};
    int startIndex = 0;
    renderMapInfo(view, startIndex, lineHeight, contentWidth);
}

void GUI::MapInfoWindow::initialize(const ZappyTypes::Vector2& position,
    const ZappyTypes::Vector2& dimensions) {
    _position = position;
    _dimensions = dimensions;
    _visible = true;
    _showWindowBox = true;
    _dragging = false;
    _dragOffset = {0, 0};
}

int GUI::MapInfoWindow::calculateVisibleItemCount(float panelHeight, float lineHeight) {
    return static_cast<int>(panelHeight / lineHeight);
}

std::string GUI::MapInfoWindow::getFormattedResource(std::vector<int> totalRessource) {
    std::string message;

    message = "Total resources: ";
    message += "Food: " + std::to_string(totalRessource[0]) + ", ";
    message += "Linemate: " + std::to_string(totalRessource[1]) + ", ";
    message += "Deraumere: " + std::to_string(totalRessource[2]) + ", ";
    message += "Sibur: " + std::to_string(totalRessource[3]) + "\n";
    message += "Mendiane: " + std::to_string(totalRessource[4]) + ", ";
    message += "Phiras: " + std::to_string(totalRessource[5]) + ", ";
    message += "Thystame: " + std::to_string(totalRessource[6]);
    return message;
}

void GUI::MapInfoWindow::renderMapInfoTeam(const ZappyTypes::Rectangle& view,
    int startIndex, float lineHeight, float contentWidth, int textY) {
    if (!_dataProvider)
        return;
    auto teamNames = _dataProvider->getTeamNames();
    auto teamPlayerCounts = _dataProvider->getTeamPlayerCounts();
    auto teamResourceTotals = _dataProvider->getTeamResourceTotals();
    std::string message;
    (void)startIndex;
    for (const auto& team : teamNames) {
        textY += lineHeight + 5;
        message = "Team: " + team;
        _guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
                textY += lineHeight + 5;
        int alivePlayers = teamPlayerCounts.count(team) ? teamPlayerCounts.at(team) : 0;
        message = "Alive players: " + std::to_string(alivePlayers);
        _guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
        textY += lineHeight;
        if (teamResourceTotals.count(team)) {
            message = getFormattedResource(teamResourceTotals.at(team));
            _guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
        }
        if (_uiContext) {
            auto playerIds = _dataProvider->getPlayerIds();
            for (int playerId : playerIds) {
                auto player = _dataProvider->getPlayerInfo(playerId);
                if (player && player->getTeamName() == team) {
                    std::string pinCommand = "pin #" + std::to_string(playerId) + "\n";
                    _uiContext->executeNetworkCommand(pinCommand);
                }
            }
        }
    }
}

void GUI::MapInfoWindow::renderMapInfo(const ZappyTypes::Rectangle& view,
    int startIndex, float lineHeight,
    float contentWidth) {
    if (!_dataProvider)
        return;
    auto teamNames = _dataProvider->getTeamNames();
    _dimensions.y = 120 + (teamNames.size() * 90);
    float textY = view.y + 10;
    std::string message = "Map size: width " + std::to_string(_dataProvider->getMapWidth()) +
                         ", height " + std::to_string(_dataProvider->getMapHeight());
    _guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
    textY += lineHeight;
    auto playerIds = _dataProvider->getPlayerIds();
    message = "Total players: " + std::to_string(playerIds.size());
    _guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
    textY += lineHeight;
    auto totalResources = _dataProvider->calculateTotalResources();
    message = getFormattedResource(totalResources);
    _guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
    renderMapInfoTeam(view, startIndex, lineHeight, contentWidth, textY);
}

void GUI::MapInfoWindow::setCommandSender(std::shared_ptr<INetworkCommandSender> sender) {
    _commandSender = sender;
}

} // namespace GUI
