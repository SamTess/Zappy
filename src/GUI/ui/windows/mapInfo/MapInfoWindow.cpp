/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MapInfoWindow implementation
*/

#include <sstream>
#include <string>
#include <memory>
#include <algorithm>
#include "MapInfoWindow.hpp"
#include "../../../gameController/IRenderable.hpp"

namespace GUI {

MapInfoWindow::MapInfoWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Broadcasts") {
}

void MapInfoWindow::renderContent() {
    const float lineHeight = 20.0f;
    const float panelWidth = m_dimensions.x - 20;
    const float panelHeight = m_dimensions.y - 40;
    const float contentWidth = panelWidth - 20;
    ZappyTypes::Rectangle view = {
        m_position.x + 10,
        m_position.y + 30,
        panelWidth,
        panelHeight
    };
    // int maxVisibleItems = calculateVisibleItemCount(panelHeight, lineHeight);
    int startIndex = 0;

    renderMapInfo(view, startIndex, lineHeight, contentWidth);
}

void GUI::MapInfoWindow::initialize(const ZappyTypes::Vector2& position,
    const ZappyTypes::Vector2& dimensions) {
    m_position = position;
    m_dimensions = dimensions;
    m_visible = true;
    m_showWindowBox = true;
    m_dragging = false;
    m_dragOffset = {0, 0};
    if (m_gameState)
        m_dimensions.y = 120 + (m_gameState->getPlayerCount() * 90);
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
    auto teamname = m_gameState->getTeamNames();
    std::string message;

    (void)startIndex;
    for (const auto& team : m_gameState->getTeamNames()) {
        int totalAlivePlayers = 0;
        textY += lineHeight + 5;
        message = "Team: " + team;
        m_guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
        std::vector<int> totalRessourceTeam = {0, 0 , 0, 0, 0, 0, 0};
        for (const auto& player : m_gameState->getPlayers()) {
            if (player.second->isAlive() && player.second->getTeamName() == team) {
                totalAlivePlayers++;
                auto inventory = m_gameState->getPlayerInventory(player.first);
                std::string messageTmp = "pin #" + std::to_string(player.second->getId()) + "\n";
                m_networkManager->sendCommand(messageTmp);
                if (!inventory)
                    continue;
                totalRessourceTeam[0] += inventory->getResourceCount(0);
                totalRessourceTeam[1] += inventory->getResourceCount(1);
                totalRessourceTeam[2] += inventory->getResourceCount(2);
                totalRessourceTeam[3] += inventory->getResourceCount(3);
                totalRessourceTeam[4] += inventory->getResourceCount(4);
                totalRessourceTeam[5] += inventory->getResourceCount(5);
                totalRessourceTeam[6] += inventory->getResourceCount(6);
            }
        }
        textY += lineHeight + 5;
        message = "Alive players: " + std::to_string(totalAlivePlayers);
        m_guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
        textY += lineHeight;
        message = getFormattedResource(totalRessourceTeam);
        m_guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
    }
}

void GUI::MapInfoWindow::renderMapInfo(const ZappyTypes::Rectangle& view,
    int startIndex, float lineHeight,
    float contentWidth) {

    m_dimensions.y = 120 + (m_gameState->getTeamNames().size() * 90);
    float textY = view.y + 10;
    std::string message = "Map size: width " + std::to_string(m_gameState->getMapWidth()) + ", height " + std::to_string(m_gameState->getMapHeight());

    m_guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
    size_t totalNbPlayers = m_gameState->getPlayerCount();
    textY += lineHeight;
    message = "Total players: " + std::to_string(totalNbPlayers);
    m_guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);

    std::vector<int> totalRessource = {0, 0 , 0, 0, 0, 0, 0};
    auto tiles = m_gameState->getTiles();
    for (const auto& row : tiles) {
        for (const auto& tile : row) {
            totalRessource[0] += tile->getResourceQuantity(ResourceType::FOOD);
            totalRessource[1] += tile->getResourceQuantity(ResourceType::LINEMATE);
            totalRessource[2] += tile->getResourceQuantity(ResourceType::DERAUMERE);
            totalRessource[3] += tile->getResourceQuantity(ResourceType::SIBUR);
            totalRessource[4] += tile->getResourceQuantity(ResourceType::MENDIANE);
            totalRessource[5] += tile->getResourceQuantity(ResourceType::PHIRAS);
            totalRessource[6] += tile->getResourceQuantity(ResourceType::THYSTAME);
        }
    }
    textY += lineHeight;
    message = getFormattedResource(totalRessource);
    m_guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
    renderMapInfoTeam(view, startIndex, lineHeight, contentWidth, textY);
}

void GUI::MapInfoWindow::setNetworkManager(std::shared_ptr<NetworkManager> networkManager) {
    m_networkManager = networkManager;
}

} // namespace GUI
