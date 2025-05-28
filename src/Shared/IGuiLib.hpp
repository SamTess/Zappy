/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** GuiLib interface
*/
#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Common.hpp"

class IGuiLib {
    public:
        virtual ~IGuiLib() = default;
        virtual void DrawButton(float x, float y, float width, float height, const std::string& text) = 0;
        virtual bool ButtonPressed(float x, float y, float width, float height, const std::string& text) = 0;
        virtual void DrawLabel(float x, float y, float width, float height, const std::string& text) = 0;
        virtual bool DrawCheckBox(float x, float y, float width, float height, const std::string& text, bool checked) = 0;
        virtual bool DrawToggle(float x, float y, float width, float height, const std::string& text, bool active) = 0;
        virtual int DrawToggleGroup(float x, float y, float width, float height, const std::string& text, int active) = 0;
        virtual int DrawComboBox(float x, float y, float width, float height, const std::string& text, int active) = 0;
        virtual int DrawDropdownBox(float x, float y, float width, float height, const std::string& text, int active, bool editMode) = 0;
        virtual bool DrawTextBox(float x, float y, float width, float height, std::shared_ptr<std::string> text, int maxLength, bool editMode) = 0;
        virtual bool DrawValueBox(float x, float y, float width, float height, std::shared_ptr<int> value, int minValue, int maxValue, bool editMode) = 0;
        virtual bool DrawSpinner(float x, float y, float width, float height, std::shared_ptr<int> value, int minValue, int maxValue, bool editMode) = 0;
        virtual float DrawSlider(float x, float y, float width, float height, const std::string& textLeft, const std::string& textRight, float value, float minValue, float maxValue) = 0;
        virtual float DrawProgressBar(float x, float y, float width, float height, const std::string& textLeft, const std::string& textRight, float value, float minValue, float maxValue) = 0;
        virtual void DrawStatusBar(float x, float y, float width, float height, const std::string& text) = 0;
        virtual bool DrawWindowBox(float x, float y, float width, float height, const std::string& title) = 0;
        virtual void DrawGroupBox(float x, float y, float width, float height, const std::string& text) = 0;
        virtual void DrawPanel(float x, float y, float width, float height) = 0;
        virtual ZappyTypes::Rectangle DrawScrollPanel(float x, float y, float width, float height, const std::string& text, ZappyTypes::Rectangle content, std::shared_ptr<ZappyTypes::Vector2> scroll) = 0;
        virtual int DrawTabBar(float x, float y, float width, float height, const std::vector<std::string>& tabs, int active) = 0;
        virtual int DrawListView(float x, float y, float width, float height, const std::vector<std::string>& items, int active) = 0;
        virtual ZappyTypes::Color DrawColorPicker(float x, float y, float width, float height, ZappyTypes::Color color) = 0;
        virtual int DrawMessageBox(float x, float y, float width, float height, const std::string& title, const std::string& message, const std::string& buttons) = 0;
        virtual int DrawTextInputBox(float x, float y, float width, float height, const std::string& title, const std::string& message, std::shared_ptr<std::string> text, const std::string& buttons) = 0;
        virtual std::string IconText(int iconId, const std::string& text) = 0;
        virtual void LoadStyle(const std::string& file) = 0;
        virtual void SetStyle(int control, int property, int value) = 0;
        virtual int GetStyle(int control, int property) = 0;
};
