/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Tile Renderer factory
*/

#pragma once
#include <memory>
#include "../../textureManager/ModelManager.hpp"
#include "../../gameController/GameState.hpp"
#include "SimpleTileRenderStrategy.hpp"
#include "ModelTileRenderStrategy.hpp"
#include "DetailedTileRenderStrategy.hpp"
#include "../../textureManager/ModelManagerAdapter.hpp"


namespace Zappy {
class TileRenderStrategyFactory {
private:
    std::shared_ptr<ModelManagerAdapter> modelManager;
public:
    explicit TileRenderStrategyFactory(const std::shared_ptr<ModelManagerAdapter>& modelManager);
    std::shared_ptr<ITileRenderStrategy> createSimpleTileStrategy(const std::shared_ptr<const GameState>& gameState);
    std::shared_ptr<ITileRenderStrategy> createModelTileStrategy(int modelId, const std::shared_ptr<const GameState>& gameState);
    std::shared_ptr<ITileRenderStrategy> createDetailedTileStrategy(const std::shared_ptr<const GameState>& gameState);
};
} // namespace Zappy
