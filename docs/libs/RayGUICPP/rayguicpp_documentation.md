# RayGUICPP Documentation

# RayGUICPP - Modern User Interface

## Overview

RayGUICPP is an advanced C++ wrapper for the raygui library, specifically designed for the Zappy project. It implements an object-oriented architecture with modern design patterns to create sophisticated immediate mode (IMGUI) user interfaces.

## Architecture and Design Patterns

### Implemented Patterns

**Factory Pattern:**
- Dynamic creation of interface components according to required type
- `ComponentFactory` for specialized widget instantiation
- Plugin support for custom component extensions

**Provider Pattern:**
- `StyleProvider` for centralized visual theme management
- `FontProvider` for font and size management
- `LocalizationProvider` for multi-language support

**Observer Pattern:**
- Event system for inter-component communication
- Automatic notifications on state changes
- Bidirectional binding between model and view

**State Pattern:**
- Window state management (active, minimized, modal)
- Component states (enabled, disabled, focused, hovered)
- Automatic state transitions with animations

## Fonctionnalités Avancées

RayGUICPP offre un système d'interface utilisateur complet avec :

### Composants de Base
- **Contrôles Primitifs :** Boutons avec états (normal, hover, pressed), labels avec formatage riche
- **Conteneurs :** Panneaux avec barres de défilement, groupes avec bordures personnalisables
- **Entrées :** Champs texte avec validation, spinners numériques, sliders avec plages personnalisées

### Composants Avancés
- **Fenêtres Modales :** Système de dialogue avec pile de modalité
- **Grilles de Données :** Affichage tabulaire avec tri et filtrage
- **Graphiques Temps Réel :** Courbes, histogrammes, indicateurs de performance
- **Contrôles Spécialisés :** Color picker, file browser, calendrier, time picker

### Système de Thèmes
- **Styles Dynamiques :** Chargement à chaud des thèmes .rgs
- **Personnalisation :** Éditeur de style intégré pour l'ajustement en temps réel
- **Animation :** Transitions fluides entre thèmes avec interpolation
- **Responsive Design :** Adaptation automatique aux différentes résolutions

### Gestion d'États
- **State Management :** Gestion centralisée de l'état de l'interface
- **Event Handling :** Système d'événements typés avec callbacks lambda
- **Data Binding :** Liaison automatique entre données et composants visuels
- **Validation :** Système de validation des entrées avec messages d'erreur

### Performance et Optimisations
- **Dirty Rectangles :** Rendu sélectif des zones modifiées
- **Component Pooling :** Réutilisation des composants pour éviter les allocations
- **Batched Rendering :** Regroupement des appels de rendu pour l'efficacité
- **Culling :** Élimination des composants hors écran

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
