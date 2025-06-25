/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** TileInfoWindow implementation
*/

#pragma once

#include <memory>
#include <utility>
#include "../AUIWindow.hpp"

namespace GUI {

class TileInfoWindow : public AUIWindow {
public:
    explicit TileInfoWindow(std::shared_ptr<IGuiLib> guiLib);
    ~TileInfoWindow() = default;

    void setSelectedTile(int x, int y);
    std::pair<int, int> getSelectedTile() const;
    bool hasTileSelected() const;

protected:
    void renderContent() override;

private:
    void renderNoTileSelected();
    void renderTilePosition();
    float renderResourceInfo();
    void renderPlayerCount(float yOffset);

    struct {
        int x;
        int y;
        bool selected;
    } m_selectedTile;
};

} // namespace GUI
