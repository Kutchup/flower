#pragma once

#include "player.h"
#include "weapon.h"
#include "pickup.h"
#include "limb.h"
#include <SDL3/SDL.h>
#include <vector>
#include <map>

// Grid-based world map
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
    
private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    
    void handleKeyboard(float deltaTime);
    void handleMouse();
    
    // Rendering helpers
    void renderWorld();
    void renderWeapons();
    void renderPickups();
    void renderLimbs();
    void renderHUD();
    void drawGrid();
    void drawFlower(const Vec3& position, const Color& color, float size);
    void drawCube(const Vec3& position, const Color& color, float size);
    
    SDL_Window* window;
    SDL_GLContext glContext;
    
    Player player;
    WorldGrid world;
    
    std::vector<Weapon*> weapons;
    std::vector<Pickup*> pickups;
    std::vector<Limb*> limbs;
    
    bool running;
    bool mouseCaptured;
    Uint64 lastTime;
    
    // Input state
    bool keyForward, keyBackward, keyLeft, keyRight, keyUp, keyDown;
};
