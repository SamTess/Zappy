# RayGUICPP Documentation

## Overview

RayGUICPP is a C++ wrapper for the raygui library, designed for the Zappy project. It allows easy creation of immediate-mode graphical user interfaces (IMGUI) that integrate seamlessly with RaylibCPP.

## Features

RayGUICPP provides classes and methods to create UI elements such as:
- **Basic controls**: Buttons, labels, checkboxes, etc.
- **Containers**: Groups, panels, windows
- **Inputs**: Text fields, spinners, sliders
- **Lists and grids**: ListViews, ComboBox, DropdownBox
- **Dialogs**: Customizable dialog boxes
- **Styles**: Style application and customization

## Architecture

The wrapper is built on raygui's immediate mode, where controls are redrawn every frame. The main structure includes:
- **RayGuiEncap**: Main namespace containing classes and functions
- **RayGui**: Central class managing the interface and styles
- **Controls**: Classes and functions for each type of control

## Usage

### Initialization

```cpp
#include "RayGuiEncap.hpp"
#include "RayLib.hpp"

int main() {
    // Initialize window with RaylibCPP
    RaylibCPP::Window window(800, 600, "RayGUICPP Demo");
    
    // Initialize RayGUICPP
    RayGuiEncap::RayGui gui;
    
    // Load a style (optional)
    gui.loadStyle("styles/dark/style_dark.rgs");
    
    while (!window.shouldClose()) {
        window.beginDrawing();
        window.clearBackground(RaylibCPP::Color::RAYWHITE);
        
        // Using GUI controls
        if (gui.button({20, 20, 120, 30}, "Click me")) {
            // Actions when the button is clicked
        }
        
        gui.label({20, 60, 120, 30}, "This is a label");
        
        static bool toggleValue = false;
        if (gui.toggle({20, 100, 120, 30}, "Enable", &toggleValue)) {
            // Actions when the toggle state changes
        }
        
        window.endDrawing();
    }
    
    return 0;
}
```

## Styles

RayGUICPP supports raygui styles that can be loaded from `.rgs` files:
- **Default**: Default raylib style
- **Dark**: Elegant dark theme
- **Bluish**: Soothing blue palette
- **Candy**: Colorful and playful style
- **Cherry**: Dark red theme
- **Cyber**: Futuristic style with blue tones
- **Lavanda**: Soft purple palette
- **Terminal**: Retro terminal-like style
- And more...

## Available Controls

- **Basic**: Label, Button, Toggle, CheckBox, ComboBox, DropdownBox
- **Containers**: GroupBox, Panel, ScrollPanel, TabBar
- **Input**: TextBox, ValueBox, Spinner, Slider, ProgressBar, ColorPicker
- **Lists**: ListView, ListViewEx, Grid
- **Advanced**: StatusBar, DummyRec, MessageBox, TextInputBox

## Integration with Zappy

RayGUICPP is used in the Zappy GUI project to:
- Display player information
- Present game statistics
- Configure display options
- Create information panels

## Compilation

To compile a program using RayGUICPP :

```bash
g++ -std=c++17 -Ilibs/RayGUICPP/include -Ilibs/RaylibCPP mon_programme.cpp \
    libs/RayGUICPP/raygui_impl.cpp libs/RayGUICPP/RayGui.cpp \
    libs/RaylibCPP/*.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Main Functions

- **RayGuiEncap::RayGui::button** : Create a clickable button
- **RayGuiEncap::RayGui::label** : Display text
- **RayGuiEncap::RayGui::slider** : Create a slider for numeric values
- **RayGuiEncap::RayGui::panel** : Create a grouping panel
- **RayGuiEncap::RayGui::textBox** : Create an editable text field
