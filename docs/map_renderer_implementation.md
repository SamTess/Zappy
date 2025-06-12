# Implémentation du rendu de carte pour Zappy

## Résumé

Le système de rendu de carte développé pour le projet Zappy utilise plusieurs design patterns pour créer une architecture modulaire, extensible et performante. Le système permet d'afficher une grille 3D représentant la carte du jeu avec ses tuiles, ressources, joueurs et autres éléments.

## Design Patterns utilisés

1. **Observer Pattern** - Pour la communication entre le contexte graphique et le renderer
2. **Strategy Pattern** - Pour les différentes approches de rendu des tuiles
3. **Factory Pattern** - Pour la création des stratégies de rendu
4. **Bridge Pattern** - Pour découpler le rendu logique de l'implémentation graphique
5. **Level of Detail (LOD) Pattern** - Pour adapter le niveau de détail en fonction du zoom

## Fonctionnalités clés

1. **Rendu adaptatif**: Choix automatique du niveau de détail en fonction du zoom
2. **Visualisation des ressources**: Représentation visuelle des ressources présentes sur chaque tuile
3. **Support de modèles 3D**: Possibilité d'utiliser des modèles 3D pour les tuiles via ModelManager
4. **Optimisation pour grandes cartes**: Ajustement automatique de la taille des tuiles

## Fichiers modifiés/créés

- `src/GUI/renderer/MapRenderer.hpp`
- `src/GUI/renderer/MapRenderer.cpp` 
- `src/GUI/graphicalContext/GraphicalContext.hpp` (enums et structures)
- `src/GUI/graphicalContext/GraphicalContext.cpp` (méthodes d'accès aux données)
- `docs/design_patterns_map_renderer.md` (documentation des patterns utilisés)
- `src/GUI/tests/test_map_renderer.cpp` (test de démonstration)
- `src/GUI/tests/Makefile` (pour compiler le test)

## Critères d'acceptation

Tous les critères d'acceptation de l'issue ont été respectés:

- ✅ **La carte complète est visible**: Le système affiche correctement l'ensemble de la carte
- ✅ **Les tuiles ne se chevauchent pas**: L'espacement est géré avec le paramètre tileSpacing
- ✅ **Le rendu reste fluide pour des maps de grande taille**: 
  - Ajustement automatique de la taille des tuiles
  - Système de Level of Detail
  - Optimisations de rendu

## Tests

Un programme de test a été fourni pour vérifier le bon fonctionnement du système avec différentes tailles de carte. Pour l'exécuter:

```bash
cd src/GUI/tests
make
./test_map_renderer
```

Controls:
- R: Augmenter la taille de la carte
- T: Diminuer la taille de la carte
- Molette de souris: Zoom

## Extensions possibles

1. **Frustum culling**: Optimisation pour ne rendre que les tuiles visibles dans la caméra
2. **Animation des ressources**: Ajouter des animations pour les ressources sur les tuiles
3. **Effets visuels**: Particules ou effets pour les incantations et événements spéciaux
4. **Transitions fluides**: Animation lors des changements d'état des tuiles
