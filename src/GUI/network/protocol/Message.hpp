/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** message
*/

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <string>
#include <memory>
#include <vector>
#include "messageData/MessageDataAll.hpp"
#include "HeaderMessage.hpp"

class Message {
public:
    /**
     * @brief Construit un message vide.
     */
    Message();
    /**
     * @brief Construit un message à partir d'un header, d'une data brute et d'une structure typée optionnelle.
     * @param header Le header du message (ex: "pnw")
     * @param data La data brute (ex: "1 2 3 4 5 Team")
     * @param structuredData Structure typée dérivée de IMessageData (optionnelle)
     */
    Message(const std::string& header, const std::string& data, std::shared_ptr<IMessageData> structuredData = nullptr);
    ~Message() = default;

    /**
     * @brief Définit le message complet (header + data). Utilisé rarement.
     */
    void setMessage(const std::string &data);
    /**
     * @brief Définit le header (ex: "pnw").
     */
    void setHeader(const std::string &header);
    /**
     * @brief Définit la data brute (ex: "1 2 3 4 5 Team").
     */
    void setData(const std::string &data);
    /**
     * @brief Définit la structure typée (IMessageData).
     */
    void setData(std::shared_ptr<IMessageData> data);

    /**
     * @brief Retourne le message complet (header + data + \n).
     */
    const std::string &getMessage() const;
    /**
     * @brief Retourne le header (ex: "pnw").
     */
    const std::string &getHeader() const;
    /**
     * @brief Retourne la data brute (ex: "1 2 3 4 5 Team\n").
     */
    const std::string &getData() const;

    /**
     * @brief Indique si une structure typée est présente.
     */
    bool hasStructuredData() const;
    /**
     * @brief Retourne la structure typée (IMessageData) si présente.
     */
    std::shared_ptr<IMessageData> getStructuredData() const { return _structuredData; }

    // Pour usage avancé (non utilisé par ProtocolParser)
    void addIntParam(int value);
    void addStringParam(const std::string &value);

private:
    std::string _messageString;      // message complet (header + data + \n)
    std::string _messageHeader;      // header seul
    std::string _messageData;        // data seule (avec \n)
    std::shared_ptr<IMessageData> _structuredData; // structure typée optionnelle
};

#endif /* !MESSAGE_HPP_ */
