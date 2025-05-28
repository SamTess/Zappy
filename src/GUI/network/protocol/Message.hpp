/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** message
*/

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <string>

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

    private:
        std::string _messageString;
        std::string _messageHeader;
        std::string _messageData;
};

#endif /* !MESSAGE_HPP_ */
