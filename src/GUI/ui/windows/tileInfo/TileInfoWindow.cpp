/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** TileInfoWindow implementation
*/

#include <sstream>
#include <memory>
#include <utility>
#include "TileInfoWindow.hpp"

namespace GUI {

TileInfoWindow::TileInfoWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Informations sur la case") {
    m_selectedTile = {0, 0, false};
}

void TileInfoWindow::renderContent() {
    if (!m_selectedTile.selected) {
        m_guiLib->DrawLabel(
            m_position.x + 10,
            m_position.y + 40,
            m_dimensions.x - 20,
            30,
            "Aucune case sélectionnée"
        );
        return;
    }
    std::stringstream tilePos;
    tilePos << "Position: (" << m_selectedTile.x << ", " << m_selectedTile.y << ")";
    m_guiLib->DrawLabel(
        m_position.x + 10,
        m_position.y + 30,
        m_dimensions.x - 20,
        20,
        tilePos.str()
    );
    int resources[7] = {0};
    for (const auto& tile : m_gameData.tiles) {
        if (tile.x == m_selectedTile.x && tile.y == m_selectedTile.y) {
            resources[0] = tile.food;
            resources[1] = tile.linemate;
            resources[2] = tile.deraumere;
            resources[3] = tile.sibur;
            resources[4] = tile.mendiane;
            resources[5] = tile.phiras;
            resources[6] = tile.thystame;
            break;
        }
    }
    const char* resourceNames[] = {
        "Nourriture", "Linemate", "Deraumere", "Sibur",
        "Mendiane", "Phiras", "Thystame"
    };
    float yOffset = m_position.y + 60;
    for (int i = 0; i < 7; i++) {
        std::stringstream ss;
        ss << resourceNames[i] << ": " << resources[i];
        m_guiLib->DrawLabel(
            m_position.x + 10,
            yOffset,
            m_dimensions.x - 20,
            20,
            ss.str()
        );
        yOffset += 20;
    }
    std::stringstream playersTitle;
    int playerCount = 0;
    for (const auto& player : m_gameData.players) {
        if (player.x == m_selectedTile.x && player.y == m_selectedTile.y) {
            playerCount++;
        }
    }
    playersTitle << "Joueurs sur cette case: " << playerCount;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        playersTitle.str()
    );
}

void TileInfoWindow::updateSpecificData(const GameData&) {
}

void TileInfoWindow::setSelectedTile(int x, int y) {
    m_selectedTile.x = x;
    m_selectedTile.y = y;
    m_selectedTile.selected = true;
    setVisible(true);
}

std::pair<int, int> TileInfoWindow::getSelectedTile() const {
    return {m_selectedTile.x, m_selectedTile.y};
}

bool TileInfoWindow::hasTileSelected() const {
    return m_selectedTile.selected;
}

} // namespace GUI
