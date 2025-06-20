/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** PlayerInfoWindow implementation
*/

#pragma once

#include <memory>
#include <string>
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
    void updateSpecificData(std::shared_ptr<const GameState> gameState,
                          int mapWidth, int mapHeight,
                          float gameTime, int frequency, int gameTick) override;

private:
    /**
     * @brief Affiche le message lorsqu'aucun joueur n'est sélectionné
     */
    void displayNoPlayerSelected();

    /**
     * @brief Recherche le joueur sélectionné dans l'état de jeu
     * @return Pointeur vers le joueur sélectionné ou nullptr si non trouvé
     */
    std::shared_ptr<const IPlayer> findSelectedPlayer();

    /**
     * @brief Affiche le message lorsque le joueur sélectionné n'est pas trouvé
     */
    void displayPlayerNotFound();

    /**
     * @brief Affiche les informations d'identification du joueur (ID et équipe)
     * @param selectedPlayer Pointeur vers le joueur sélectionné
     * @param yOffset Position Y pour l'affichage
     * @return Nouvelle position Y après l'affichage
     */
    float displayPlayerIdentity(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);

    /**
     * @brief Affiche les informations de position et d'orientation du joueur
     * @param selectedPlayer Pointeur vers le joueur sélectionné
     * @param yOffset Position Y pour l'affichage
     * @return Nouvelle position Y après l'affichage
     */
    float displayPositionInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);

    /**
     * @brief Affiche le niveau du joueur
     * @param selectedPlayer Pointeur vers le joueur sélectionné
     * @param yOffset Position Y pour l'affichage
     * @return Nouvelle position Y après l'affichage
     */
    float displayLevelInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);

    /**
     * @brief Affiche l'inventaire du joueur
     * @param selectedPlayer Pointeur vers le joueur sélectionné
     * @param yOffset Position Y pour l'affichage
     * @return Nouvelle position Y après l'affichage
     */
    float displayInventory(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);

    /**
     * @brief Obtient la représentation textuelle de l'orientation du joueur
     * @param orientation Valeur numérique de l'orientation
     * @return Chaîne de caractères représentant l'orientation
     */
    std::string getOrientationString(int orientation);

    /**
     * @brief Vérifie si le joueur sélectionné existe dans l'état de jeu
     * @param gameState L'état de jeu actuel
     * @return true si le joueur existe, false sinon
     */
    bool playerExistsInGameState(std::shared_ptr<const GameState> gameState);

    int m_selectedPlayerId;
    bool m_hasSelectedPlayer;
};

} // namespace GUI
