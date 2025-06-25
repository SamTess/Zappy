/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Tile Renderer factory
*/


#include <memory>
#include "TileRenderStrategyFactory.hpp"
#include "SimpleTileRenderStrategy.hpp"
#include "ModelTileRenderStrategy.hpp"
#include "DetailedTileRenderStrategy.hpp"

namespace Zappy {

TileRenderStrategyFactory::TileRenderStrategyFactory(const std::shared_ptr<ModelManagerAdapter>& manager)
    : modelManager(manager) {}

std::shared_ptr<ITileRenderStrategy> TileRenderStrategyFactory::createSimpleTileStrategy(const std::shared_ptr<const GameState>& gameState) {
    return std::make_shared<SimpleTileRenderStrategy>(gameState);
}

std::shared_ptr<ITileRenderStrategy> TileRenderStrategyFactory::createModelTileStrategy(int modelId, const std::shared_ptr<const GameState>& gameState) {
    auto managerPtr = std::shared_ptr<ModelManager>(&modelManager->getManager(), [](ModelManager*){});
    return std::make_shared<ModelTileRenderStrategy>(managerPtr, modelId, gameState);
}

std::shared_ptr<ITileRenderStrategy> TileRenderStrategyFactory::createDetailedTileStrategy(
    const std::shared_ptr<const GameState>& gameState) {
    return std::make_shared<DetailedTileRenderStrategy>(gameState);
}

} // namespace Zappy
