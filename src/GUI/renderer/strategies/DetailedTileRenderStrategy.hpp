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
#include "../../graphicalContext/GraphicalContext.hpp"

namespace Zappy {

class DetailedTileRenderStrategy : public ITileRenderStrategy {
private:
    const std::shared_ptr<GraphicalContext> context;
    void renderText3D(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        const std::string& text,
        ZappyTypes::Vector3 position,
        float fontSize,
        ZappyTypes::Color color);
    void renderResourceIndicator(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        ZappyTypes::Vector3 position,
        ResourceType resourceType,
        int quantity,
        float tileSize);
    void renderPlayerIndicator(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        ZappyTypes::Vector3 position,
        int playerId,
        float tileSize);
    void renderEggIndicator(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        ZappyTypes::Vector3 position,
        int eggId,
        float tileSize);

public:
    explicit DetailedTileRenderStrategy(const std::shared_ptr<GraphicalContext>& ctx);
    void renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        int x, int y,
        const ZappyTypes::Color& color,
        float tileSize,
        float spacing) override;
};

} // namespace Zappy
