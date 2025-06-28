---
sidebar_position: 2
---

# Gameplay Guide - How to Play Zappy

Welcome to the **Zappy Gameplay Guide**! This comprehensive guide will teach you everything you need to know about playing Zappy, a competitive multiplayer survival and evolution game where AI-controlled agents fight for supremacy in a resource-limited world.

## 🎯 Game Objective

The ultimate goal in Zappy is to **evolve 6 players from your team to level 8** before any other team achieves this milestone. Players must survive, collect resources, collaborate with teammates, and perform complex incantation rituals to advance through 8 levels of increasing difficulty.

## 🌍 Game World

### Map Structure
- **Grid-based world**: The game takes place on a rectangular grid of tiles
- **Wraparound edges**: The map is toroidal - moving off one edge teleports you to the opposite side
- **Resource distribution**: Each tile contains various amounts of resources
- **Dynamic environment**: Resources regenerate over time, creating strategic opportunities

### Visual Elements
The 3D GUI displays various cute character models representing both players and resources:

#### Player Models:
- **Baby Tripy Trophy** - A trophy-like character (used for eggs)
- **Baby Espressona Signora** - An elegant coffee-themed character (represents Mendiane resources)
- **Baby Frigo Camelo** - A cool camel character (represents Sibur resources)
- **Baby Garamaran** - A honey pot character (represents Deraumere resources)
- **Baby La Vaca Saturno** - A cosmic cow character (represents Food resources)
- **Baby TRALALERO TRALALA** - A shark with shoes character (represents Phiras resources)
- **Baby Trulimero Trulicina** - A cat-fish character (represents Linemate resources)
- **Baby Tung Tung Tung Sahur** - TUNG TUNG TUNG SAHUR (represents Thystame resources)
- **Labubu** - A popular collectible character (player character)
- **Island** - The base terrain model for tiles

## 🎮 Basic Gameplay Mechanics

### Starting the Game
1. **Choose your team**: Players are assigned to teams with unique names
2. **Spawn on the map**: Each player starts at a random location
3. **Begin survival**: You start with limited food and must immediately begin collecting resources

### Core Actions
- **Movement**: Navigate the grid in 4 directions (North, South, East, West)
- **Look around**: Survey surrounding tiles for resources and other players
- **Take objects**: Collect resources from the ground
- **Drop objects**: Place items from your inventory onto tiles
- **Communication**: Broadcast messages to coordinate with your team
- **Incantation**: Perform level-up rituals when requirements are met

## 📦 Resource System

Understanding resources is crucial for survival and advancement:

### Resource Types

#### 🍖 Food (Baby La Vaca Saturno)
- **Purpose**: Essential for survival - prevents starvation
- **Consumption**: Players automatically consume food over time
- **Critical importance**: Without food, players die and are removed from the game
- **Strategy**: Always maintain a food reserve for emergencies
- **3D Model**: Represented by the cosmic cow character "Baby La Vaca Saturno"

#### 💎 Precious Stones (Required for Incantations)

1. **🔹 Linemate** (Baby Trulimero Trulicina)
   - Most common evolution stone
   - Required for almost every level advancement
   - Fundamental building block of progression
   - **3D Model**: Represented by the cat-fish character "Baby Trulimero Trulicina"

2. **🔸 Deraumere** (Baby Garamaran)
   - Mid-tier evolution material
   - Becomes important from level 3 onwards
   - Often scarce, requiring strategic collection
   - **3D Model**: Represented by the honey pot character "Baby Garamaran"

3. **🔷 Sibur** (Baby Frigo Camelo)
   - Advanced evolution component
   - Critical for higher-level incantations
   - High strategic value
   - **3D Model**: Represented by the cool camel character "Baby Frigo Camelo"

4. **🟡 Mendiane** (Baby Espressona Signora)
   - Rare evolution material
   - Required for levels 5-8
   - Teams often fight over these resources
   - **3D Model**: Represented by the elegant coffee-themed character "Baby Espressona Signora"

5. **🟣 Phiras** (Baby TRALALERO TRALALA)
   - Elite evolution stone
   - Needed for the highest level advancements
   - Extremely valuable and contested
   - **3D Model**: Represented by the shark with shoes character "Baby TRALALERO TRALALA"

