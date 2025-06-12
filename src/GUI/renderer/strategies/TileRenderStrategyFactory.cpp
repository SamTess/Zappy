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

TileRenderStrategyFactory::TileRenderStrategyFactory(const std::shared_ptr<ModelManager>& manager)
    : modelManager(manager) {}

TileRenderStrategyFactory::TileRenderStrategyFactory(ModelManager* manager)
    : modelManager(std::shared_ptr<ModelManager>(manager, [](ModelManager*) {})) {}

std::shared_ptr<ITileRenderStrategy> TileRenderStrategyFactory::createSimpleTileStrategy(const std::shared_ptr<GraphicalContext>& ctx) {
    return std::make_shared<SimpleTileRenderStrategy>(ctx);
}

std::shared_ptr<ITileRenderStrategy> TileRenderStrategyFactory::createModelTileStrategy(int modelId, const std::shared_ptr<GraphicalContext>& ctx) {
    return std::make_shared<ModelTileRenderStrategy>(modelManager, modelId, ctx);
}

std::shared_ptr<ITileRenderStrategy> TileRenderStrategyFactory::createDetailedTileStrategy(
    const std::shared_ptr<GraphicalContext>& ctx) {
    return std::make_shared<DetailedTileRenderStrategy>(ctx);
}

} // namespace Zappy
