/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Simple tile Renderer
*/

#pragma once
#include <memory>
#include "ITileRenderStrategy.hpp"
#include "../../gameController/GameState.hpp"

namespace Zappy {

class SimpleTileRenderStrategy : public ITileRenderStrategy {
private:
    const std::shared_ptr<const GameState> gameState;
public:
    explicit SimpleTileRenderStrategy(const std::shared_ptr<const GameState>& gameState);
    void renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        int x, int y,
        const ZappyTypes::Color& color,
        float tileSize,
        float spacing) override;
};

} // namespace Zappy
