/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Map renderer observer
*/

#include <algorithm>
#include "MapRendererObserver.hpp"
#include "MapRenderer.hpp"

namespace Zappy {

MapRendererObserver::MapRendererObserver(std::shared_ptr<MapRenderer> renderer)
    : renderer(renderer) {}

void MapRendererObserver::onMapSizeChanged(int width, int height) {
    if (width > 20 || height > 20) {
        float newSize = 10.0f / std::max(width, height);
        renderer->setTileSize(newSize);
        renderer->setTileSpacing(newSize * 0.1f);
    }
}

void MapRendererObserver::onTileChanged(int /*x*/, int /*y*/, const TileData& /*tileData*/) {
    //TODO(Sam) : maj avec les données du network
}

} // namespace Zappy
