/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Model tile Renderer
*/

#pragma once
#include <memory>
#include "ITileRenderStrategy.hpp"
#include "../../graphicalContext/GraphicalContext.hpp"
#include "../../textureManager/ModelManager.hpp"

namespace Zappy {

class ModelTileRenderStrategy : public ITileRenderStrategy {
private:
    std::shared_ptr<ModelManager> modelManager;
    int modelId;
    const std::shared_ptr<GraphicalContext> context;
public:
    ModelTileRenderStrategy(const std::shared_ptr<ModelManager>& manager, int modelId, const std::shared_ptr<GraphicalContext>& ctx);
    void renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        int x, int y,
        const ZappyTypes::Color& color,
        float tileSize,
        float spacing) override;
};

} // namespace Zappy
