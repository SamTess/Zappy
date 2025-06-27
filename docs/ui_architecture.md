# Architecture de l'Interface Utilisateur de Zappy GUI

## Présentation générale

L'interface utilisateur (UI) du projet Zappy GUI a été développée en utilisant plusieurs patterns de conception avancés pour améliorer sa modularité, sa maintenabilité et son extensibilité. Cette architecture moderne permet une gestion efficace des fenêtres spécialisées et une interaction fluide avec le système de jeu.

## Hiérarchie des classes

Le système d'UI repose sur une hiérarchie de classes claire et extensible :

```
IUIWindow (Interface)
    |
    +-- AUIWindow (Classe abstraite)
            |
            +-- TileInfoWindow      (Informations détaillées sur les tuiles)
            +-- PlayerInfoWindow    (Statistiques et actions des joueurs)
            +-- TimeInfoWindow      (Informations temporelles et performances)
            +-- MenuWindow          (Configuration globale de l'application)
            +-- MapInfoWindow       (Informations générales sur la carte)
            +-- [fenêtres futures à ajouter facilement]
```

### Composants principaux

1. **UserInterface** : Point d'entrée principal pour l'UI, coordonne toutes les interactions
2. **UIWindowFactory** : Fabrique responsable de la création, gestion et coordination des fenêtres
3. **IUIWindow** : Interface définissant le contrat strict pour toutes les fenêtres UI
4. **AUIWindow** : Classe abstraite implémentant les comportements communs et la logique de base
5. **Fenêtres spécialisées** : Classes concrètes pour chaque type de fenêtre avec fonctionnalités dédiées
6. **UIContext** : Contexte partagé contenant les références aux services et données
7. **UIDataProvider** : Fournisseur de données centralisé pour toutes les fenêtres

## Patterns de conception utilisés

### Factory Pattern

Le pattern Factory est utilisé de manière sophistiquée pour encapsuler la création et la gestion des différentes fenêtres. La classe `UIWindowFactory` est responsable de :

**Fonctionnalités principales :**
- Créer et initialiser toutes les instances des fenêtres
- Fournir un accès centralisé et typé à toutes les fenêtres
- Gérer le cycle de vie complet des fenêtres (création, mise à jour, destruction)
- Coordonner les interactions entre fenêtres
- Gérer les événements de souris et les sélections

**Avantages :**
- Centralisation complète de la création des fenêtres
- Facilité maximale pour ajouter de nouveaux types de fenêtres
- Séparation claire des responsabilités entre création et utilisation
- Gestion unifiée des dépendances entre fenêtres

### Interface et Classe abstraite

L'utilisation combinée d'une interface (`IUIWindow`) et d'une classe abstraite (`AUIWindow`) permet de :

**Bénéfices de l'interface :**
- Définir un contrat strict et non-négociable pour toutes les fenêtres
- Garantir la cohérence des signatures de méthodes
- Faciliter les tests unitaires avec des mocks

**Bénéfices de la classe abstraite :**
- Mutualiser du code commun (gestion de visibilité, positionnement, état)
- Fournir des implémentations par défaut pour les comportements standards
- Faciliter l'extensibilité avec un minimum de code dupliqué

### Provider Pattern

Le `UIDataProvider` centralise l'accès aux données de jeu :
- **GameState** : État global du jeu
- **Données temporelles** : Fréquence, temps de jeu, FPS
- **Sélections** : Tuiles et joueurs sélectionnés
- **Configuration** : Paramètres de l'interface utilisateur

## Architecture détaillée des fenêtres

### TileInfoWindow
**Responsabilités :**
- Affichage détaillé des informations de tuile sélectionnée
- Visualisation des ressources présentes avec quantités
- Liste des joueurs présents sur la tuile
- Coordonnées et statistiques de la tuile

### PlayerInfoWindow
**Responsabilités :**
- Informations complètes du joueur (niveau, orientation, équipe)
- Inventaire détaillé avec toutes les ressources
- Commandes interactives (suivi de joueur, informations étendues)
- Historique des actions récentes

### TimeInfoWindow
**Responsabilités :**
- Informations temporelles en temps réel (fréquence serveur, temps de jeu)
- Métriques de performance (FPS, latence réseau)
- Statistiques de simulation
- Contrôles de vitesse de jeu

### MenuWindow
**Responsabilités :**
- Configuration audio (volume musique, effets sonores)
- Paramètres de gameplay (vitesse, transparence UI)
- Gestion de la visibilité des fenêtres
- Réinitialisation des positions par défaut
- Paramètres de rendu et d'affichage

### MapInfoWindow
**Responsabilités :**
- Informations générales sur la carte (dimensions, ressources totales)
- Statistiques d'équipes et de joueurs
- Vue d'ensemble du terrain de jeu
- Contrôles de carte et de zoom

## Comment ajouter une nouvelle fenêtre

