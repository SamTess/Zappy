/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** TileInfoWindow implementation
*/

#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "TileInfoWindow.hpp"

namespace GUI {

TileInfoWindow::TileInfoWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Informations sur la case") {
    _selectedTile = {0, 0, false};
}

void GUI::TileInfoWindow::renderContent() {
    if (!_selectedTile.selected) {
        renderNoTileSelected();
        return;
    }
    renderTilePosition();
    float yOffset = renderResourceInfo();
    renderPlayerCount(yOffset);
}

void GUI::TileInfoWindow::renderNoTileSelected() {
    _guiLib->DrawLabel(_position.x + 10, _position.y + 40, _dimensions.x - 20, 30, "Aucune case sélectionnée");
}

void GUI::TileInfoWindow::renderTilePosition() {
    std::stringstream tilePos;
    tilePos << "Position: (" << _selectedTile.x << ", " << _selectedTile.y << ")";
    _guiLib->DrawLabel(_position.x + 10, _position.y + 30, _dimensions.x - 20, 20, tilePos.str() );
}

float GUI::TileInfoWindow::renderResourceInfo() {
    int resources[7] = {0};
    if (_gameState && _gameState->isMapInitialized()) {
        auto tile = _gameState->getTile(_selectedTile.x, _selectedTile.y);
        if (tile) {
            const auto& tileResources = tile->getResources();
            for (int i = 0; i < 7; ++i) {
                resources[i] = tileResources[i];
            }
        }
    }
    const std::vector<std::string> resourceNames = {"Nourriture", "Linemate", "Deraumere", "Sibur",
        "Mendiane", "Phiras", "Thystame"};
    float yOffset = _position.y + 60;
    for (int i = 0; i < 7; i++) {
        std::stringstream ss;
        ss << resourceNames[i] << ": " << resources[i];
        _guiLib->DrawLabel(
            _position.x + 10,
            yOffset,
            _dimensions.x - 20,
            20,
            ss.str()
        );
        yOffset += 20;
    }
    return yOffset;
}

void GUI::TileInfoWindow::renderPlayerCount(float yOffset) {
    int playerCount = 0;

    if (_gameState && _gameState->isMapInitialized()) {
        auto playerIds = _gameState->getPlayersOnTile(_selectedTile.x, _selectedTile.y);
        playerCount = static_cast<int>(playerIds.size());
    }
    std::stringstream playersTitle;
    playersTitle << "Joueurs sur cette case: " << playerCount;
    _guiLib->DrawLabel(_position.x + 10, yOffset, _dimensions.x - 20, 20, playersTitle.str());
}

void GUI::TileInfoWindow::setSelectedTile(int x, int y) {
    _selectedTile.x = x;
    _selectedTile.y = y;
    _selectedTile.selected = true;
    setVisible(true);
}

std::pair<int, int> GUI::TileInfoWindow::getSelectedTile() const {
    return {_selectedTile.x, _selectedTile.y};
}

bool GUI::TileInfoWindow::hasTileSelected() const {
    return _selectedTile.selected;
}

} // namespace GUI
