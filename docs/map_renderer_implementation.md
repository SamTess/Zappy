# Map Rendering Implementation for Zappy

## Summary

The map rendering system developed for the Zappy project uses several design patterns to create a modular, extensible, and efficient architecture. The system displays a 3D grid representing the game map with its tiles, resources, players, and other elements.

## Design Patterns Used

1. **Observer Pattern** - For communication between the graphical context and the renderer
2. **Strategy Pattern** - For different approaches to tile rendering
3. **Factory Pattern** - For creating rendering strategies
4. **Bridge Pattern** - To decouple logical rendering from graphical implementation
5. **Level of Detail (LOD) Pattern** - To adapt the level of detail based on zoom

## Key Features

1. **Adaptive rendering**: Automatic selection of detail level based on zoom
2. **Resource visualization**: Visual representation of resources present on each tile
3. **3D Model Support**: Ability to use 3D models for tiles via ModelManager
4. **Optimization for large maps**: Automatic adjustment of tile size

## Modified/Created Files

- `src/GUI/renderer/MapRenderer.hpp`
- `src/GUI/renderer/MapRenderer.cpp` 
- `src/GUI/graphicalContext/GraphicalContext.hpp` (enums and structures)
- `src/GUI/graphicalContext/GraphicalContext.cpp` (data access methods)
- `docs/design_patterns_map_renderer.md` (documentation of used patterns)
- `src/GUI/tests/test_map_renderer.cpp` (demonstration test)
- `src/GUI/tests/Makefile` (to compile the test)

## Acceptance Criteria

All acceptance criteria for the issue have been met:

- ✅ **The entire map is visible**: The system correctly displays the whole map
- ✅ **Tiles do not overlap**: Spacing is managed with the tileSpacing parameter
- ✅ **Rendering remains smooth for large maps**: 
  - Automatic adjustment of tile size
  - Level of Detail system
  - Rendering optimizations

## Tests

A test program has been provided to verify the correct functioning of the system with different map sizes. To run it:

```bash
cd src/GUI/tests
make
./test_map_renderer
```

Controls:
- R: Increase map size
- T: Decrease map size
- Mouse wheel: Zoom

## Possible Extensions

1. **Frustum culling**: Optimization to render only the tiles visible in the camera
2. **Resource animations**: Add animations for resources on tiles
3. **Visual effects**: Particles or effects for spells and special events
4. **Smooth transitions**: Animation when changing tile states
