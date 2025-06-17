# Architecture de l'Interface Utilisateur de Zappy GUI

## Présentation générale

L'interface utilisateur (UI) du projet Zappy GUI a été refactorisée en utilisant plusieurs patterns de conception pour améliorer sa modularité, sa maintenabilité et son extensibilité. Cette documentation présente l'architecture mise en place et explique comment l'étendre.

## Hiérarchie des classes

Le système d'UI repose sur une hiérarchie de classes claire :

```
IUIWindow (Interface)
    |
    +-- AUIWindow (Classe abstraite)
            |
            +-- LogsWindow
            +-- TileInfoWindow
            +-- PlayerInfoWindow
            +-- BroadcastsWindow
            +-- ControlsWindow
            +-- TimeInfoWindow
            +-- MenuWindow
            +-- [autres fenêtres à venir]
```

### Composants principaux

1. **UserInterface** : Point d'entrée principal pour l'UI, délègue les opérations à UIWindowFactory
2. **UIWindowFactory** : Fabrique responsable de la création et de la gestion des fenêtres
3. **IUIWindow** : Interface définissant le contrat pour toutes les fenêtres UI
4. **AUIWindow** : Classe abstraite implémentant les comportements communs de base des fenêtres
5. **Fenêtres concrètes** : Classes spécialisées pour chaque type de fenêtre

## Pattern de conception utilisés

### Factory Pattern

Le pattern Factory est utilisé pour encapsuler la création et la gestion des différentes fenêtres. La classe `UIWindowFactory` est responsable de :
- Créer les instances des fenêtres
- Fournir un accès centralisé à toutes les fenêtres
- Gérer le cycle de vie des fenêtres

Avantages :
- Centralisation de la création des fenêtres
- Facilité pour ajouter de nouveaux types de fenêtres
- Séparation claire des responsabilités

### Interface et Classe abstraite

L'utilisation d'une interface (`IUIWindow`) et d'une classe abstraite (`AUIWindow`) permet de :
- Définir un contrat clair pour toutes les fenêtres
- Mutualiser du code commun
- Faciliter l'extensibilité du système

## Comment ajouter une nouvelle fenêtre

1. Créer une nouvelle classe héritant de `AUIWindow`
2. Implémenter les méthodes abstraites `renderContent()` et `updateSpecificData()`
3. Ajouter la création de la fenêtre dans `UIWindowFactory::createAllWindows()`
4. Ajouter des méthodes spécifiques dans `UIWindowFactory` pour interagir avec la nouvelle fenêtre

Exemple :
```cpp
// NouvelleWindow.hpp
class NouvelleWindow : public AUIWindow {
public:
    NouvelleWindow(std::shared_ptr<IGuiLib> guiLib);
    ~NouvelleWindow() = default;
    
    // Méthodes spécifiques à cette fenêtre
    
protected:
    void renderContent() override;
    void updateSpecificData(const GameData& gameData) override;
};
```

## Interaction entre les fenêtres

Les fenêtres peuvent interagir entre elles via la `UIWindowFactory` qui sert de médiateur. Par exemple, lorsqu'une tuile est sélectionnée, la factory met à jour la `TileInfoWindow` et ajoute également un message dans la `LogsWindow`.

## Gestion des événements

Le système de gestion des événements de la souris est centralisé dans la classe `UserInterface` qui délègue ensuite à la `UIWindowFactory`. Cela permet notamment de gérer le glissement des fenêtres de manière transparente.

## Conclusion

Cette architecture refactorisée offre une base solide et extensible pour l'interface utilisateur de Zappy GUI. Elle facilite l'ajout de nouvelles fonctionnalités tout en maintenant une séparation claire des responsabilités.
