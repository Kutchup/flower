# Design Document - Flower Game

## Vision

"There's no grand villain, no impending doom, only the quiet task of reminding the world of its own splendor."

Flower is a peaceful first-person adventure game that subverts traditional FPS mechanics by replacing violence with beauty. Players explore a grid-based world, planting and nurturing flowers, taking photographs, and creating beautiful landscapes.

## Core Gameplay Loop

1. **Explore** - Move through the 50x50 grid world in first-person
2. **Collect** - Find seeds and tools scattered throughout the world
3. **Plant** - Use seeds to plant flowers in empty spaces
4. **Nurture** - Water flowers to help them grow and thrive
5. **Capture** - Take photographs of beautiful scenes you've created
6. **Expand** - Continue planting to create larger gardens

## Game Mechanics

### Movement System
- **First-Person Camera**: Full 360° camera rotation with mouse
- **WASD Movement**: Standard FPS movement controls
  - W key moves player horizontally forward relative to their look direction
  - Movement is projected onto the surface the player is standing on
  - Automatic slope detection and slope-aware movement
- **Vertical Movement**: Space/Shift for up/down (allows viewing from different angles)
- **Grid-Based World**: Discrete grid positions for planting
- **Slope Support**: Player movement adapts to terrain slopes
  - Surface normals calculated from terrain heights
  - Movement velocity projected onto surface plane
  - Maintains horizontal forward direction relative to view
  - Enables future support for hills and varied terrain

### Advanced Movement Mathematics
The player movement system uses sophisticated vector mathematics to ensure smooth movement on slopes:
1. **Horizontal Forward Direction**: The player's forward view vector is projected onto the XZ plane
2. **Surface Projection**: This horizontal direction is then projected onto the terrain surface using the surface normal
3. **Slope-Aware Movement**: Movement follows the terrain contours while maintaining the intended horizontal direction
4. **Automatic Mode Switching**: System automatically switches between flat and slope-aware movement based on terrain angle

### Tool System (Weapons)
Instead of weapons, players have peaceful tools:

1. **Seed Planter**
   - Plants flowers at targeted grid position
   - Different seed types create different colored flowers
   - Limited by seed inventory

2. **Watering Can**
   - Waters existing flowers to make them flourish
   - Triggers growth animations
   - Visual effect shows water particles

3. **Camera**
   - Captures screenshots of beautiful scenes
   - Counts towards completion objectives
   - Freezes the moment to appreciate beauty

### Collectibles System (Pickups)
Seeds and items can be collected:

- **Sunflower Seeds** (Yellow flowers)
- **Rose Seeds** (Red flowers)
- **Tulip Seeds** (Pink flowers)
- **Daisy Seeds** (White flowers)
- **Lavender Seeds** (Purple flowers)
- **Water Buckets** (Refill watering can)

### Animation System (Limbs)
Flowers have animated parts that respond to the environment:

- **Petals**: Rotate gently in the wind
- **Stems**: Sway side to side
- **Leaves**: Flutter and wave

## World Design

### Grid System
- 50x50 grid provides 2,500 potential planting locations
- Each grid cell can contain:
  - Grass (default state)
  - Dirt (prepared for planting)
  - Flower (planted and growing)
  - Water (decorative features)

### Cell Types
- **GRASS**: Natural ground cover, can be planted on
- **DIRT**: Tilled ground ready for seeds
- **FLOWER**: Growing plant with animations
- **WATER**: Decorative water features

## Objectives System

Players have multiple objectives to encourage exploration and creativity:

### Primary Objectives
- Plant 100 flowers across the world
- Water 50 flowers to full growth
- Take 25 photographs of beautiful scenes

### Secondary Objectives
- Create flower gardens in all four corners
- Plant at least one of each flower type
- Create a flower pattern or design
- Find all seed caches hidden in the world

