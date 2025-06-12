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
#include "../graphicalContext/GraphicalContext.hpp"
#include "../../Shared/IGraphicsLib.hpp"
#include "../textureManager/ModelManager.hpp"
#include "../textureManager/ModelManagerAdapter.hpp"
#include "strategies/DetailedTileRenderStrategy.hpp"
#include "strategies/ModelTileRenderStrategy.hpp"
#include "strategies/TileRenderStrategyFactory.hpp"
#include "MapRendererObserver.hpp"
#include "strategies/ITileRenderStrategy.hpp"

namespace Zappy {

/**
 * @brief Renderer de carte responsable de l'affichage de la grille
 * Implémente le pattern Bridge pour séparer l'interface du rendu
 */
class MapRenderer {
private:
    std::shared_ptr<IGraphicsLib> graphicsLib;
    std::shared_ptr<GraphicalContext> context;
    std::shared_ptr<ITileRenderStrategy> tileRenderStrategy;
    std::shared_ptr<ITileRenderStrategy> detailedTileStrategy;  // Stratégie détaillée pour le zoom proche
    TileRenderStrategyFactory strategyFactory;

    // Paramètres de rendu de la carte
    float tileSize;
    float tileSpacing;
    float zoomLevel;           // Niveau de zoom actuel
    float detailThreshold;     // Seuil de zoom pour afficher les détails
    std::unordered_map<int, ZappyTypes::Color> resourceColors;

    // Map de stratégies de rendu par type de ressource
    std::unordered_map<int, std::shared_ptr<ITileRenderStrategy>> resourceRenderStrategies;

public:
    MapRenderer(const std::shared_ptr<IGraphicsLib>& graphicsLib,
               const std::shared_ptr<GraphicalContext>& context,
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
};

} // namespace Zappy
