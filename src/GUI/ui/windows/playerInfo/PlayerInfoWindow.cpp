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

void PlayerInfoWindow::renderContent() {
    if (!m_hasSelectedPlayer) {
        m_guiLib->DrawLabel(
            m_position.x + 10,
            m_position.y + 40,
            m_dimensions.x - 20,
            30,
            "Aucun joueur sélectionné"
        );
        return;
    }

    // Recherche le joueur dans les données de jeu
    const Player* selectedPlayer = nullptr;
    for (const auto& player : m_gameData.players) {
        if (player.id == m_selectedPlayerId) {
            selectedPlayer = &player;
            break;
        }
    }

    if (!selectedPlayer) {
        m_guiLib->DrawLabel(
            m_position.x + 10,
            m_position.y + 40,
            m_dimensions.x - 20,
            30,
            "Joueur introuvable"
        );
        return;
    }

    // Affiche les informations du joueur
    float yOffset = m_position.y + 30;

    // ID et équipe
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

    // Position et orientation
    std::stringstream posInfo;
    std::string orientationStr;
    switch (selectedPlayer->orientation) {
        case 1: orientationStr = "Nord"; break;
        case 2: orientationStr = "Est"; break;
        case 3: orientationStr = "Sud"; break;
        case 4: orientationStr = "Ouest"; break;
        default: orientationStr = "Inconnue";
    }
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

    // Niveau
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

    // Inventaire
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        "Inventaire:"
    );
    yOffset += 20;

    // Liste des ressources dans l'inventaire
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

void PlayerInfoWindow::updateSpecificData(const GameData& gameData) {
    // Les données sont déjà mises à jour par la classe parente

    // Vérifie si le joueur sélectionné existe toujours
    if (m_hasSelectedPlayer) {
        bool playerExists = false;
        for (const auto& player : gameData.players) {
            if (player.id == m_selectedPlayerId) {
                playerExists = true;
                break;
            }
        }
        if (!playerExists)
            m_hasSelectedPlayer = false;
    }
}

void PlayerInfoWindow::setSelectedPlayer(int playerId) {
    m_selectedPlayerId = playerId;
    m_hasSelectedPlayer = true;
    setVisible(true);
}

int PlayerInfoWindow::getSelectedPlayer() const {
    return m_selectedPlayerId;
}

bool PlayerInfoWindow::hasPlayerSelected() const {
    return m_hasSelectedPlayer;
}

} // namespace GUI
