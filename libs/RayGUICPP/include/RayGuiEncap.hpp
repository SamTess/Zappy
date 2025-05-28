/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** RayGUI encapsulation
*/

#pragma once
#include <cstring>
#include <memory>
#include <algorithm>
#include <string>
#include <vector>
#include "../raygui-4.0/src/raygui.h"

namespace RayGUICPP {

class RayGui {
public:
    RayGui() = default;
    ~RayGui() = default;

    static void LoadStyle(const std::string& file) {
        GuiLoadStyle(file.c_str());
    }
    static void SetStyle(int control, int property, int value) {
        GuiSetStyle(control, property, value);
    }
    static int GetStyle(int control, int property) {
        return GuiGetStyle(control, property);
    }
};

class Button {
public:
    static bool Draw(const Rectangle& bounds, const std::string& text) {
        return GuiButton(bounds, text.c_str());
    }
};

class Label {
public:
    static void Draw(const Rectangle& bounds, const std::string& text) {
        GuiLabel(bounds, text.c_str());
    }
};

class CheckBox {
public:
    static bool Draw(const Rectangle& bounds, const std::string& text, bool checked) {
        bool value = checked;
        GuiCheckBox(bounds, text.c_str(), &value);
        return value;
    }
};

class Toggle {
public:
    static bool Draw(const Rectangle& bounds, const std::string& text, bool active) {
        bool value = active;
        if (GuiToggle(bounds, text.c_str(), &value)) {
            value = !active;
        }
        return value;
    }
};

class ToggleGroup {
public:
    static int Draw(const Rectangle& bounds, const std::string& text, int active) {
        int value = active;
        GuiToggleGroup(bounds, text.c_str(), &value);
        return value;
    }
};

class ComboBox {
public:
    static int Draw(const Rectangle& bounds, const std::string& text, int active) {
        int value = active;
        GuiComboBox(bounds, text.c_str(), &value);
        return value;
    }
};

class DropdownBox {
public:
    static int Draw(const Rectangle& bounds, const std::string& text, int active, bool editMode) {
        int value = active;
        GuiDropdownBox(bounds, text.c_str(), &value, editMode);
        return value;
    }
};

class TextBox {
public:
    static bool Draw(const Rectangle& bounds, std::shared_ptr<std::string> text, int maxLength, bool editMode) {
        if (!text || maxLength <= 0)
            return false;
        const int bufferSize = std::min(maxLength + 1, 1024);
        std::vector<char> buffer(bufferSize);
        std::strncpy(buffer.data(), text->c_str(), bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
        const int effectiveMaxLength = std::min(maxLength, bufferSize - 1);
        bool changed = GuiTextBox(bounds, buffer.data(), effectiveMaxLength, editMode);
        if (changed)
            *text = buffer.data();
        return changed;
    }
};

class ValueBox {
public:
    static bool Draw(const Rectangle& bounds, std::shared_ptr<int> value, int minValue, int maxValue, bool editMode) {
        return GuiValueBox(bounds, nullptr, value.get(), minValue, maxValue, editMode);
    }
};

class Spinner {
public:
    static bool Draw(const Rectangle& bounds, std::shared_ptr<int> value, int minValue, int maxValue, bool editMode) {
        return GuiSpinner(bounds, nullptr, value.get(), minValue, maxValue, editMode);
    }
};

class Slider {
public:
    static float Draw(const Rectangle& bounds, const std::string& textLeft, const std::string& textRight, float value, float minValue, float maxValue) {
        float v = value;
        GuiSlider(bounds, textLeft.c_str(), textRight.c_str(), &v, minValue, maxValue);
        return v;
    }
};

class ProgressBar {
public:
    static float Draw(const Rectangle& bounds, const std::string& textLeft, const std::string& textRight, float value, float minValue, float maxValue) {
        float v = value;
        GuiProgressBar(bounds, textLeft.c_str(), textRight.c_str(), &v, minValue, maxValue);
        return v;
    }
};

class StatusBar {
public:
    static void Draw(const Rectangle& bounds, const std::string& text) {
        GuiStatusBar(bounds, text.c_str());
    }
};

class WindowBox {
public:
    static bool Draw(const Rectangle& bounds, const std::string& title) {
        return GuiWindowBox(bounds, title.c_str());
    }
};

class GroupBox {
public:
    static void Draw(const Rectangle& bounds, const std::string& text) {
        GuiGroupBox(bounds, text.c_str());
    }
};

class Panel {
public:
    static void Draw(const Rectangle& bounds) {
        GuiPanel(bounds, nullptr);
    }
};

class ScrollPanel {
public:
    static Rectangle Draw(const Rectangle& bounds, const std::string& text, Rectangle content, std::shared_ptr<Vector2> scroll) {
        Rectangle view = {0};
        GuiScrollPanel(bounds, text.c_str(), content, scroll.get(), &view);
        return view;
    }
};

class TabBar {
public:
    static int Draw(const Rectangle& bounds, const std::vector<std::string>& tabs, int active) {
        std::vector<const char*> c_tabs;
        for (const auto& t : tabs) c_tabs.push_back(t.c_str());
        int value = active;
        GuiTabBar(bounds, c_tabs.data(), static_cast<int>(c_tabs.size()), &value);
        return value;
    }
};

class ListView {
public:
    static int Draw(const Rectangle& bounds, const std::vector<std::string>& items, int active) {
        std::string joined;
        for (size_t i = 0; i < items.size(); ++i) {
            joined += items[i];
            if (i + 1 < items.size()) joined += ";";
        }
        int value = active;
        int scrollIndex = 0;
        GuiListView(bounds, joined.c_str(), &scrollIndex, &value);
        return value;
    }
};

class ColorPicker {
public:
    static Color Draw(const Rectangle& bounds, Color color) {
        GuiColorPicker(bounds, nullptr, &color);
        return color;
    }
};

class MessageBox {
public:
    static int Draw(const Rectangle& bounds, const std::string& title, const std::string& message, const std::string& buttons) {
        return GuiMessageBox(bounds, title.c_str(), message.c_str(), buttons.c_str());
    }
};

class TextInputBox {
public:
    static int Draw(const Rectangle& bounds, const std::string& title, const std::string& message, std::shared_ptr<std::string> text, const std::string& buttons) {
        char buffer[1024];
        std::strncpy(buffer, text->c_str(), sizeof(buffer));
        buffer[sizeof(buffer)-1] = '\0';
        int result = GuiTextInputBox(bounds, title.c_str(), message.c_str(), buttons.c_str(), buffer, sizeof(buffer), nullptr);
        *text = buffer;
        return result;
    }
};

class Icon {
public:
    static std::string IconText(int iconId, const std::string& text) {
        return GuiIconText(iconId, text.c_str());
    }
};

} // namespace RayGUICPP
