/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IMessageHandler
*/

#ifndef IMESSAGE_HANDLER_HPP_
#define IMESSAGE_HANDLER_HPP_

#include <memory>
#include "../../shared/Message.hpp"

class IMessageHandler {
public:
    virtual ~IMessageHandler() = default;
    virtual void handleMessage(std::shared_ptr<IMessageData> data) = 0;
    virtual MessageType getMessageType() const = 0;
};

#endif /* !IMESSAGE_HANDLER_HPP_ */
