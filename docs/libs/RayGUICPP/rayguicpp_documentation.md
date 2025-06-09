# Documentation de RayGUICPP

## Vue d'ensemble

RayGUICPP est une encapsulation C++ de la bibliothèque raygui, conçue pour le projet Zappy. Elle permet de créer facilement des interfaces utilisateur graphiques en mode immédiat (IMGUI - Immediate Mode GUI) qui s'intègrent parfaitement avec RaylibCPP.

## Fonctionnalités

RayGUICPP fournit des classes et méthodes pour créer des éléments d'interface utilisateur comme :
- **Contrôles basiques** : Boutons, étiquettes, cases à cocher, etc.
- **Conteneurs** : Groupes, panels, fenêtres
- **Entrées** : Champs de texte, spinners, sliders
- **Listes et grilles** : ListViews, ComboBox, DropdownBox
- **Dialogs** : Boîtes de dialogue personnalisables
- **Styles** : Application et personnalisation de styles

## Architecture

L'encapsulation est construite sur le mode immédiat de raygui, où les contrôles sont redessinés à chaque frame. La structure principale comprend :
- **RayGuiEncap** : Namespace principal contenant les classes et fonctions
- **RayGui** : Classe centrale gérant l'interface et les styles
- **Contrôles** : Classes et fonctions pour chaque type de contrôle

## Utilisation

### Initialisation

```cpp
#include "RayGuiEncap.hpp"
#include "RayLib.hpp"

int main() {
    // Initialisation de la fenêtre avec RaylibCPP
    RaylibCPP::Window window(800, 600, "RayGUICPP Demo");
    
    // Initialisation de RayGUICPP
    RayGuiEncap::RayGui gui;
    
    // Chargement d'un style (optionnel)
    gui.loadStyle("styles/dark/style_dark.rgs");
    
    while (!window.shouldClose()) {
        window.beginDrawing();
        window.clearBackground(RaylibCPP::Color::RAYWHITE);
        
        // Utilisation des contrôles GUI
        if (gui.button({20, 20, 120, 30}, "Cliquez-moi")) {
            // Actions lorsque le bouton est cliqué
        }
        
        gui.label({20, 60, 120, 30}, "Ceci est une étiquette");
        
        static bool toggleValue = false;
        if (gui.toggle({20, 100, 120, 30}, "Activer", &toggleValue)) {
            // Actions lorsque le toggle change d'état
        }
        
        window.endDrawing();
    }
    
    return 0;
}
```

## Styles

RayGUICPP supporte les styles raygui qui peuvent être chargés à partir de fichiers `.rgs` :
- **Default** : Style par défaut de raylib
- **Dark** : Thème sombre élégant
- **Bluish** : Palette de bleus apaisants
- **Candy** : Style coloré et ludique
- **Cherry** : Thème rouge foncé
- **Cyber** : Style futuriste aux tons bleus
- **Lavanda** : Palette violette douce
- **Terminal** : Style rétro type terminal
- Et d'autres...

## Contrôles disponibles

- **Basiques** : Label, Button, Toggle, CheckBox, ComboBox, DropdownBox
- **Conteneurs** : GroupBox, Panel, ScrollPanel, TabBar
- **Entrée** : TextBox, ValueBox, Spinner, Slider, ProgressBar, ColorPicker
- **Listes** : ListView, ListViewEx, Grid
- **Avancés** : StatusBar, DummyRec, MessageBox, TextInputBox

## Intégration avec Zappy

RayGUICPP est utilisé dans le projet Zappy GUI pour :
- Afficher les informations sur les joueurs
- Présenter les statistiques du jeu
- Configurer les options de visualisation
- Créer des panneaux d'information

## Compilation

Pour compiler un programme utilisant RayGUICPP :

```bash
g++ -std=c++17 -Ilibs/RayGUICPP/include -Ilibs/RaylibCPP mon_programme.cpp \
    libs/RayGUICPP/raygui_impl.cpp libs/RayGUICPP/RayGui.cpp \
    libs/RaylibCPP/*.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Fonctions principales

- **RayGuiEncap::RayGui::button** : Créer un bouton cliquable
- **RayGuiEncap::RayGui::label** : Afficher du texte
- **RayGuiEncap::RayGui::slider** : Créer un slider pour valeurs numériques
- **RayGuiEncap::RayGui::panel** : Créer un panneau de regroupement
- **RayGuiEncap::RayGui::textBox** : Créer un champ de texte éditable
