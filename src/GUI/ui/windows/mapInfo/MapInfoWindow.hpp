/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MapInfoWindow
*/

#pragma once

#include <deque>
#include <string>
#include <memory>
#include <vector>
#include "../AUIWindow.hpp"
#include "../../../shared/commands/INetworkCommandSender.hpp"

namespace GUI {

class MapInfoWindow : public AUIWindow {
    public:
        explicit MapInfoWindow(std::shared_ptr<IGuiLib> guiLib);
        ~MapInfoWindow() = default;
        void initialize(const ZappyTypes::Vector2& position,
            const ZappyTypes::Vector2& dimensions) override;
        void setCommandSender(std::shared_ptr<INetworkCommandSender> sender);

    private:
        void renderContent() override;
        int calculateVisibleItemCount(float panelHeight, float lineHeight);
        void renderMapInfo(const ZappyTypes::Rectangle& view,
            int startIndex, float lineHeight, float contentWidth);
        void renderMapInfoTeam(const ZappyTypes::Rectangle& view,
            int startIndex, float lineHeight, float contentWidth, int textY);
        std::string getFormattedResource(std::vector<int> totalRessource);
        std::shared_ptr<INetworkCommandSender> _commandSender;
};
} // namespace GUI
