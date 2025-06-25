/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Interface for UI Windows
*/

#pragma once

#include <memory>
#include "../../../Shared/IGuiLib.hpp"
#include "../data/IUIDataProvider.hpp"

namespace GUI {

class IUIWindow {
public:
    virtual ~IUIWindow() = default;
    virtual void initialize(const ZappyTypes::Vector2& position,
                           const ZappyTypes::Vector2& dimensions) = 0;

    virtual bool render() = 0;
    virtual void updateData(std::shared_ptr<IUIDataProvider> dataProvider) = 0;
    virtual void setPosition(const ZappyTypes::Vector2& position) = 0;
    virtual ZappyTypes::Vector2 getPosition() const = 0;
    virtual ZappyTypes::Vector2 getDimensions() const = 0;
    virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;
    virtual void toggleVisibility() = 0;
    virtual bool startDragging(const ZappyTypes::Vector2& mousePosition) = 0;
    virtual void updateDragging(const ZappyTypes::Vector2& mousePosition) = 0;
    virtual void stopDragging() = 0;
    virtual bool isDragging() const = 0;
    virtual bool isPositionInWindow(const ZappyTypes::Vector2& position) const = 0;
};

} // namespace GUI
