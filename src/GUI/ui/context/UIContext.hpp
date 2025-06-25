/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** UI Context Header
*/

#ifndef UICONTEXT_HPP_
    #define UICONTEXT_HPP_
    #include <map>
    #include <string>
    #include <memory>
    #include <functional>
    #include "IUIContext.hpp"
    #include "../data/IUIDataProvider.hpp"
    #include "../../shared/commands/INetworkCommandSender.hpp"

namespace GUI {

class UIWindowFactory;

class UIContext : public IUIContext {
public:
    UIContext(std::shared_ptr<UIWindowFactory> windowFactory,
              std::shared_ptr<INetworkCommandSender> commandSender = nullptr);
    virtual ~UIContext() = default;

    std::shared_ptr<IUIDataProvider> getDataProvider() const override;
    void setDataProvider(std::shared_ptr<IUIDataProvider> dataProvider);
    void requestWindowClose(const std::string& windowId) override;
    void requestWindowVisibilityChange(const std::string& windowId, bool visible) override;
    void notifyTileSelection(int x, int y) override;
    void notifyPlayerSelection(int playerId) override;
    void notifyViewModeChange(int viewMode) override;
    void executeNetworkCommand(const std::string& command) override;
    void setCommandSender(std::shared_ptr<INetworkCommandSender> commandSender);
    void registerDataUpdateCallback(std::function<void()> callback) override;
    void unregisterDataUpdateCallback(const std::string& callbackId) override;
    void notifyDataUpdate();

private:
    std::shared_ptr<UIWindowFactory> _windowFactory;
    std::shared_ptr<INetworkCommandSender> _commandSender;
    std::shared_ptr<IUIDataProvider> _dataProvider;
    std::map<std::string, std::function<void()>> _dataUpdateCallbacks;
    int _nextCallbackId;
    std::string generateCallbackId();
    bool isValidWindowId(const std::string& windowId) const;
    void logSecurityWarning(const std::string& operation, const std::string& details) const;
};

} // namespace GUI

#endif /* UICONTEXT_HPP_ */
