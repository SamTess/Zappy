/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MapRenderer
*/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "../gameController/GameState.hpp"
#include "../../GUI/shared/IGraphicsLib.hpp"
#include "TileRender.hpp"

namespace Zappy {

class MapRenderer {
    public:
        MapRenderer(const std::shared_ptr<IGraphicsLib>& graphicsLib,
            const std::shared_ptr<const GameState>& gameState);
        void initialize();
        void render();
        void renderWithSelection(int selectedTileX, int selectedTileY, int selectedPlayerId);
        void setTileSize(float size);
        void setTileSpacing(float spacing);
        float getTileSize() const;
        float getTileSpacing() const;
        void setResourceColor(int resourceType, const ZappyTypes::Color& color);

    private:
        void renderTile(int x, int y);
        void renderTileSelectionEffect(int x, int y);
        void renderPlayerSelectionEffect(int playerId);
        ZappyTypes::Vector3 calculateBasePosition(int x, int y);
        ZappyTypes::Vector3 calculatePlayerVisualPosition(int playerId, int x, int y, const ZappyTypes::Vector3& basePosition);
        void renderPlayerWireframe(const ZappyTypes::Vector3& position, float time, float pulseIntensity);
        void renderSelectionCylinder(const ZappyTypes::Vector3& position, float pulseIntensity);
        void renderSelectionRing(const ZappyTypes::Vector3& basePosition, float time, float pulseIntensity);
        void renderSelectionParticles(const ZappyTypes::Vector3& position, float time);
        void renderVictoryScreen();

        std::shared_ptr<IGraphicsLib> graphicsLib;
        std::shared_ptr<const GameState> gameState;
        std::shared_ptr<DetailedTileRenderStrategy> tileRenderStrategy;
        float tileSize;
        float tileSpacing;
        std::unordered_map<int, ZappyTypes::Color> resourceColors;
};

} // namespace Zappy
