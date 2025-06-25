/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Abstract class for UI Windows
*/

#pragma once

#include <memory>
#include <string>
#include "IUIWindow.hpp"

namespace GUI {

class AUIWindow : public IUIWindow {
    public:
        AUIWindow(std::shared_ptr<IGuiLib> guiLib, const std::string& title);
        virtual ~AUIWindow() = default;

        void initialize(const ZappyTypes::Vector2& position,
            const ZappyTypes::Vector2& dimensions) override;
        bool render() override;
        void updateData(std::shared_ptr<const GameState> gameState) override;
        void setPosition(const ZappyTypes::Vector2& position) override;
        ZappyTypes::Vector2 getPosition() const override;
        ZappyTypes::Vector2 getDimensions() const override;
        bool isVisible() const override;
        void setVisible(bool visible) override;
        void toggleVisibility() override;
        bool startDragging(const ZappyTypes::Vector2& mousePosition) override;
        void updateDragging(const ZappyTypes::Vector2& mousePosition) override;
        void stopDragging() override;
        bool isDragging() const override;
        bool isPositionInWindow(const ZappyTypes::Vector2& position) const override;
        void setShowWindowBox(bool show);
        bool getShowWindowBox() const;

    protected:
        virtual void renderContent() = 0;
        std::shared_ptr<IGuiLib> _guiLib;
        std::string _title;
        ZappyTypes::Vector2 _position;
        ZappyTypes::Vector2 _dimensions;

        bool _visible;
        bool _showWindowBox;
        bool _dragging;
        ZappyTypes::Vector2 _dragOffset;
        std::shared_ptr<const GameState> _gameState;
};

} // namespace GUI
