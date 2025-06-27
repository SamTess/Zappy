/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** IUIContext
*/

#pragma once

#include <memory>
#include <functional>
#include <string>
#include "../data/IUIDataProvider.hpp"

namespace GUI {

class IUIContext {
    public:
        virtual ~IUIContext() = default;

        virtual std::shared_ptr<IUIDataProvider> getDataProvider() const = 0;
        virtual void setDataProvider(std::shared_ptr<IUIDataProvider> dataProvider) = 0;
        virtual void requestWindowClose(const std::string& windowId) = 0;
        virtual void requestWindowVisibilityChange(const std::string& windowId, bool visible) = 0;
        virtual void notifyTileSelection(int x, int y) = 0;
        virtual void notifyPlayerSelection(int playerId) = 0;
        virtual void executeNetworkCommand(const std::string& command) = 0;
        virtual void registerDataUpdateCallback(std::function<void()> callback) = 0;
        virtual void unregisterDataUpdateCallback(const std::string& callbackId) = 0;
};

} // namespace GUI
