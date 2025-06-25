/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** PlayerInfoWindow implementation
*/

#include <sstream>
#include <algorithm>
#include <string>
#include <utility>
#include <memory>
#include "PlayerInfoWindow.hpp"
#include "../../../network/networkManager/NetworkManager.hpp"

namespace GUI {

PlayerInfoWindow::PlayerInfoWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Informations sur le joueur"),
      m_selectedPlayerId(-1),
      m_hasSelectedPlayer(false) {
}

void GUI::PlayerInfoWindow::renderContent() {
    if (!m_hasSelectedPlayer) {
        displayNoPlayerSelected();
        return;
    }
    auto selectedPlayer = findSelectedPlayer();
    if (!selectedPlayer) {
        displayPlayerNotFound();
        return;
    }
    if (m_networkManager && m_networkManager->isConnected()) {
        std::stringstream pinCommand;
        pinCommand << "pin #" << m_selectedPlayerId;
        m_networkManager->sendCommand(pinCommand.str());
    }
    float yOffset = m_position.y + 30;
    yOffset = displayPlayerIdentity(selectedPlayer, yOffset);
    yOffset = displayPositionInfo(selectedPlayer, yOffset);
    yOffset = displayLevelInfo(selectedPlayer, yOffset);
    yOffset = displayInventory(selectedPlayer, yOffset);
}

void GUI::PlayerInfoWindow::displayNoPlayerSelected() {
    m_guiLib->DrawLabel(
        m_position.x + 10,
        m_position.y + 40,
        m_dimensions.x - 20,
        30,
        "Aucun joueur sélectionné"
    );
}

std::shared_ptr<const IPlayer> GUI::PlayerInfoWindow::findSelectedPlayer() {
    if (!m_gameState) {
        return nullptr;
    }
    return m_gameState->getPlayerInfo(m_selectedPlayerId);
}

void GUI::PlayerInfoWindow::displayPlayerNotFound() {
    m_guiLib->DrawLabel(
        m_position.x + 10,
        m_position.y + 40,
        m_dimensions.x - 20,
        30,
        "Joueur introuvable"
    );
}

float GUI::PlayerInfoWindow::displayPlayerIdentity(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset) {
    std::stringstream idTeam;
    idTeam << "ID: " << selectedPlayer->getId() << " | Équipe: " << selectedPlayer->getTeamName();
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        idTeam.str()
    );
    return yOffset + 20;
}

std::string GUI::PlayerInfoWindow::getOrientationString(int orientation) {
    switch (orientation) {
        case 1: return "Nord";
        case 2: return "Est";
        case 3: return "Sud";
        case 4: return "Ouest";
        default: return "Inconnue";
    }
}

float GUI::PlayerInfoWindow::displayPositionInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset) {
    std::stringstream posInfo;
    std::string orientationStr = getOrientationString(selectedPlayer->getOrientation());
    posInfo << "Position: (" << selectedPlayer->getX() << ", " << selectedPlayer->getY()
            << ") | Orientation: " << orientationStr;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        posInfo.str()
    );
    return yOffset + 20;
}

float GUI::PlayerInfoWindow::displayLevelInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset) {
    std::stringstream levelInfo;
    levelInfo << "Niveau: " << selectedPlayer->getLevel();
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        levelInfo.str()
    );
    return yOffset + 30;
}

float GUI::PlayerInfoWindow::displayInventory(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset) {
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        "Inventaire:"
    );
    float newYOffset = yOffset + 20;
    if (m_gameState) {
        auto inventory = m_gameState->getPlayerInventory(selectedPlayer->getId());
        if (inventory) {
            const std::pair<std::string, int> resources[] = {
                {"Nourriture", inventory->getFood()},
                {"Linemate", inventory->getLinemate()},
                {"Deraumere", inventory->getDeraumere()},
                {"Sibur", inventory->getSibur()},
                {"Mendiane", inventory->getMendiane()},
                {"Phiras", inventory->getPhiras()},
                {"Thystame", inventory->getThystame()}
            };

            for (const auto& resource : resources) {
                std::stringstream ss;
                ss << resource.first << ": " << resource.second;
                m_guiLib->DrawLabel(
                    m_position.x + 10,
                    newYOffset,
                    m_dimensions.x - 20,
                    20,
                    ss.str()
                );
                newYOffset += 20;
            }
        }
    }
    return newYOffset;
}

bool GUI::PlayerInfoWindow::playerExistsInGameState(std::shared_ptr<const GameState> gameState) {
    if (!gameState) {
        return false;
    }
    auto player = gameState->getPlayerInfo(m_selectedPlayerId);
    return player != nullptr;
}

void GUI::PlayerInfoWindow::setSelectedPlayer(int playerId) {
    m_selectedPlayerId = playerId;
    m_hasSelectedPlayer = true;
    setVisible(true);
}

int GUI::PlayerInfoWindow::getSelectedPlayer() const {
    return m_selectedPlayerId;
}

bool GUI::PlayerInfoWindow::hasPlayerSelected() const {
    return m_hasSelectedPlayer;
}

void GUI::PlayerInfoWindow::setNetworkManager(std::shared_ptr<NetworkManager> networkManager) {
    m_networkManager = networkManager;
}

} // namespace GUI
