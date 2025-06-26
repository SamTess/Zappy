/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MessageHandlerRegistry
*/

#ifndef MESSAGE_HANDLER_REGISTRY_HPP_
#define MESSAGE_HANDLER_REGISTRY_HPP_

#include <map>
#include <memory>
#include <functional>
#include "IMessageHandler.hpp"
#include "../../shared/Message.hpp"

class MessageHandlerRegistry {
    public:
        MessageHandlerRegistry();
        ~MessageHandlerRegistry() = default;

        void registerHandler(std::shared_ptr<IMessageHandler> handler);
        void processMessage(const Message& message);
        bool hasHandler(MessageType messageType) const;

    private:
        std::map<MessageType, std::shared_ptr<IMessageHandler>> _handlers;
};

#endif /* !MESSAGE_HANDLER_REGISTRY_HPP_ */
