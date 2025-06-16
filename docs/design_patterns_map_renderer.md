# Design Patterns utilisés pour le rendu de la carte

## Problématique
Implémenter le rendu de la carte à partir des données serveur en utilisant une architecture flexible qui peut s'adapter à différents types de rendu et permettre de changer facilement les stratégies de visualisation.

## Design Patterns implémentés

### 1. Observer Pattern
Le pattern Observer est utilisé pour réagir aux changements dans le `GraphicalContext`. Il permet de découpler la logique de mise à jour des données (dans `GraphicalContext`) de la logique d'affichage (dans `MapRenderer`).

- `IGraphicalContextObserver` : Interface pour les observateurs
- `MapRendererObserver` : Observateur concret qui adapte le rendu en fonction des changements

Avantage : Mise à jour automatique du rendu lorsque les données changent, sans couplage fort entre les classes.

### 2. Strategy Pattern
Le pattern Strategy est utilisé pour encapsuler différentes approches de rendu de tuiles.

- `ITileRenderStrategy` : Interface définissant le comportement commun
- `SimpleTileRenderStrategy` : Implémentation pour le rendu simple (cubes)
- `ModelTileRenderStrategy` : Implémentation pour le rendu avec modèles 3D
- `DetailedTileRenderStrategy` : Implémentation pour le rendu détaillé avec visualisation des ressources

Avantage : Permet de changer l'algorithme de rendu des tuiles à l'exécution, sans modifier le code client.

### 3. Factory Pattern
Le pattern Factory est utilisé pour créer les différentes stratégies de rendu.

- `TileRenderStrategyFactory` : Fabrique qui crée les différentes stratégies

Avantage : Centralise la logique de création et facilite le changement de stratégies.

### 4. Bridge Pattern
Le pattern Bridge est utilisé pour séparer l'abstraction (rendu de carte) de son implémentation (bibliothèque graphique).

- `MapRenderer` : Abstraction qui utilise une implémentation
- `IGraphicsLib` : Interface d'implémentation

Avantage : Découplage entre le rendu logique et la bibliothèque graphique sous-jacente.

### 5. Level of Detail (LOD) Pattern
Le pattern Level of Detail est utilisé pour adapter le niveau de détail du rendu en fonction du niveau de zoom.

- Zoom éloigné : Utilisation des stratégies simples ou avec modèles pour une performance optimale
- Zoom rapproché : Utilisation de la stratégie détaillée pour afficher les ressources individuellement

Avantage : Optimise les performances tout en fournissant une expérience utilisateur riche.

## Conception modulaire

L'architecture permet:
1. **Flexibilité** : Changer facilement l'apparence des tuiles sans modifier le code du renderer
2. **Extensibilité** : Ajouter de nouvelles stratégies de rendu sans perturber le code existant
3. **Réutilisabilité** : Les composants sont indépendants et peuvent être réutilisés dans d'autres parties de l'application
4. **Découplage** : Le rendu est découplé de la logique de jeu et des données
5. **Adaptabilité** : Le système s'adapte aux différentes échelles de carte et niveaux de zoom

## Utilisation du ModelManager

Le `ModelManager` est intégré via le pattern Strategy, ce qui permet de:

1. Utiliser des modèles 3D pour les tuiles quand c'est souhaité
2. Conserver une implémentation simple par défaut
3. Changer le modèle utilisé à l'exécution

Pour définir un rendu spécifique par ressource:
```cpp
// Exemple d'utilisation du ModelManager pour différents types de ressources
modelManager.loadModel("../assets/models/food.obj"); // ID 1
mapRenderer.setResourceRenderStrategy(
   static_cast<int>(ResourceType::FOOD), 
   strategyFactory.createModelTileStrategy(1)
);
```

## Visualisation détaillée des ressources

Le système permet d'afficher les détails des ressources lorsque l'utilisateur zoom sur une zone:

1. Les ressources sont visualisées comme de petits objets 3D sur les tuiles
2. La quantité de chaque ressource est représentée visuellement
3. Les joueurs et incantations sont clairement indiqués
4. La transition entre les différents niveaux de détail est fluide

