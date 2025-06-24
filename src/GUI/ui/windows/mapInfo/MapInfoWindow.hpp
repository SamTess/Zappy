/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MapInfoWindow implementation
*/

#pragma once

#include <deque>
#include <string>
#include <memory>
#include "../AUIWindow.hpp"
#include "../../../network/networkManager/NetworkManager.hpp"

namespace GUI {

class MapInfoWindow : public AUIWindow {
public:
    explicit MapInfoWindow(std::shared_ptr<IGuiLib> guiLib);
    ~MapInfoWindow() = default;



    /**
     * @brief Efface tous les messages de broadcast
     */
    void clearBroadcasts();

    void setNetworkManager(std::shared_ptr<NetworkManager> networkManager);
    void initialize(const ZappyTypes::Vector2& position,
                    const ZappyTypes::Vector2& dimensions) override;
protected:
    void renderContent() override;

private:
    /**
     * @brief Calcule le nombre d'éléments visibles dans la fenêtre
     * @param panelHeight Hauteur du panneau
     * @param lineHeight Hauteur d'une ligne
     * @return Nombre d'éléments pouvant être affichés
     */
    int calculateVisibleItemCount(float panelHeight, float lineHeight);

    /**
     * @brief Affiche les messages de broadcast
     * @param view Rectangle de la zone d'affichage
     * @param startIndex Index du premier message à afficher
     * @param lineHeight Hauteur d'une ligne
     * @param contentWidth Largeur du contenu
     */
    void renderMapInfo(const ZappyTypes::Rectangle& view,
                                int startIndex, float lineHeight,
                                float contentWidth);

    void renderMapInfoTeam(const ZappyTypes::Rectangle& view,
                                int startIndex, float lineHeight,
                                float contentWidth, int textY);
    std::string getFormattedResource(std::vector<int> totalRessource);
    // Structure pour stocker les messages de broadcast
    struct Broadcast {
        std::string team;
        std::string message;
        float timeLeft; // Temps restant avant disparition
    };


    std::shared_ptr<NetworkManager> m_networkManager;


};
} // namespace GUI