### Achievement Ideas
- "First Bloom": Plant your first flower
- "Green Thumb": Water 10 flowers
- "Shutterbug": Take 10 photographs
- "Rainbow Garden": Plant all 5 flower types
- "Master Gardener": Plant 100 flowers
- "Nature Photographer": Take 25 photographs
- "World Beautifier": Complete all objectives

## Visual Design

### Color Palette
- **Sky**: Serene blue (#87CEEB)
- **Grass**: Fresh green (#4CAF50)
- **Flowers**: Vibrant, varied colors
  - Yellow (Sunflowers)
  - Red (Roses)
  - Pink (Tulips)
  - White (Daisies)
  - Purple (Lavender)

### Atmosphere
- Bright, cheerful lighting
- Clear skies
- No harsh shadows
- Soft, inviting aesthetics

## Audio Design (Future Enhancement)

### Music
- Gentle, peaceful ambient music
- Nature sounds (birds, wind)
- Calming melody that doesn't get repetitive

### Sound Effects
- Footsteps on grass
- Seed planting (soft thud)
- Water splashing (watering can)
- Camera shutter click
- Flower growth (gentle chime)
- Pickup collection (positive sound)

## Technical Architecture

### Core Classes

1. **Player** (`player.h/cpp`)
   - First-person camera controller
   - Position and orientation management
   - Statistics tracking
   - Slope-aware movement system
   - Surface normal tracking for terrain

2. **Engine** (`engine.h/cpp`)
   - Main game loop
   - Input handling
   - Rendering system
   - World management
   - Integration of Entity and World systems

3. **WorldGrid** (in `engine.h/cpp`)
   - Grid-based world representation (legacy)
   - Cell type management
   - Position validation

4. **World** (`world.h/cpp`) - NEW
   - Enhanced world management system
   - Terrain height and slope support
   - Surface normal calculation for slopes
   - Entity management
   - Prefabricated map loading/saving
   - Lighting system (for future versions)
   - Grid-to-world coordinate conversion

5. **Entity** (`entity.h/cpp`) - NEW
   - Base class for all game objects
   - Position, rotation, and scale
   - Physics properties (velocity, mass)
   - Surface normal for slope handling
   - Bounding box collision detection
   - Type system (Static, Dynamic, Interactive, Decorative)

6. **Tool** (`tool.h/cpp`)
   - Tool system (peaceful mechanics)
   - Cooldown management
   - Type-specific behaviors

7. **Pickup** (`pickup.h/cpp`)
   - Collectible items
   - Seed types
   - Bobbing animation

8. **Limb** (`limb.h/cpp`)
   - Animated flower parts
   - Wind simulation
   - Growth animations

### Rendering System
- OpenGL for 3D graphics
- Simple geometric primitives (cubes, quads)
- Flat shaded aesthetic
- Grid visualization

## Expansion Ideas

### Phase 2 Features
- More flower varieties
- Weather effects (rain makes flowers grow)
- Day/night cycle
- Seasonal changes
- Butterflies and bees
- Decorative objects (benches, paths)

### Phase 3 Features
- Multiple biomes
- Quest system with NPC characters
- Co-op multiplayer (plant gardens together)
- Garden customization tools
- Export screenshots/time-lapse videos

## Philosophy

This game is fundamentally about:
- **Mindfulness**: Being present in the moment
- **Creation**: Building something beautiful
- **Peace**: Finding calm in gameplay
- **Appreciation**: Learning to see beauty in simplicity
- **Joy**: Sharing happiness through art

The game should never feel stressful or competitive. There are no fail states, no enemies, no time limits. Players should feel free to explore, create, and appreciate at their own pace.

## Success Metrics

A successful playthrough means the player:
1. Feels relaxed and peaceful
2. Takes time to appreciate what they've created
3. Wants to continue playing to make the world more beautiful
4. Shares their creations (screenshots) with others
5. Returns to the game for a calming experience

The game succeeds when it helps players remember that the world is beautiful, and they have the power to make it even more so.
