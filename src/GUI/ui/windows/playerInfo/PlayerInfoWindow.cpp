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
      _selectedPlayerId(-1), _hasSelectedPlayer(false) {
}

void GUI::PlayerInfoWindow::renderContent() {
    if (!_hasSelectedPlayer) {
        displayNoPlayerSelected();
        return;
    }
    auto selectedPlayer = findSelectedPlayer();
    if (!selectedPlayer) {
        displayPlayerNotFound();
        return;
    }
    if (_uiContext) {
        std::stringstream pinCommand;
        pinCommand << "pin #" << _selectedPlayerId;
        _uiContext->executeNetworkCommand(pinCommand.str());
    }
    float yOffset = _position.y + 30;
    yOffset = displayPlayerIdentity(selectedPlayer, yOffset);
    yOffset = displayPositionInfo(selectedPlayer, yOffset);
    yOffset = displayLevelInfo(selectedPlayer, yOffset);
    yOffset = displayInventory(selectedPlayer, yOffset);
}

void GUI::PlayerInfoWindow::displayNoPlayerSelected() {
    _guiLib->DrawLabel(_position.x + 10, _position.y + 40, _dimensions.x - 20,
        30, "Aucun joueur sélectionné");
}

std::shared_ptr<const IPlayer> GUI::PlayerInfoWindow::findSelectedPlayer() {
    if (!_dataProvider)
        return nullptr;
    return _dataProvider->getPlayerInfo(_selectedPlayerId);
}

void GUI::PlayerInfoWindow::displayPlayerNotFound() {
    _guiLib->DrawLabel(_position.x + 10, _position.y + 40, _dimensions.x - 20,
        30, "Joueur introuvable");
}

float GUI::PlayerInfoWindow::displayPlayerIdentity(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset) {
    std::stringstream idTeam;
    idTeam << "ID: " << selectedPlayer->getId() << " | Équipe: " << selectedPlayer->getTeamName();
    _guiLib->DrawLabel(_position.x + 10, yOffset, _dimensions.x - 20,
        20, idTeam.str());
    return yOffset + 20;
}

std::string GUI::PlayerInfoWindow::getOrientationString(int orientation) {
    switch (orientation) {
        case 1:
            return "Nord";
        case 2:
            return "Est";
        case 3:
            return "Sud";
        case 4:
            return "Ouest";
        default:
            return "Inconnue";
    }
}

float GUI::PlayerInfoWindow::displayPositionInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset) {
    std::stringstream posInfo;
    std::string orientationStr = getOrientationString(selectedPlayer->getOrientation());
    posInfo << "Position: (" << selectedPlayer->getX() << ", " << selectedPlayer->getY()
            << ") | Orientation: " << orientationStr;
    _guiLib->DrawLabel(_position.x + 10, yOffset, _dimensions.x - 20,
        20, posInfo.str());
    return yOffset + 20;
}

float GUI::PlayerInfoWindow::displayLevelInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset) {
    std::stringstream levelInfo;
    levelInfo << "Niveau: " << selectedPlayer->getLevel();
    _guiLib->DrawLabel(_position.x + 10, yOffset,
        _dimensions.x - 20, 20, levelInfo.str());
    return yOffset + 30;
}

float GUI::PlayerInfoWindow::displayInventory(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset) {
    _guiLib->DrawLabel( _position.x + 10, yOffset, _dimensions.x - 20,
        20, "Inventaire:");
    float newYOffset = yOffset + 20;
    if (_dataProvider) {
        auto inventory = _dataProvider->getPlayerInventory(selectedPlayer->getId());
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
                _guiLib->DrawLabel(_position.x + 10, newYOffset,
                    _dimensions.x - 20, 20, ss.str());
                newYOffset += 20;
            }
        }
    }
    return newYOffset;
}

bool GUI::PlayerInfoWindow::playerExistsInDataProvider() {
    if (!_dataProvider)
        return false;
    auto player = _dataProvider->getPlayerInfo(_selectedPlayerId);
    return player != nullptr;
}

void GUI::PlayerInfoWindow::setSelectedPlayer(int playerId) {
    _selectedPlayerId = playerId;
    _hasSelectedPlayer = true;
    setVisible(true);
}

int GUI::PlayerInfoWindow::getSelectedPlayer() const {
    return _selectedPlayerId;
}

bool GUI::PlayerInfoWindow::hasPlayerSelected() const {
    return _hasSelectedPlayer;
}

void GUI::PlayerInfoWindow::setCommandSender(std::shared_ptr<INetworkCommandSender> commandSender) {
    _commandSender = commandSender;
}

} // namespace GUI
