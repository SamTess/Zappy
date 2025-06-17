/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** PlayerInfoWindow implementation
*/

#pragma once

#include <memory>
#include "../AUIWindow.hpp"

namespace GUI {

class PlayerInfoWindow : public AUIWindow {
public:
    explicit PlayerInfoWindow(std::shared_ptr<IGuiLib> guiLib);
    ~PlayerInfoWindow() = default;
    /**
     * @brief Définit le joueur sélectionné
     * @param playerId Identifiant du joueur sélectionné
     */
    void setSelectedPlayer(int playerId);
    /**
     * @brief Récupère l'identifiant du joueur sélectionné
     * @return Identifiant du joueur sélectionné, -1 si aucun joueur n'est sélectionné
     */
    int getSelectedPlayer() const;
    /**
     * @brief Vérifie si un joueur est sélectionné
     * @return true si un joueur est sélectionné, false sinon
     */
    bool hasPlayerSelected() const;

protected:
    void renderContent() override;
    void updateSpecificData(const GameData& gameData) override;

private:
    int m_selectedPlayerId;
    bool m_hasSelectedPlayer;
};

} // namespace GUI
