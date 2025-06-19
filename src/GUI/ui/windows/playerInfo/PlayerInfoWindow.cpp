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
    const Player* selectedPlayer = findSelectedPlayer();
    if (!selectedPlayer) {
        displayPlayerNotFound();
        return;
    }
    float yOffset = m_position.y + 30;
    displayPlayerIdentity(selectedPlayer, yOffset);
    displayPositionInfo(selectedPlayer, yOffset);
    displayLevelInfo(selectedPlayer, yOffset);
    displayInventory(selectedPlayer, yOffset);
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

const Player* GUI::PlayerInfoWindow::findSelectedPlayer() {
    for (const auto& player : m_gameData.players) {
        if (player.id == m_selectedPlayerId) {
            return &player;
        }
    }
    return nullptr;
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

void GUI::PlayerInfoWindow::displayPlayerIdentity(const Player* selectedPlayer, float& yOffset) {
    std::stringstream idTeam;
    idTeam << "ID: " << selectedPlayer->id << " | Équipe: " << selectedPlayer->team;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        idTeam.str()
    );
    yOffset += 20;
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

void GUI::PlayerInfoWindow::displayPositionInfo(const Player* selectedPlayer, float& yOffset) {
    std::stringstream posInfo;
    std::string orientationStr = getOrientationString(selectedPlayer->orientation);
    posInfo << "Position: (" << selectedPlayer->x << ", " << selectedPlayer->y
            << ") | Orientation: " << orientationStr;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        posInfo.str()
    );
    yOffset += 20;
}

void GUI::PlayerInfoWindow::displayLevelInfo(const Player* selectedPlayer, float& yOffset) {
    std::stringstream levelInfo;
    levelInfo << "Niveau: " << selectedPlayer->level;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        levelInfo.str()
    );
    yOffset += 30;
}

void GUI::PlayerInfoWindow::displayInventory(const Player* selectedPlayer, float& yOffset) {
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        "Inventaire:"
    );
    yOffset += 20;
    const std::pair<std::string, int> resources[] = {
        {"Nourriture", selectedPlayer->inventory.food},
        {"Linemate", selectedPlayer->inventory.linemate},
        {"Deraumere", selectedPlayer->inventory.deraumere},
        {"Sibur", selectedPlayer->inventory.sibur},
        {"Mendiane", selectedPlayer->inventory.mendiane},
        {"Phiras", selectedPlayer->inventory.phiras},
        {"Thystame", selectedPlayer->inventory.thystame}
    };

    for (const auto& resource : resources) {
        std::stringstream ss;
        ss << resource.first << ": " << resource.second;
        m_guiLib->DrawLabel(
            m_position.x + 10,
            yOffset,
            m_dimensions.x - 20,
            20,
            ss.str()
        );
        yOffset += 20;
    }
}

void GUI::PlayerInfoWindow::updateSpecificData(const GUI::GameData& gameData) {
    if (m_hasSelectedPlayer && !playerExistsInGameData(gameData)) {
        m_hasSelectedPlayer = false;
    }
}

bool GUI::PlayerInfoWindow::playerExistsInGameData(const GUI::GameData& gameData) {
    for (const auto& player : gameData.players) {
        if (player.id == m_selectedPlayerId) {
            return true;
        }
    }
    return false;
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

} // namespace GUI
