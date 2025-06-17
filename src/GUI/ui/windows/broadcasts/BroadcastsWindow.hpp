/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** BroadcastsWindow implementation
*/

#pragma once

#include <deque>
#include <string>
#include <memory>
#include "../AUIWindow.hpp"

namespace GUI {

class BroadcastsWindow : public AUIWindow {
public:
    explicit BroadcastsWindow(std::shared_ptr<IGuiLib> guiLib);
    ~BroadcastsWindow() = default;

    /**
     * @brief Ajoute un message de broadcast
     * @param team Équipe émettrice
     * @param message Contenu du message
     */
    void addBroadcast(const std::string& team, const std::string& message);

    /**
     * @brief Efface tous les messages de broadcast
     */
    void clearBroadcasts();

protected:
    void renderContent() override;
    void updateSpecificData(const GameData& gameData) override;

private:
    // Structure pour stocker les messages de broadcast
    struct Broadcast {
        std::string team;
        std::string message;
        float timeLeft; // Temps restant avant disparition
    };
    std::deque<Broadcast> m_broadcasts;
    size_t m_maxBroadcasts;
};

} // namespace GUI
