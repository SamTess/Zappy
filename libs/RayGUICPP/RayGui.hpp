/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** RayGUI encapsulation
*/

#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../../src/Shared/IGuiLib.hpp"
#include "../../src/Shared/Common.hpp"

class RayGui : public IGuiLib {
public:
    RayGui();
    ~RayGui() override;
    void DrawButton(float x, float y, float width, float height, const char* text) override;
    bool ButtonPressed(float x, float y, float width, float height, const char* text) override;
    void DrawLabel(float x, float y, float width, float height, const char* text) override;
    bool DrawCheckBox(float x, float y, float width, float height, const char* text, bool checked) override;
    bool DrawToggle(float x, float y, float width, float height, const char* text, bool active) override;
    int DrawToggleGroup(float x, float y, float width, float height, const char* text, int active) override;
    int DrawComboBox(float x, float y, float width, float height, const char* text, int active) override;
    int DrawDropdownBox(float x, float y, float width, float height, const char* text, int active, bool editMode) override;
    bool DrawTextBox(float x, float y, float width, float height, std::shared_ptr<std::string> text, int maxLength, bool editMode) override;
    bool DrawValueBox(float x, float y, float width, float height, std::shared_ptr<int> value, int minValue, int maxValue, bool editMode) override;
    bool DrawSpinner(float x, float y, float width, float height, std::shared_ptr<int> value, int minValue, int maxValue, bool editMode) override;
    float DrawSlider(float x, float y, float width, float height, const char* textLeft, const char* textRight, float value, float minValue, float maxValue) override;
    float DrawProgressBar(float x, float y, float width, float height, const char* textLeft, const char* textRight, float value, float minValue, float maxValue) override;
    void DrawStatusBar(float x, float y, float width, float height, const char* text) override;
    bool DrawWindowBox(float x, float y, float width, float height, const char* title) override;
    void DrawGroupBox(float x, float y, float width, float height, const char* text) override;
    void DrawPanel(float x, float y, float width, float height) override;
    ZappyTypes::Rectangle DrawScrollPanel(float x, float y, float width, float height, const char* text, ZappyTypes::Rectangle content, std::shared_ptr<ZappyTypes::Vector2> scroll) override;
    int DrawTabBar(float x, float y, float width, float height, const std::vector<std::string>& tabs, int active) override;
    int DrawListView(float x, float y, float width, float height, const std::vector<std::string>& items, int active) override;
    ZappyTypes::Color DrawColorPicker(float x, float y, float width, float height, ZappyTypes::Color color) override;
    int DrawMessageBox(float x, float y, float width, float height, const char* title, const char* message, const char* buttons) override;
    int DrawTextInputBox(float x, float y, float width, float height, const char* title, const char* message, std::shared_ptr<std::string> text, const char* buttons) override;
    std::string IconText(int iconId, const std::string& text) override;
    void LoadStyle(const std::string& file) override;
    void SetStyle(int control, int property, int value) override;
    int GetStyle(int control, int property) override;
};