### 1. Créer la classe de fenêtre
```cpp
// NouvelleWindow.hpp
class NouvelleWindow : public AUIWindow {
public:
    NouvelleWindow(std::shared_ptr<IGuiLib> guiLib);
    ~NouvelleWindow() = default;
    
    // Méthodes spécifiques à cette fenêtre
    void setSpecificData(const SpecificData& data);
    
protected:
    void renderContent() override;
    void updateSpecificData(std::shared_ptr<const GameState> gameState) override;
    
private:
    // Données spécifiques à cette fenêtre
    SpecificData _specificData;
};
```

### 2. Implémenter les méthodes abstraites
```cpp
// NouvelleWindow.cpp
void NouvelleWindow::renderContent() {
    // Logique de rendu spécifique
    _guiLib->DrawLabel(10, 30, 200, 20, "Contenu spécifique");
    // ... autres éléments d'interface
}

void NouvelleWindow::updateSpecificData(std::shared_ptr<const GameState> gameState) {
    // Mise à jour des données spécifiques depuis l'état du jeu
    if (gameState) {
        // Extraire et traiter les données nécessaires
    }
}
```

### 3. Intégrer dans la factory
```cpp
// Dans UIWindowFactory::createAllWindows()
_windows["nouvelleWindow"] = std::make_shared<NouvelleWindow>(_guiLib);
_windows["nouvelleWindow"]->setUIContext(_uiContext);
```

### 4. Ajouter les méthodes d'accès
```cpp
// Dans UIWindowFactory.hpp
std::shared_ptr<NouvelleWindow> getNouvelleWindow() const;

// Dans UIWindowFactory.cpp
std::shared_ptr<NouvelleWindow> UIWindowFactory::getNouvelleWindow() const {
    return std::dynamic_pointer_cast<NouvelleWindow>(_windows.at("nouvelleWindow"));
}
```

## Interaction entre les fenêtres

### Système de communication
Les fenêtres communiquent via plusieurs mécanismes :

**1. UIWindowFactory comme médiateur :**
- Coordination centralisée des interactions
- Propagation des sélections (tuiles, joueurs)
- Synchronisation des états entre fenêtres

**2. Événements de sélection :**
```cpp
// Sélection de tuile
void UIWindowFactory::setSelectedTile(int x, int y) {
    auto tileInfoWindow = getTileInfoWindow();
    if (tileInfoWindow) {
        tileInfoWindow->setSelectedTile(x, y);
    }
    // Mise à jour d'autres fenêtres si nécessaire
}
```

**3. UIContext partagé :**
- Accès uniforme aux services (réseau, commandes)
- Partage des données communes
- Configuration globale accessible

## Gestion avancée des événements

### Système de gestion des événements souris
Le système de gestion des événements est centralisé dans `UserInterface` et délégué intelligemment :

**1. Détection des interactions :**
```cpp
bool UserInterface::handleMouseEvents() {
    ZappyTypes::Vector2 mousePos = _guiLib->GetMousePosition();
    
    // Vérification des interactions avec les fenêtres
    if (_windowFactory->handleMouseEvents(mousePos)) {
        return true; // Événement consommé par l'UI
    }
    
    return false; // Événement libre pour la caméra/map
}
```

**2. Glissement de fenêtres :**
- Détection automatique des zones de titre
- Gestion fluide du glissement avec contraintes
- Sauvegarde des positions personnalisées

**3. Redimensionnement adaptatif :**
- Ajustement automatique selon la résolution d'écran
- Positions par défaut intelligentes
- Réinitialisation facile aux valeurs d'origine

## Performance et optimisations

### Rendu conditionnel
- Rendu uniquement des fenêtres visibles
- Mise à jour différentielle des données
- Cache des calculs coûteux

### Gestion mémoire
- Partage intelligent des ressources via smart pointers
- Libération automatique des ressources inutilisées
- Pool de fenêtres pour éviter les allocations répétées

## Extensibilité future

### Points d'extension identifiés
1. **Système de plugins** : Architecture prête pour des fenêtres sous forme de plugins
2. **Thèmes personnalisables** : Support facile de thèmes visuels
3. **Sauvegarde de configuration** : Persistance des paramètres utilisateur
4. **Multi-écrans** : Support natif pour configurations multi-écrans
5. **Internationalisation** : Architecture prête pour la traduction

### Nouvelles fenêtres envisageables
- **StatisticsWindow** : Graphiques et statistiques avancées
- **DebugWindow** : Outils de débogage pour développeurs
- **ReplayWindow** : Contrôles de replay et d'enregistrement
- **ChatWindow** : Interface de chat entre spectateurs

## Conclusion

Cette architecture refactorisée et enrichie offre une base extrêmement solide et extensible pour l'interface utilisateur de Zappy GUI. Elle facilite non seulement l'ajout de nouvelles fonctionnalités, mais assure également une maintenabilité élevée et une séparation claire des responsabilités. La combinaison des patterns Factory, Provider et Observer crée un système cohérent et performant qui peut évoluer facilement avec les besoins futurs du projet.
