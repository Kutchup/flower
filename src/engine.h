#pragma once

#include "player.h"
#include "tool.h"
#include "pickup.h"
#include "limb.h"
#include "world.h"
#include <SDL3/SDL.h>
#include <vector>
#include <map>

// Grid-based world map (legacy - kept for compatibility)
class WorldGrid {
public:
    WorldGrid(int width, int height);
    
    enum class CellType {
        GRASS,
        DIRT,
        FLOWER,
        WATER
    };
    
    void setCell(int x, int z, CellType type);
    CellType getCell(int x, int z) const;
    bool isValidPosition(int x, int z) const;
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
private:
    int width;
    int height;
    std::vector<CellType> cells;
};

// Main game engine
class Engine {
public:
    Engine();
    ~Engine();
    
    bool initialize();
    void run();
    void shutdown();
    
    Player& getPlayer() { return player; }
    WorldGrid& getWorld() { return world; }
    World& getWorldSystem() { return worldSystem; }  // New world system
    
private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    
    void handleKeyboard(float deltaTime);
    void handleMouse();
    
    // Rendering helpers
    void renderWorld();
    void renderTools();
    void renderPickups();
    void renderLimbs();
    void renderHUD();
    void drawGrid();
    void drawFlower(const Vec3& position, const Color& color, float size);
    void drawCube(const Vec3& position, const Color& color, float size);
    
    // Gameplay helpers
    void spawnFlowerLimbs(const Vec3& flowerPosition);
    void updateWorldTime(float deltaTime);
    void checkPlayerObjectives();
    float calculateFlowerDensity(int gridX, int gridZ, int radius);
    void generateInitialWorld();
    
    SDL_Window* window;
    SDL_GLContext glContext;
    
    Player player;
    WorldGrid world;         // Legacy grid system
    World worldSystem;       // New enhanced world system with entities and slopes
    
    std::vector<Tool*> tools;
    std::vector<Pickup*> pickups;
    std::vector<Limb*> limbs;
    
    bool running;
    bool mouseCaptured;
    Uint64 lastTime;
    
    // Input state
    bool keyForward, keyBackward, keyLeft, keyRight, keyUp, keyDown;
};
