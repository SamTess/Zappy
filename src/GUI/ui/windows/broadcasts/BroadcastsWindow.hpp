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
    /**
     * @brief Calcule le nombre d'éléments visibles dans la fenêtre
     * @param panelHeight Hauteur du panneau
     * @param lineHeight Hauteur d'une ligne
     * @return Nombre d'éléments pouvant être affichés
     */
    int calculateVisibleItemCount(float panelHeight, float lineHeight);

    /**
     * @brief Calcule l'index de départ pour l'affichage des messages
     * @param maxVisibleItems Nombre maximum d'éléments visibles
     * @return Index de départ
     */
    int calculateStartIndex(int maxVisibleItems);

    /**
     * @brief Affiche les messages de broadcast
     * @param view Rectangle de la zone d'affichage
     * @param startIndex Index du premier message à afficher
     * @param lineHeight Hauteur d'une ligne
     * @param contentWidth Largeur du contenu
     */
    void renderBroadcastMessages(const ZappyTypes::Rectangle& view,
                                int startIndex, float lineHeight,
                                float contentWidth);

    // Structure pour stocker les messages de broadcast
    struct Broadcast {
        std::string team;
        std::string message;
        float timeLeft; // Temps restant avant disparition
    };

    /**
     * @brief Formate un message de broadcast pour l'affichage
     * @param broadcast Message de broadcast
     * @return Message formaté
     */
    std::string formatBroadcastMessage(const Broadcast& broadcast);

    /**
     * @brief Met à jour les messages de broadcast (temps restant)
     */
    void updateBroadcastMessages();
    std::deque<Broadcast> m_broadcasts;
    size_t m_maxBroadcasts;
};

} // namespace GUI
