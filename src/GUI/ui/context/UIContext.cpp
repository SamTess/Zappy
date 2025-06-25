/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** UI Context implementation
*/

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include "UIContext.hpp"
#include "../UIWindowFactory.hpp"

namespace GUI {

UIContext::UIContext(std::shared_ptr<UIWindowFactory> windowFactory,
                     std::shared_ptr<INetworkCommandSender> commandSender)
    : m_windowFactory(windowFactory),
      m_commandSender(commandSender),
      m_nextCallbackId(0) {
}

std::shared_ptr<IUIDataProvider> UIContext::getDataProvider() const {
    return m_dataProvider;
}

void UIContext::setDataProvider(std::shared_ptr<IUIDataProvider> dataProvider) {
    m_dataProvider = dataProvider;
    notifyDataUpdate();
}

void UIContext::requestWindowClose(const std::string& windowId) {
    if (!isValidWindowId(windowId)) {
        logSecurityWarning("requestWindowClose", "Invalid window ID: " + windowId);
        return;
    }

    if (m_windowFactory) {
        auto window = m_windowFactory->getWindow(windowId);
        if (window)
            window->setVisible(false);
    }
}

void UIContext::requestWindowVisibilityChange(const std::string& windowId, bool visible) {
    if (!isValidWindowId(windowId)) {
        logSecurityWarning("requestWindowVisibilityChange", "Invalid window ID: " + windowId);
        return;
    }
    if (m_windowFactory) {
        auto window = m_windowFactory->getWindow(windowId);
        if (window)
            window->setVisible(visible);
    }
}

void UIContext::notifyTileSelection(int x, int y) {
    if (m_windowFactory)
        m_windowFactory->setSelectedTile(x, y);
}

void UIContext::notifyPlayerSelection(int playerId) {
    if (m_windowFactory)
        m_windowFactory->setSelectedPlayer(playerId);
}

void UIContext::notifyViewModeChange(int viewMode) {
    if (m_windowFactory)
        m_windowFactory->setViewMode(viewMode);
}

void UIContext::executeNetworkCommand(const std::string& command) {
    if (!m_commandSender) {
        logSecurityWarning("executeNetworkCommand", "No command sender available");
        return;
    }
    if (command.empty() || command.size() > 1024) {
        logSecurityWarning("executeNetworkCommand", "Invalid command format");
        return;
    }
    m_commandSender->sendCommand(command);
}

void UIContext::setCommandSender(std::shared_ptr<INetworkCommandSender> commandSender) {
    m_commandSender = commandSender;
}

void UIContext::registerDataUpdateCallback(std::function<void()> callback) {
    std::string callbackId = generateCallbackId();
    m_dataUpdateCallbacks[callbackId] = callback;
}

void UIContext::unregisterDataUpdateCallback(const std::string& callbackId) {
    m_dataUpdateCallbacks.erase(callbackId);
}

void UIContext::notifyDataUpdate() {
    for (const auto& pair : m_dataUpdateCallbacks) {
        try {
            pair.second();
        } catch (const std::exception& e) {
            logSecurityWarning("notifyDataUpdate", "Callback exception: " + std::string(e.what()));
        }
    }
}

std::string UIContext::generateCallbackId() {
    std::stringstream ss;
    ss << "callback_" << (++m_nextCallbackId);
    return ss.str();
}

bool UIContext::isValidWindowId(const std::string& windowId) const {
    static const std::vector<std::string> validWindowIds = {
        "tileInfo", "playerInfo", "broadcasts", "controls", "timeInfo", "menu"
    };
    return std::find(validWindowIds.begin(), validWindowIds.end(), windowId) != validWindowIds.end();
}

void UIContext::logSecurityWarning(const std::string& operation, const std::string& details) const {
    std::cerr << "[UIContext Security Warning] " << operation << ": " << details << std::endl;
}

} // namespace GUI
