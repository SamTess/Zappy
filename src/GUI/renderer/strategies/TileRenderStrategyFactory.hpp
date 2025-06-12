/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Tile Renderer factory
*/

#pragma once
#include <memory>
#include "../../textureManager/ModelManager.hpp"
#include "../../graphicalContext/GraphicalContext.hpp"
#include "SimpleTileRenderStrategy.hpp"
#include "ModelTileRenderStrategy.hpp"
#include "DetailedTileRenderStrategy.hpp"

namespace Zappy {
class TileRenderStrategyFactory {
private:
    std::shared_ptr<ModelManager> modelManager;
public:
    explicit TileRenderStrategyFactory(const std::shared_ptr<ModelManager>& modelManager);
    explicit TileRenderStrategyFactory(ModelManager* modelManager);
    std::shared_ptr<ITileRenderStrategy> createSimpleTileStrategy(const std::shared_ptr<GraphicalContext>& ctx);
    std::shared_ptr<ITileRenderStrategy> createModelTileStrategy(int modelId, const std::shared_ptr<GraphicalContext>& ctx);
    std::shared_ptr<ITileRenderStrategy> createDetailedTileStrategy(const std::shared_ptr<GraphicalContext>& ctx);
};
} // namespace Zappy
