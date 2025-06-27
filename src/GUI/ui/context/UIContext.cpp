/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** UIContext
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
    : _windowFactory(windowFactory),
      _commandSender(commandSender),
      _nextCallbackId(0) {
}

std::shared_ptr<IUIDataProvider> UIContext::getDataProvider() const {
    return _dataProvider;
}

void UIContext::setDataProvider(std::shared_ptr<IUIDataProvider> dataProvider) {
    _dataProvider = dataProvider;
    notifyDataUpdate();
}

void UIContext::requestWindowClose(const std::string& windowId) {
    if (!isValidWindowId(windowId)) {
        logSecurityWarning("requestWindowClose", "Invalid window ID: " + windowId);
        return;
    }

    if (_windowFactory) {
        auto window = _windowFactory->getWindow(windowId);
        if (window)
            window->setVisible(false);
    }
}

void UIContext::requestWindowVisibilityChange(const std::string& windowId, bool visible) {
    if (!isValidWindowId(windowId)) {
        logSecurityWarning("requestWindowVisibilityChange", "Invalid window ID: " + windowId);
        return;
    }
    if (_windowFactory) {
        auto window = _windowFactory->getWindow(windowId);
        if (window)
            window->setVisible(visible);
    }
}

void UIContext::notifyTileSelection(int x, int y) {
    if (_windowFactory)
        _windowFactory->setSelectedTile(x, y);
}

void UIContext::notifyPlayerSelection(int playerId) {
    if (_windowFactory)
        _windowFactory->setSelectedPlayer(playerId);
}

void UIContext::executeNetworkCommand(const std::string& command) {
    if (!_commandSender)
        return;
    if (command.empty() || command.size() > 1024) {
        logSecurityWarning("executeNetworkCommand", "Invalid command format");
        return;
    }
    _commandSender->sendCommand(command);
}

void UIContext::setCommandSender(std::shared_ptr<INetworkCommandSender> commandSender) {
    _commandSender = commandSender;
}

void UIContext::registerDataUpdateCallback(std::function<void()> callback) {
    std::string callbackId = generateCallbackId();
    _dataUpdateCallbacks[callbackId] = callback;
}

void UIContext::unregisterDataUpdateCallback(const std::string& callbackId) {
    _dataUpdateCallbacks.erase(callbackId);
}

void UIContext::notifyDataUpdate() {
    for (const auto& pair : _dataUpdateCallbacks) {
        try {
            pair.second();
        } catch (const std::exception& e) {
            logSecurityWarning("notifyDataUpdate", "Callback exception: " + std::string(e.what()));
        }
    }
}

std::string UIContext::generateCallbackId() {
    std::stringstream ss;
    ss << "callback_" << (++_nextCallbackId);
    return ss.str();
}

bool UIContext::isValidWindowId(const std::string& windowId) const {
    static const std::vector<std::string> validWindowIds = {
        "tileInfo", "playerInfo", "timeInfo", "menu", "mapInfo"
    };
    return std::find(validWindowIds.begin(), validWindowIds.end(), windowId) != validWindowIds.end();
}

void UIContext::logSecurityWarning(const std::string& operation, const std::string& details) const {
    std::cerr << "[UIContext Security Warning] " << operation << ": " << details << std::endl;
}

} // namespace GUI
