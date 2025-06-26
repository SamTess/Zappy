/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** TileInfoWindow
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
    _x = 0;
    _y = 0;
    _isSelecting = false;
}

void GUI::TileInfoWindow::renderContent() {
    if (!_isSelecting) {
        renderNoTileSelected();
        return;
    }
    renderTilePosition();
    float yOffset = renderResourceInfo();
    renderPlayerCount(yOffset);
}

void GUI::TileInfoWindow::renderNoTileSelected() {
    _guiLib->DrawLabel( _position.x + 10, _position.y + 40, _dimensions.x - 20,
        30, "Aucune case sélectionnée"
    );
}

void GUI::TileInfoWindow::renderTilePosition() {
    std::stringstream tilePos;
    tilePos << "Position: (" << _x << ", " << _y << ")";
    _guiLib->DrawLabel(_position.x + 10, _position.y + 30, _dimensions.x - 20,
        20, tilePos.str());
}

float GUI::TileInfoWindow::renderResourceInfo() {
    int resources[7] = {0};
    if (_dataProvider && _dataProvider->isMapInitialized()) {
        auto tile = _dataProvider->getTile(_x, _y);
        if (tile) {
            const auto& tileResources = tile->getResources();
            for (int i = 0; i < 7; ++i) {
                resources[i] = tileResources[i];
            }
        }
    }
    const std::vector<std::string> resourceNames = {
        "Nourriture", "Linemate", "Deraumere", "Sibur",
        "Mendiane", "Phiras", "Thystame"
    };
    float yOffset = _position.y + 60;
    for (int i = 0; i < 7; i++) {
        std::stringstream ss;
        ss << resourceNames[i] << ": " << resources[i];
        _guiLib->DrawLabel(_position.x + 10, yOffset, _dimensions.x - 20,
            20, ss.str());
        yOffset += 20;
    }
    return yOffset;
}

void GUI::TileInfoWindow::renderPlayerCount(float yOffset) {
    int playerCount = 0;
    if (_dataProvider && _dataProvider->isMapInitialized()) {
        auto playerIds = _dataProvider->getPlayersOnTile(_x, _y);
        playerCount = static_cast<int>(playerIds.size());
    }
    std::stringstream playersTitle;
    playersTitle << "Joueurs sur cette case: " << playerCount;
    _guiLib->DrawLabel(_position.x + 10, yOffset, _dimensions.x - 20,
        20, playersTitle.str());
}

void GUI::TileInfoWindow::setSelectedTile(int x, int y) {
    _x = x;
    _y = y;
    _isSelecting = true;
    setVisible(true);
}

bool GUI::TileInfoWindow::hasTileSelected() const {
    return _isSelecting;
}

} // namespace GUI