6. **🟢 Thystame** (Baby Tung Tung Tung Sahur)
   - Ultimate evolution material
   - Only required for the final level 7→8 transition
   - The rarest and most precious resource
   - **3D Model**: Represented by the "Baby Tung Tung Tung Sahur" character

## 🆙 Level Progression System

Players advance through 8 levels, each requiring specific resources and team coordination:

### Level Requirements

| Level Transition | Players Needed | Food | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|------------------|----------------|------|----------|-----------|-------|----------|--------|----------|
| **1 → 2** | 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| **2 → 3** | 2 | 1 | 1 | 1 | 1 | 0 | 0 | 0 |
| **3 → 4** | 2 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| **4 → 5** | 4 | 1 | 1 | 1 | 2 | 0 | 1 | 0 |
| **5 → 6** | 4 | 1 | 1 | 2 | 1 | 3 | 0 | 0 |
| **6 → 7** | 6 | 1 | 1 | 2 | 3 | 0 | 1 | 0 |
| **7 → 8** | 6 | 2 | 2 | 2 | 2 | 2 | 2 | 1 |

### Incantation Process

1. **Preparation Phase**
   - Gather required players on the same tile
   - Ensure all required resources are present on the tile
   - All participating players must be at the same level

2. **Ritual Initiation**
   - One player starts the incantation command
   - The ritual takes 300 game ticks to complete
   - Players become "busy" and cannot perform other actions

3. **Completion**
   - If successful, all participating players advance one level
   - Required resources are consumed from the tile
   - Players can immediately begin working toward the next level

4. **Failure Conditions**
   - Missing resources when the ritual completes
   - Insufficient players of the correct level
   - Players leaving the tile during the incantation

## 🗣️ Communication System

Effective communication is essential for team coordination:

### Broadcast Types
- **Team broadcasts**: Messages sent to all team members
- **Local broadcasts**: Messages heard by players within a certain distance
- **Distance-based**: Message clarity decreases with distance

### Strategic Communication
- **Resource coordination**: "I found linemate at (10,5)"
- **Meeting arrangements**: "Level 3 players meet at (15,20)"
- **Danger warnings**: "Enemy team spotted near base"
- **Incantation calls**: "Ready for level 4 ritual at my position"

## 🏆 Victory Conditions

### Team Victory
A team wins when they successfully get **6 players to level 8**. This requires:
- Massive resource collection and management
- Perfect team coordination
- Strategic territorial control
- Efficient incantation scheduling

### Game End
- The first team to achieve 6 level-8 players wins immediately
- All other teams lose regardless of their progress
- The victory is announced to all players and spectators

## 🎯 Strategic Tips

### Early Game (Levels 1-3)
- **Priority**: Establish a steady food supply
- **Focus**: Basic resource gathering and map exploration
- **Team coordination**: Share resource locations
- **Risk management**: Stay close to food sources

### Mid Game (Levels 4-6)
- **Resource specialization**: Different players focus on different stones
- **Territorial control**: Secure resource-rich areas
- **Timing coordination**: Plan group incantations
- **Information warfare**: Track enemy team progress

### Late Game (Levels 7-8)
- **Resource hoarding**: Accumulate materials for final pushes
- **Perfect execution**: Any mistake can cost the victory
- **All-in strategies**: Sometimes risk everything for the win
- **Defensive play**: Prevent other teams from completing their goals

## 🎨 Visual Experience

The Zappy GUI provides a rich 3D environment featuring:
- **Animated characters**: Each player is represented by unique, adorable models
- **Dynamic effects**: Incantation rituals create visual spectacles
- **Resource visualization**: Different stones have distinct appearances
- **Team colors**: Easy identification of friend and foe
- **Victory celebrations**: Special effects when teams achieve milestones

## 🔥 Advanced Tactics

### Resource Denial
- Collect resources your opponents need
- Control key strategic tiles
- Disrupt enemy incantation attempts

### Information Gathering
- Monitor enemy movements and communications
- Track opponent resource collection patterns
- Predict where enemies will attempt incantations

### Timing Strategies
- Rush early levels for advantage
- Coordinate simultaneous multi-level advancement
- Save resources for critical moments

### Risk vs Reward
- Balance individual survival with team goals
- Decide when to help teammates vs self-advancement
- Choose between safe play and aggressive expansion

---

Ready to enter the world of Zappy? Remember: survival, teamwork, and strategic thinking are the keys to victory. May the best team evolve! 🚀
