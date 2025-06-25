/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MapInfoWindow implementation
*/

#pragma once

#include <deque>
#include <string>
#include <memory>
#include "../AUIWindow.hpp"
#include "../../../network/networkManager/NetworkManager.hpp"

namespace GUI {

class MapInfoWindow : public AUIWindow {
    public:
        explicit MapInfoWindow(std::shared_ptr<IGuiLib> guiLib);
        ~MapInfoWindow() = default;
        void setNetworkManager(std::shared_ptr<NetworkManager> networkManager);
        void initialize(const ZappyTypes::Vector2& position,
            const ZappyTypes::Vector2& dimensions) override;

    private:
        void renderContent() override;
        int calculateVisibleItemCount(float panelHeight, float lineHeight);
        void renderMapInfo(const ZappyTypes::Rectangle& view,
            int startIndex, float lineHeight, float contentWidth);
        void renderMapInfoTeam(const ZappyTypes::Rectangle& view,
            int startIndex, float lineHeight, float contentWidth, int textY);
        std::string getFormattedResource(std::vector<int> totalRessource);
        std::shared_ptr<NetworkManager> _networkManager;
};
} // namespace GUI
