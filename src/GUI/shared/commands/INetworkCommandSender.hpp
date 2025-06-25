/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Network Command Sender Interface implementation
*/

#ifndef INETWORK_COMMAND_SENDER_HPP_
    #define INETWORK_COMMAND_SENDER_HPP_
    #include <string>

class INetworkCommandSender {
public:
    virtual ~INetworkCommandSender() = default;
    virtual void sendCommand(const std::string& command) = 0;
    virtual bool isConnected() const = 0;
};

#endif /* !INETWORK_COMMAND_SENDER_HPP_ */
