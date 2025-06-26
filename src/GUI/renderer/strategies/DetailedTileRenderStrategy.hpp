/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Detailed tile Renderer
*/

#pragma once
#include <memory>
#include <string>
#include "ITileRenderStrategy.hpp"
#include "../../gameController/GameState.hpp"

namespace Zappy {

class DetailedTileRenderStrategy : public ITileRenderStrategy {
    public:
        explicit DetailedTileRenderStrategy(const std::shared_ptr<const GameState>& gameState);
        void renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
            int x, int y, float tileSize, float spacing) override;

    private:
        void renderAllResources(std::shared_ptr<const ITile> tile,
            const std::shared_ptr<IGraphicsLib>& graphicsLib, const ZappyTypes::Vector3& position, float tileSize);
        void renderAllPlayers(std::shared_ptr<const ITile> tile,
            const std::shared_ptr<IGraphicsLib>& graphicsLib, const ZappyTypes::Vector3& position, float tileSize);
        void renderAllEggs(std::shared_ptr<const ITile> tile,
            const std::shared_ptr<IGraphicsLib>& graphicsLib, const ZappyTypes::Vector3& position, float tileSize);
        const std::shared_ptr<const GameState> gameState;
};

} // namespace Zappy
