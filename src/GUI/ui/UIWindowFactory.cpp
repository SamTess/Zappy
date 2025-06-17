/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** UIWindowFactory implementation
*/

#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <utility>
#include "UIWindowFactory.hpp"
#include "windows/logs/LogsWindow.hpp"
#include "windows/tileInfo/TileInfoWindow.hpp"
#include "windows/playerInfo/PlayerInfoWindow.hpp"
#include "windows/broadcasts/BroadcastsWindow.hpp"
#include "windows/controls/ControlsWindow.hpp"
#include "windows/timeInfo/TimeInfoWindow.hpp"
#include "windows/menu/MenuWindow.hpp"

namespace GUI {

UIWindowFactory::UIWindowFactory(std::shared_ptr<IGuiLib> guiLib)
    : m_guiLib(guiLib), m_currentViewMode(0) {
    m_selectedTile = {0, 0, false};
    m_viewModes = {"Vue standard", "Vue aérienne", "Vue wireframe", "Vue ressources"};
}

void UIWindowFactory::createAllWindows(int, int) {
    // Positions initiales pour les fenêtres
    ZappyTypes::Vector2 logsPos = {20, 40};
    ZappyTypes::Vector2 tileInfoPos = {50, 50};
    ZappyTypes::Vector2 playerInfoPos = {80, 60};
    ZappyTypes::Vector2 broadcastsPos = {110, 70};
    ZappyTypes::Vector2 controlsPos = {140, 80};
    ZappyTypes::Vector2 timeInfoPos = {170, 90};
    ZappyTypes::Vector2 menuPos = {10, 10};

    // Dimensions des fenêtres
    ZappyTypes::Vector2 standardDimensions = {400, 300};
    ZappyTypes::Vector2 timeInfoDimensions = {250, 150};
    ZappyTypes::Vector2 menuDimensionsLarger = {40, 40};

    // Création des fenêtres
    auto logsWindow = std::make_shared<LogsWindow>(m_guiLib);
    logsWindow->initialize(logsPos, standardDimensions);
    logsWindow->setVisible(true);
    m_windows["logs"] = logsWindow;

    auto tileInfoWindow = std::make_shared<TileInfoWindow>(m_guiLib);
    tileInfoWindow->initialize(tileInfoPos, standardDimensions);
    tileInfoWindow->setVisible(true);
    m_windows["tileInfo"] = tileInfoWindow;

    auto playerInfoWindow = std::make_shared<PlayerInfoWindow>(m_guiLib);
    playerInfoWindow->initialize(playerInfoPos, standardDimensions);
    playerInfoWindow->setVisible(true);
    m_windows["playerInfo"] = playerInfoWindow;

    auto broadcastsWindow = std::make_shared<BroadcastsWindow>(m_guiLib);
    broadcastsWindow->initialize(broadcastsPos, standardDimensions);
    broadcastsWindow->setVisible(true);
    m_windows["broadcasts"] = broadcastsWindow;

    auto controlsWindow = std::make_shared<ControlsWindow>(m_guiLib);
    controlsWindow->initialize(controlsPos, standardDimensions);
    controlsWindow->setVisible(true);
    m_windows["controls"] = controlsWindow;

    auto timeInfoWindow = std::make_shared<TimeInfoWindow>(m_guiLib);
    timeInfoWindow->initialize(timeInfoPos, timeInfoDimensions);
    timeInfoWindow->setVisible(true);
    m_windows["timeInfo"] = timeInfoWindow;

    auto menuWindow = std::make_shared<MenuWindow>(m_guiLib);
    menuWindow->initialize(menuPos, menuDimensionsLarger);
    menuWindow->setVisible(true);
    m_windows["menu"] = menuWindow;

    // Initialisation des messages de log
    addLogMessage("Bienvenue dans Zappy GUI!");
    addLogMessage("Connecté au serveur");
    addLogMessage("Interface utilisateur initialisée");

    // Ajout d'un broadcast d'exemple
    addBroadcast("Team1", "Je viens de monter niveau 2!");
    addBroadcast("Team2", "Besoin d'aide à la position (5,7)");
}

std::shared_ptr<IUIWindow> UIWindowFactory::getWindow(const std::string& windowId) {
    auto it = m_windows.find(windowId);
    if (it != m_windows.end()) {
        return it->second;
    }
    return nullptr;
}

void UIWindowFactory::renderAllWindows() {
    std::string menuId = "menu";
    for (auto& pair : m_windows) {
        if (pair.first != menuId && pair.second->isVisible()) {
            bool result = pair.second->render();
            if (!result)
                pair.second->setVisible(true);
        }
    }
    auto menuIt = m_windows.find(menuId);
    if (menuIt != m_windows.end() && menuIt->second->isVisible()) {
        bool result = menuIt->second->render();
        if (!result) {
            menuIt->second->setVisible(true);
        }
    } else {
        std::cout << "  La fenêtre menu n'est pas visible ou n'existe pas" << std::endl;
    }
}

void UIWindowFactory::updateAllWindows(const GameData& gameData) {
    for (auto& pair : m_windows) {
        pair.second->updateData(gameData);
    }
}

void UIWindowFactory::addLogMessage(const std::string& message) {
    auto logsWindow = std::dynamic_pointer_cast<LogsWindow>(m_windows["logs"]);
    if (logsWindow) {
        logsWindow->addLogMessage(message);
    }
}

void UIWindowFactory::setSelectedTile(int x, int y) {
    m_selectedTile = {x, y, true};
    auto tileInfoWindow = std::dynamic_pointer_cast<TileInfoWindow>(m_windows["tileInfo"]);
    if (tileInfoWindow) {
        tileInfoWindow->setSelectedTile(x, y);
    }
    std::stringstream ss;
    ss << "Sélection de la case (" << x << ", " << y << ")";
    addLogMessage(ss.str());
}

void UIWindowFactory::addBroadcast(const std::string& team, const std::string& message) {
    auto broadcastsWindow = std::dynamic_pointer_cast<BroadcastsWindow>(m_windows["broadcasts"]);
    if (broadcastsWindow)
        broadcastsWindow->addBroadcast(team, message);
    std::stringstream ss;
    ss << "[BROADCAST:" << team << "] " << message;
    addLogMessage(ss.str());
}

void UIWindowFactory::setViewMode(int mode) {
    if (mode >= 0 && mode < static_cast<int>(m_viewModes.size())) {
        m_currentViewMode = mode;
        auto controlsWindow = std::dynamic_pointer_cast<ControlsWindow>(m_windows["controls"]);
        if (controlsWindow)
            controlsWindow->setViewMode(mode);
        std::stringstream ss;
        ss << "Mode de vue changé: " << m_viewModes[mode];
        addLogMessage(ss.str());
    }
}

void UIWindowFactory::handleWindowDragging(const ZappyTypes::Vector2& mousePosition) {
    std::vector<std::pair<std::string, std::shared_ptr<IUIWindow>>> windowsVec;
    windowsVec.reserve(m_windows.size());
    for (const auto& pair : m_windows)
        windowsVec.push_back(pair);
    for (auto it = windowsVec.rbegin(); it != windowsVec.rend(); ++it) {
        if (it->second->isVisible() && it->second->startDragging(mousePosition))
            break;
    }
}

void UIWindowFactory::updateWindowDragging(const ZappyTypes::Vector2& mousePosition) {
    for (auto& pair : m_windows) {
        if (pair.second->isDragging())
            pair.second->updateDragging(mousePosition);
    }
}

void UIWindowFactory::stopWindowDragging() {
    for (auto& pair : m_windows) {
        if (pair.second->isDragging())
            pair.second->stopDragging();
    }
}

} // namespace GUI