## Performance et optimisations

Pour les cartes de grande taille:
1. Le renderer ajuste automatiquement la taille des tuiles pour s'adapter
2. La stratégie de rendu est simplifiée pour les tuiles éloignées
3. Le système observe uniquement les tuiles qui changent pour minimiser les mises à jour
4. Le niveau de détail adaptatif permet de maintenir des performances fluides

## Exemple d'intégration

Voici un exemple d'intégration du MapRenderer dans le GameLoop:

```cpp
// Dans GameLoop.hpp
private:
    // ...
    std::shared_ptr<GraphicalContext> m_context;
    std::shared_ptr<MapRenderer> m_mapRenderer;
    float m_currentZoomLevel;

// Dans GameLoop.cpp
void GameLoop::setupComponents() {
    // Initialisation du contexte graphique
    m_context = std::make_shared<GraphicalContext>();
    
    // Obtenir l'instance du ModelManager
    auto& modelManager = ModelManager::getInstance();
    
    // Création du MapRenderer
    m_mapRenderer = std::make_shared<MapRenderer>(m_graphics, m_context, modelManager);
    m_mapRenderer->initialize();
    m_currentZoomLevel = 1.0f;
    
    // Précharger quelques modèles pour les tuiles spéciales
    int foodModelId = modelManager.loadModel("../assets/models/food.obj");
    int crystalModelId = modelManager.loadModel("../assets/models/crystal.obj");
    
    // Configurer des stratégies de rendu spécifiques pour certaines ressources
    m_mapRenderer->setResourceRenderStrategy(
        static_cast<int>(ResourceType::FOOD),
        m_mapRenderer->getStrategyFactory().createModelTileStrategy(foodModelId)
    );
    
    m_mapRenderer->setResourceRenderStrategy(
        static_cast<int>(ResourceType::THYSTAME),
        m_mapRenderer->getStrategyFactory().createModelTileStrategy(crystalModelId)
    );
}

void GameLoop::render() {
    // Vérifier si l'utilisateur zoome/dézoome
    float mouseWheel = m_graphics->GetMouseWheelMove();
    if (mouseWheel != 0.0f) {
        // Mettre à jour le niveau de zoom
        m_currentZoomLevel += mouseWheel * 0.1f;
        m_currentZoomLevel = std::max(0.5f, std::min(5.0f, m_currentZoomLevel));
        m_mapRenderer->setZoomLevel(m_currentZoomLevel);
        
        // Ajuster également la caméra
        m_camera->setZoomLevel(m_currentZoomLevel);
    }
    
    // Début du rendu
    m_graphics->BeginDrawing();
    m_graphics->ClearBackground({20, 20, 40, 255}); // Fond bleu foncé
    
    m_graphics->BeginMode3D(m_camera->getCamera());
    
    // Rendu de la carte
    m_mapRenderer->render();
    
    m_graphics->EndMode3D();
    
    // Rendu de l'interface utilisateur
    m_uiRenderer->render(m_graphics, m_gui);
    
    m_graphics->EndDrawing();
}
```

## Gestion des grandes cartes

Pour les cartes de grande taille, le système implémente plusieurs optimisations:

1. **Ajustement automatique de la taille** - Les tuiles sont redimensionnées automatiquement:
```cpp
void MapRendererObserver::onMapSizeChanged(int width, int height) {
    if (width > 20 || height > 20) {
        float newSize = 10.0f / std::max(width, height);
        renderer.setTileSize(newSize);
        renderer.setTileSpacing(newSize * 0.1f);
    }
}
```

2. **Frustum culling** - Option d'extension pour ne rendre que les tuiles visibles:
```cpp
// Code potentiel pour le frustum culling (non implémenté dans la version actuelle)
bool isTileVisible(int x, int y, const Camera3D& camera) {
    ZappyTypes::Vector3 position = {
        x * (tileSize + tileSpacing),
        0.0f,
        y * (tileSize + tileSpacing)
    };
    return checkPointInFrustum(position, camera);
}
```
