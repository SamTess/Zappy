/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TileContentMessageHandler
*/

#ifndef TILE_CONTENT_MESSAGE_HANDLER_HPP_
#define TILE_CONTENT_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../networkCommandManager/NetworkCommandManager.hpp"

class TileContentMessageHandler : public IMessageHandler {
public:
    TileContentMessageHandler(std::shared_ptr<GameState> gameState,
        std::shared_ptr<NetworkCommandManager> networkManager);
    ~TileContentMessageHandler() = default;

    void handleMessage(std::shared_ptr<IMessageData> data) override;
    MessageType getMessageType() const override { return MessageType::TileContent; }

private:
    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<NetworkCommandManager> _networkManager;

    // Méthodes d'aide pour refactoriser handleMessage
    void handleUninitializedMap();
    void updateTileContent(std::shared_ptr<TileContentData> tileData);
};

#endif /* !TILE_CONTENT_MESSAGE_HANDLER_HPP_ */
