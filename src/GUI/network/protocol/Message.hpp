/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** message
*/

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <string>
#include <vector>
#include "HeaderMessage.hpp"

class Message {
    public:
        Message();
        Message(std::string header, std::string data);
        ~Message() = default;

        void setMessage(const std::string &data);
        void setHeader(const std::string &header);
        void setData(const std::string &data);

        const std::string &getMessage() const;
        const std::string &getHeader() const;
        const std::string &getData() const;

        HeaderMessage::MessageType getType() const;
        void setType(HeaderMessage::MessageType type);

        const std::vector<std::string> &getParameters() const;
        void setParameters(const std::vector<std::string> &params);
        int getIntParam(size_t index) const;

    private:
        std::string _messageString;
        std::string _messageHeader;
        std::string _messageData;
        HeaderMessage::MessageType _messageType;
        std::vector<std::string> _parameters;
};

#endif /* !MESSAGE_HPP_ */
