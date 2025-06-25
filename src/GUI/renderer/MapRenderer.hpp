/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Map renderer
*/
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "../gameController/GameState.hpp"
#include "../../Shared/IGraphicsLib.hpp"
#include "../textureManager/ModelManager.hpp"
#include "../textureManager/ModelManagerAdapter.hpp"
#include "strategies/DetailedTileRenderStrategy.hpp"
#include "strategies/ModelTileRenderStrategy.hpp"
#include "strategies/TileRenderStrategyFactory.hpp"
#include "strategies/ITileRenderStrategy.hpp"

namespace Zappy {

/**
 * @brief Renderer de carte responsable de l'affichage de la grille
 * Implémente le pattern Bridge pour séparer l'interface du rendu
 */
class MapRenderer {
private:
    std::shared_ptr<IGraphicsLib> graphicsLib;
    std::shared_ptr<const GameState> gameState;
    std::shared_ptr<ITileRenderStrategy> tileRenderStrategy;
    std::shared_ptr<ITileRenderStrategy> detailedTileStrategy;  // Stratégie détaillée pour le zoom proche
    TileRenderStrategyFactory strategyFactory;

    // Paramètres de rendu de la carte
    float tileSize;
    float tileSpacing;
    float zoomLevel;
    float detailThreshold;
    std::unordered_map<int, ZappyTypes::Color> resourceColors;

    // Map de stratégies de rendu par type de ressource
    std::unordered_map<int, std::shared_ptr<ITileRenderStrategy>> resourceRenderStrategies;

public:
    MapRenderer(const std::shared_ptr<IGraphicsLib>& graphicsLib,
               const std::shared_ptr<const GameState>& gameState,
               const std::shared_ptr<ModelManagerAdapter>& modelManagerAdapter);
    /**
     * @brief Initialise le renderer de carte
     */
    void initialize();
    /**
     * @brief Rend la carte complète
     */
    void render();

    /**
     * @brief Rend la carte avec les effets de sélection
     * @param selectedTileX Coordonnée X de la tuile sélectionnée (-1 si aucune)
     * @param selectedTileY Coordonnée Y de la tuile sélectionnée (-1 si aucune)
     * @param selectedPlayerId ID du joueur sélectionné (-1 si aucun)
     */
    void renderWithSelection(int selectedTileX, int selectedTileY, int selectedPlayerId);
    /**
     * @brief Définit la stratégie de rendu pour toutes les tuiles
     * @param strategy La stratégie à utiliser
     */
    void setTileRenderStrategy(std::shared_ptr<ITileRenderStrategy> strategy);
    /**
     * @brief Définit une stratégie de rendu spécifique pour un type de ressource
     * @param resourceType Le type de ressource
     * @param strategy La stratégie à utiliser
     */
    void setResourceRenderStrategy(int resourceType, std::shared_ptr<ITileRenderStrategy> strategy);
    /**
     * @brief Modifie la taille des tuiles
     * @param size La nouvelle taille
     */
    void setTileSize(float size);
    /**
     * @brief Modifie l'espacement entre les tuiles
     * @param spacing Le nouvel espacement
     */
    void setTileSpacing(float spacing);
    /**
     * @brief Définit la couleur pour un type de ressource spécifique
     * @param resourceType Le type de ressource
     * @param color La couleur à utiliser
     */
    void setResourceColor(int resourceType, const ZappyTypes::Color& color);

    /**
     * @brief Définit le niveau de zoom actuel
     * @param zoom Le niveau de zoom (1.0 = normal)
     */
    void setZoomLevel(float zoom);

    /**
     * @brief Définit le seuil de zoom pour afficher les détails
     * @param threshold Le seuil (typiquement entre 1.5 et 3.0)
     */
    void setDetailThreshold(float threshold);

private:
    /**
     * @brief Dessine une tuile individuelle
     * @param x Position X de la tuile
     * @param y Position Y de la tuile
     * @param resourceType Type de ressource principal sur la tuile
     */
    void renderTile(int x, int y, int resourceType);

    /**
     * @brief Calcule la couleur d'une tuile en fonction de ses ressources
     * @param x Position X de la tuile
     * @param y Position Y de la tuile
     * @return La couleur calculée
     */
    ZappyTypes::Color calculateTileColor(int x, int y);

    /**
     * @brief Rend l'effet de sélection pour une tuile
     * @param x Position X de la tuile
     * @param y Position Y de la tuile
     */
    void renderTileSelectionEffect(int x, int y);

    /**
     * @brief Rend l'effet de sélection pour un joueur
     * @param playerId ID du joueur sélectionné
     */
    void renderPlayerSelectionEffect(int playerId);

    /**
     * @brief Calcule la position de base d'une tuile
     * @param x Position X de la tuile
     * @param y Position Y de la tuile
     * @return La position de base calculée
     */
    ZappyTypes::Vector3 calculateBasePosition(int x, int y);

    /**
     * @brief Calcule la position visuelle d'un joueur
     * @param playerId ID du joueur
     * @param x Position X de la tuile
     * @param y Position Y de la tuile
     * @param basePosition Position de base de la tuile
     * @return La position visuelle calculée
     */
    ZappyTypes::Vector3 calculatePlayerVisualPosition(int playerId, int x, int y, const ZappyTypes::Vector3& basePosition);

    /**
     * @brief Rend le wireframe de sélection du joueur
     * @param position Position du joueur
     * @param time Temps actuel pour l'animation
     * @param pulseIntensity Intensité de l'effet de pulsation
     */
    void renderPlayerWireframe(const ZappyTypes::Vector3& position, float time, float pulseIntensity);

    /**
     * @brief Rend le cylindre de sélection autour du joueur
     * @param position Position du joueur
     * @param pulseIntensity Intensité de l'effet de pulsation
     */
    void renderSelectionCylinder(const ZappyTypes::Vector3& position, float pulseIntensity);

    /**
     * @brief Rend l'anneau de sélection autour de la tuile
     * @param basePosition Position de base de la tuile
     * @param time Temps actuel pour l'animation
     * @param pulseIntensity Intensité de l'effet de pulsation
     */
    void renderSelectionRing(const ZappyTypes::Vector3& basePosition, float time, float pulseIntensity);

    /**
     * @brief Rend les particules de sélection autour du joueur
     * @param position Position du joueur
     * @param time Temps actuel pour l'animation
     */
    void renderSelectionParticles(const ZappyTypes::Vector3& position, float time);

    /**
     * @brief Rend l'écran de victoire si le jeu est terminé
     */
    void renderVictoryScreen();
};

} // namespace Zappy
