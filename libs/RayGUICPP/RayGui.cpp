/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** RayGui implementation
*/
#include <vector>
#include <string>
#include <memory>
#include "RayGui.hpp"
#include "../TypeAdapter.hpp"
#include "include/RayGuiEncap.hpp"

RayGui::RayGui() {}
RayGui::~RayGui() {}

void RayGui::DrawButton(float x, float y, float width, float height, const std::string& text) {
    RayGUICPP::Button::Draw({x, y, width, height}, text);
}
bool RayGui::ButtonPressed(float x, float y, float width, float height, const std::string& text) {
    return RayGUICPP::Button::Draw({x, y, width, height}, text);
}
void RayGui::DrawLabel(float x, float y, float width, float height, const std::string& text) {
    RayGUICPP::Label::Draw({x, y, width, height}, text);
}
bool RayGui::DrawCheckBox(float x, float y, float width, float height, const std::string& text, bool checked) {
    return RayGUICPP::CheckBox::Draw({x, y, width, height}, text, checked);
}
bool RayGui::DrawToggle(float x, float y, float width, float height, const std::string& text, bool active) {
    return RayGUICPP::Toggle::Draw({x, y, width, height}, text, active);
}
int RayGui::DrawToggleGroup(float x, float y, float width, float height, const std::string& text, int active) {
    return RayGUICPP::ToggleGroup::Draw({x, y, width, height}, text, active);
}
int RayGui::DrawComboBox(float x, float y, float width, float height, const std::string& text, int active) {
    return RayGUICPP::ComboBox::Draw({x, y, width, height}, text, active);
}
int RayGui::DrawDropdownBox(float x, float y, float width, float height, const std::string& text, int active, bool editMode) {
    return RayGUICPP::DropdownBox::Draw({x, y, width, height}, text, active, editMode);
}
bool RayGui::DrawTextBox(float x, float y, float width, float height, std::shared_ptr<std::string> text, int maxLength, bool editMode) {
    return RayGUICPP::TextBox::Draw({x, y, width, height}, text, maxLength, editMode);
}
bool RayGui::DrawValueBox(float x, float y, float width, float height, std::shared_ptr<int> value, int minValue, int maxValue, bool editMode) {
    return RayGUICPP::ValueBox::Draw({x, y, width, height}, value, minValue, maxValue, editMode);
}
bool RayGui::DrawSpinner(float x, float y, float width, float height, std::shared_ptr<int> value, int minValue, int maxValue, bool editMode) {
    return RayGUICPP::Spinner::Draw({x, y, width, height}, value, minValue, maxValue, editMode);
}
float RayGui::DrawSlider(float x, float y, float width, float height, const std::string& textLeft, const std::string& textRight, float value, float minValue, float maxValue) {
    return RayGUICPP::Slider::Draw({x, y, width, height}, textLeft, textRight, value, minValue, maxValue);
}
float RayGui::DrawProgressBar(float x, float y, float width, float height, const std::string& textLeft, const std::string& textRight, float value, float minValue, float maxValue) {
    return RayGUICPP::ProgressBar::Draw({x, y, width, height}, textLeft, textRight, value, minValue, maxValue);
}
void RayGui::DrawStatusBar(float x, float y, float width, float height, const std::string& text) {
    RayGUICPP::StatusBar::Draw({x, y, width, height}, text);
}
bool RayGui::DrawWindowBox(float x, float y, float width, float height, const std::string& title) {
    return RayGUICPP::WindowBox::Draw({x, y, width, height}, title);
}
void RayGui::DrawGroupBox(float x, float y, float width, float height, const std::string& text) {
    RayGUICPP::GroupBox::Draw({x, y, width, height}, text);
}
void RayGui::DrawPanel(float x, float y, float width, float height) {
    RayGUICPP::Panel::Draw({x, y, width, height});
}
ZappyTypes::Rectangle RayGui::DrawScrollPanel(float x, float y, float width, float height, const std::string& text, ZappyTypes::Rectangle content, std::shared_ptr<ZappyTypes::Vector2> scroll) {
    if (!scroll) return ZappyTypes::Rectangle{};
    auto raylibContent = TypeAdapter::ToRaylib(content);
    auto raylibScroll = std::make_shared<Vector2>(TypeAdapter::ToRaylib(*scroll));
    auto result = RayGUICPP::ScrollPanel::Draw({x, y, width, height}, text, raylibContent, raylibScroll);
    *scroll = TypeAdapter::FromRaylib(*raylibScroll);
    return TypeAdapter::FromRaylib(result);
}
int RayGui::DrawTabBar(float x, float y, float width, float height, const std::vector<std::string>& tabs, int active) {
    return RayGUICPP::TabBar::Draw({x, y, width, height}, tabs, active);
}
int RayGui::DrawListView(float x, float y, float width, float height, const std::vector<std::string>& items, int active) {
    return RayGUICPP::ListView::Draw({x, y, width, height}, items, active);
}
ZappyTypes::Color RayGui::DrawColorPicker(float x, float y, float width, float height, ZappyTypes::Color color) {
    return TypeAdapter::FromRaylib(RayGUICPP::ColorPicker::Draw({x, y, width, height}, TypeAdapter::ToRaylib(color)));
}
int RayGui::DrawMessageBox(float x, float y, float width, float height, const std::string& title, const std::string& message, const std::string& buttons) {
    return RayGUICPP::MessageBox::Draw({x, y, width, height}, title, message, buttons);
}
int RayGui::DrawTextInputBox(float x, float y, float width, float height, const std::string& title, const std::string& message, std::shared_ptr<std::string> text, const std::string& buttons) {
    return RayGUICPP::TextInputBox::Draw({x, y, width, height}, title, message, text, buttons);
}
std::string RayGui::IconText(int iconId, const std::string& text) {
    return RayGUICPP::Icon::IconText(iconId, text);
}
void RayGui::LoadStyle(const std::string& file) {
    RayGUICPP::RayGui::LoadStyle(file);
}
void RayGui::SetStyle(int control, int property, int value) {
    RayGUICPP::RayGui::SetStyle(control, property, value);
}
int RayGui::GetStyle(int control, int property) {
    return RayGUICPP::RayGui::GetStyle(control, property);
}

extern "C" {
    std::shared_ptr<IGuiLib> createGuiLib() {
        static std::shared_ptr<RayGui> instance = std::make_shared<RayGui>();
        return instance;
    }
}
