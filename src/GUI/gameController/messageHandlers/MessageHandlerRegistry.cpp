/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MessageHandlerRegistry
*/

#include "MessageHandlerRegistry.hpp"
#include <iostream>

MessageHandlerRegistry::MessageHandlerRegistry() {}

void MessageHandlerRegistry::registerHandler(std::shared_ptr<IMessageHandler> handler) {
    if (!handler) {
        std::cerr << "[MessageHandlerRegistry] Cannot register null handler" << std::endl;
        return;
    }
    MessageType messageType = handler->getMessageType();
    _handlers[messageType] = handler;
}

void MessageHandlerRegistry::processMessage(const Message& message) {
    if (!message.getStructuredData())
        return;
    MessageType messageType = message.getStructuredData()->getType();
    auto it = _handlers.find(messageType);
    if (it != _handlers.end())
        it->second->handleMessage(message.getStructuredData());
    else
        std::cerr << "[MessageHandlerRegistry] No handler for message type: " << static_cast<int>(messageType) << std::endl;
}

bool MessageHandlerRegistry::hasHandler(MessageType messageType) const {
    auto it = _handlers.find(messageType);

    if (it != _handlers.end())
        return true;
    return false;
}
