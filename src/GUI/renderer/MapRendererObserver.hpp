/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Map renderer observer
*/

#pragma once
#include <memory>
#include "../graphicalContext/GraphicalContext.hpp"

namespace Zappy {
class MapRenderer;
class MapRendererObserver : public IGraphicalContextObserver {
private:
    std::shared_ptr<MapRenderer> renderer;
public:
    explicit MapRendererObserver(std::shared_ptr<MapRenderer> renderer);
    void onMapSizeChanged(int width, int height) override;
    void onTileChanged(int x, int y, const TileData& tileData) override;
};
} // namespace Zappy
