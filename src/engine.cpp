#include "engine.h"
#include <iostream>
#include <cmath>

// OpenGL includes
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

WorldGrid::WorldGrid(int width, int height) 
    : width(width), height(height) {
    cells.resize(width * height, CellType::GRASS);
}

void WorldGrid::setCell(int x, int z, CellType type) {
    if (isValidPosition(x, z)) {
        cells[z * width + x] = type;
    }
}

WorldGrid::CellType WorldGrid::getCell(int x, int z) const {
    if (isValidPosition(x, z)) {
        return cells[z * width + x];
    }
    return CellType::GRASS;
}

bool WorldGrid::isValidPosition(int x, int z) const {
    return x >= 0 && x < width && z >= 0 && z < height;
}

Engine::Engine() 
    : window(nullptr)
    , glContext(nullptr)
    , world(50, 50)  // 50x50 grid
    , running(false)
    , mouseCaptured(false)
    , lastTime(0)
    , keyForward(false)
    , keyBackward(false)
    , keyLeft(false)
    , keyRight(false)
    , keyUp(false)
    , keyDown(false)
{
}

Engine::~Engine() {
    shutdown();
}

bool Engine::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    // Create window
    window = SDL_CreateWindow(
        "Flower - A Peaceful Adventure",
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Enable V-Sync
    SDL_GL_SetSwapInterval(1);
    
    // Initialize OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);  // Sky blue
    
    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = 800.0f / 600.0f;
    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float f = 1.0f / std::tan(fov * 3.14159f / 360.0f);
    
    GLfloat projection[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (farPlane + nearPlane)/(nearPlane - farPlane), -1,
        0, 0, (2*farPlane*nearPlane)/(nearPlane - farPlane), 0
    };
    glLoadMatrixf(projection);
    
    // Set player starting position
    player.setPosition(Vec3(25, 1.7f, 25));
    
    // Create some initial pickups (seeds)
    for (int i = 0; i < 5; i++) {
        pickups.push_back(new Pickup(Vec3(20 + i * 2, 0.5f, 20), Pickup::Type::SUNFLOWER_SEEDS));
        pickups.push_back(new Pickup(Vec3(20 + i * 2, 0.5f, 22), Pickup::Type::ROSE_SEEDS));
    }
    
    // Create initial tools (tools)
    tools.push_back(new Tool(Vec3(25, 1.5f, 20), Tool::Type::WATERING_CAN));
    tools.push_back(new Tool(Vec3(27, 1.5f, 20), Tool::Type::CAMERA));
    
    std::cout << "Flower game initialized successfully!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move" << std::endl;
    std::cout << "  Space/Shift - Up/Down" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  Left Click - Use tool" << std::endl;
    std::cout << "  E - Pick up items" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    
    return true;
}

void Engine::run() {
    running = true;
    lastTime = SDL_GetTicks();
    
    while (running) {
        handleEvents();
        
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // Cap delta time to prevent huge jumps
        if (deltaTime > 0.1f) deltaTime = 0.1f;
        
        update(deltaTime);
        render();
    }
}

void Engine::shutdown() {
    // Clean up tools
    for (auto tool : tools) {
        delete tool;
    }
    tools.clear();
    
    // Clean up pickups
    for (auto pickup : pickups) {
        delete pickup;
    }
    pickups.clear();
    
    // Clean up limbs
    for (auto limb : limbs) {
        delete limb;
    }
    limbs.clear();
    
    if (glContext) {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    SDL_Quit();
}

void Engine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
                
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_W:
                        keyForward = true;
                        break;
                    case SDLK_S:
                        keyBackward = true;
                        break;
                    case SDLK_A:
                        keyLeft = true;
                        break;
                    case SDLK_D:
                        keyRight = true;
                        break;
                    case SDLK_SPACE:
                        keyUp = true;
                        break;
                    case SDLK_LSHIFT:
                        keyDown = true;
                        break;
                    case SDLK_E:
                        // Pick up nearby items
                        {
                            Vec3 playerPos = player.getPosition();
                            for (auto it = pickups.begin(); it != pickups.end();) {
                                Vec3 diff = (*it)->getPosition() - playerPos;
                                if (diff.length() < 2.0f) {
                                    std::cout << "Picked up seeds!" << std::endl;
                                    delete *it;
                                    it = pickups.erase(it);
                                } else {
                                    ++it;
                                }
                            }
                            
                            for (auto it = tools.begin(); it != tools.end();) {
                                Vec3 diff = (*it)->getPosition() - playerPos;
                                if (diff.length() < 2.0f) {
                                    std::cout << "Picked up " << (*it)->getName() << "!" << std::endl;
                                    delete *it;
                                    it = tools.erase(it);
                                } else {
                                    ++it;
                                }
                            }
                        }
                        break;
                }
                break;
                
            case SDL_EVENT_KEY_UP:
                switch (event.key.key) {
                    case SDLK_W:
                        keyForward = false;
                        break;
                    case SDLK_S:
                        keyBackward = false;
                        break;
                    case SDLK_A:
                        keyLeft = false;
                        break;
                    case SDLK_D:
                        keyRight = false;
                        break;
                    case SDLK_SPACE:
                        keyUp = false;
                        break;
                    case SDLK_LSHIFT:
                        keyDown = false;
                        break;
                }
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Capture mouse on first click
                    if (!mouseCaptured) {
                        SDL_SetWindowRelativeMouseMode(window, true);
                        mouseCaptured = true;
                    } else {
                        // Use current tool (plant flower, water, take photo)
                        Vec3 pos = player.getPosition();
                        Vec3 forward = player.getForward();
                        Vec3 plantPos = pos + forward * 3.0f;
                        
                        GridPos gridPos(static_cast<int>(std::floor(plantPos.x)),
                                      static_cast<int>(std::floor(plantPos.z)));
                        
                        if (world.isValidPosition(gridPos.x, gridPos.z)) {
                            if (world.getCell(gridPos.x, gridPos.z) == WorldGrid::CellType::GRASS) {
                                world.setCell(gridPos.x, gridPos.z, WorldGrid::CellType::FLOWER);
                                player.incrementFlowersPlanted();
                                std::cout << "Planted a flower! Total: " << player.getFlowersPlanted() << std::endl;
                            } else if (world.getCell(gridPos.x, gridPos.z) == WorldGrid::CellType::FLOWER) {
                                player.incrementFlowersWatered();
                                std::cout << "Watered a flower! Total: " << player.getFlowersWatered() << std::endl;
                            }
                        }
                    }
                }
                break;
                
            case SDL_EVENT_MOUSE_MOTION:
                if (mouseCaptured) {
                    float sensitivity = 0.1f;
                    player.rotate(event.motion.xrel * sensitivity, 
                                -event.motion.yrel * sensitivity);
                }
                break;
        }
    }
}

void Engine::update(float deltaTime) {
    handleKeyboard(deltaTime);
    player.update(deltaTime);
    
    // Update tools
    for (auto tool : tools) {
        tool->update(deltaTime);
    }
    
    // Update pickups
    for (auto pickup : pickups) {
        pickup->update(deltaTime);
    }
    
    // Update limbs (flower petals/stems that can animate)
    for (auto limb : limbs) {
        limb->update(deltaTime);
    }
}

void Engine::handleKeyboard(float deltaTime) {
    float speed = 5.0f * deltaTime;
    
    if (keyForward) player.moveForward(speed);
    if (keyBackward) player.moveForward(-speed);
    if (keyRight) player.moveRight(speed);
    if (keyLeft) player.moveRight(-speed);
    if (keyUp) player.moveUp(speed);
    if (keyDown) player.moveUp(-speed);
}

void Engine::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up camera view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    Vec3 pos = player.getPosition();
    Vec3 forward = player.getForward();
    Vec3 up = player.getUp();
    Vec3 target = pos + forward;
    
    // Simple lookAt implementation
    Vec3 f = (target - pos).normalized();
    Vec3 s = Vec3::cross(f, up).normalized();
    Vec3 u = Vec3::cross(s, f);
    
    GLfloat viewMatrix[16] = {
        s.x, u.x, -f.x, 0,
        s.y, u.y, -f.y, 0,
        s.z, u.z, -f.z, 0,
        -Vec3::cross(s, Vec3(pos.x, pos.y, pos.z)).x,
        -Vec3::cross(u, Vec3(pos.x, pos.y, pos.z)).x,
        Vec3::cross(f, Vec3(pos.x, pos.y, pos.z)).x,
        1
    };
    
    // Simpler approach
    glRotatef(-player.getPitch(), 1, 0, 0);
    glRotatef(-player.getYaw(), 0, 1, 0);
    glTranslatef(-pos.x, -pos.y, -pos.z);
    
    // Render world
    renderWorld();
    renderTools();
    renderPickups();
    renderLimbs();
    
    SDL_GL_SwapWindow(window);
}

void Engine::renderWorld() {
    drawGrid();
    
    // Draw flowers on grid
    for (int z = 0; z < world.getHeight(); z++) {
        for (int x = 0; x < world.getWidth(); x++) {
            Vec3 cellPos(x + 0.5f, 0, z + 0.5f);
            
            WorldGrid::CellType cell = world.getCell(x, z);
            
            // Draw ground
            if (cell == WorldGrid::CellType::GRASS) {
                drawCube(cellPos, Color(0.3f, 0.7f, 0.3f), 1.0f);
            } else if (cell == WorldGrid::CellType::DIRT) {
                drawCube(cellPos, Color(0.5f, 0.3f, 0.2f), 1.0f);
            } else if (cell == WorldGrid::CellType::FLOWER) {
                drawCube(cellPos, Color(0.3f, 0.7f, 0.3f), 1.0f);
                // Draw flower on top
                Vec3 flowerPos = cellPos;
                flowerPos.y = 0.5f;
                
                // Randomize color based on position
                float hue = (x * 7 + z * 13) % 6;
                Color flowerColor;
                if (hue < 1) flowerColor = Color(1.0f, 0.8f, 0.0f);  // Yellow
                else if (hue < 2) flowerColor = Color(1.0f, 0.2f, 0.3f);  // Red
                else if (hue < 3) flowerColor = Color(1.0f, 0.4f, 0.6f);  // Pink
                else if (hue < 4) flowerColor = Color(0.9f, 0.9f, 1.0f);  // White
                else if (hue < 5) flowerColor = Color(0.6f, 0.3f, 0.9f);  // Purple
                else flowerColor = Color(1.0f, 0.6f, 0.2f);  // Orange
                
                drawFlower(flowerPos, flowerColor, 0.3f);
            }
        }
    }
}

void Engine::renderTools() {
    for (auto tool : tools) {
        Vec3 pos = tool->getPosition();
        Color color = tool->getColor();
        drawCube(pos, color, 0.3f);
    }
}

void Engine::renderPickups() {
    for (auto pickup : pickups) {
        Vec3 pos = pickup->getPosition();
        Color color = pickup->getColor();
        
        // Make pickups bob up and down
        float bobOffset = std::sin(SDL_GetTicks() / 300.0f) * 0.1f;
        pos.y += bobOffset;
        
        drawCube(pos, color, 0.2f);
    }
}

void Engine::renderLimbs() {
    for (auto limb : limbs) {
        Vec3 pos = limb->getPosition();
        Color color = limb->getColor();
        drawCube(pos, color, limb->getSize());
    }
}

void Engine::drawGrid() {
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.5f, 0.2f);
    
    // Draw grid lines every 5 units for visibility
    for (int i = 0; i <= world.getWidth(); i += 5) {
        glVertex3f(i, 0.01f, 0);
        glVertex3f(i, 0.01f, world.getHeight());
    }
    
    for (int i = 0; i <= world.getHeight(); i += 5) {
        glVertex3f(0, 0.01f, i);
        glVertex3f(world.getWidth(), 0.01f, i);
    }
    
    glEnd();
}

void Engine::drawFlower(const Vec3& position, const Color& color, float size) {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    
    // Draw stem (green)
    glColor4f(0.2f, 0.6f, 0.2f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0, -0.3f, 0);
    glVertex3f(0, 0, 0);
    glEnd();
    
    // Draw flower head (colored)
    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= 8; i++) {
        float angle = i * 3.14159f * 2.0f / 8.0f;
        glVertex3f(std::cos(angle) * size, 0, std::sin(angle) * size);
    }
    glEnd();
    
    glPopMatrix();
}

void Engine::drawCube(const Vec3& position, const Color& color, float size) {
    glPushMatrix();
    glTranslatef(position.x, position.y - size/2, position.z);
    
    float s = size / 2.0f;
    
    glColor4f(color.r, color.g, color.b, color.a);
    
    // Draw cube faces
    glBegin(GL_QUADS);
    
    // Front
    glVertex3f(-s, -s, s);
    glVertex3f(s, -s, s);
    glVertex3f(s, s, s);
    glVertex3f(-s, s, s);
    
    // Back
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, -s, -s);
    
    // Top
    glVertex3f(-s, s, -s);
    glVertex3f(-s, s, s);
    glVertex3f(s, s, s);
    glVertex3f(s, s, -s);
    
    // Bottom
    glVertex3f(-s, -s, -s);
    glVertex3f(s, -s, -s);
    glVertex3f(s, -s, s);
    glVertex3f(-s, -s, s);
    
    // Right
    glVertex3f(s, -s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, s, s);
    glVertex3f(s, -s, s);
    
    // Left
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, -s, s);
    glVertex3f(-s, s, s);
    glVertex3f(-s, s, -s);
    
    glEnd();
    
    glPopMatrix();
}

// Additional helper methods for enhanced gameplay

void Engine::spawnFlowerLimbs(const Vec3& flowerPosition) {
    // Create animated limbs for a newly planted flower
    // This makes flowers come alive with movement
    
    // Create stem
    limbs.push_back(new Limb(
        Vec3(flowerPosition.x, flowerPosition.y - 0.3f, flowerPosition.z),
        Limb::Type::STEM,
        Vec3(flowerPosition.x, flowerPosition.y - 0.5f, flowerPosition.z)
    ));
    
    // Create petals in a circle around flower center
    const int petalCount = 8;
    for (int i = 0; i < petalCount; i++) {
        float angle = (i / static_cast<float>(petalCount)) * 2.0f * 3.14159f;
        float radius = 0.2f;
        Vec3 petalPos = flowerPosition + Vec3(
            std::cos(angle) * radius,
            0,
            std::sin(angle) * radius
        );
        
        limbs.push_back(new Limb(petalPos, Limb::Type::PETAL, flowerPosition));
    }
    
    // Create leaves
    limbs.push_back(new Limb(
        Vec3(flowerPosition.x - 0.15f, flowerPosition.y - 0.2f, flowerPosition.z),
        Limb::Type::LEAF,
        flowerPosition
    ));
    limbs.push_back(new Limb(
        Vec3(flowerPosition.x + 0.15f, flowerPosition.y - 0.2f, flowerPosition.z),
        Limb::Type::LEAF,
        flowerPosition
    ));
}

void Engine::updateWorldTime(float deltaTime) {
    // Track game time for day/night cycles (future enhancement)
    static float gameTime = 0.0f;
    gameTime += deltaTime;
    
    // Could be used to change lighting, sky color, etc.
}

void Engine::checkPlayerObjectives() {
    // Check if player has completed any objectives
    int planted = player.getFlowersPlanted();
    int watered = player.getFlowersWatered();
    int photos = player.getPhotographsTaken();
    
    // Milestone notifications
    if (planted == 10 || planted == 25 || planted == 50 || planted == 100) {
        std::cout << "🌸 Milestone! You've planted " << planted << " flowers!" << std::endl;
    }
    
    if (watered == 10 || watered == 25 || watered == 50) {
        std::cout << "💧 Milestone! You've watered " << watered << " flowers!" << std::endl;
    }
    
    if (photos == 5 || photos == 10 || photos == 25) {
        std::cout << "📷 Milestone! You've taken " << photos << " photographs!" << std::endl;
    }
}

float Engine::calculateFlowerDensity(int gridX, int gridZ, int radius) {
    // Calculate how many flowers are in an area
    // Useful for gameplay mechanics and aesthetics
    int flowerCount = 0;
    int totalCells = 0;
    
    for (int x = gridX - radius; x <= gridX + radius; x++) {
        for (int z = gridZ - radius; z <= gridZ + radius; z++) {
            if (world.isValidPosition(x, z)) {
                totalCells++;
                if (world.getCell(x, z) == WorldGrid::CellType::FLOWER) {
                    flowerCount++;
                }
            }
        }
    }
    
    if (totalCells == 0) return 0.0f;
    return static_cast<float>(flowerCount) / static_cast<float>(totalCells);
}

void Engine::generateInitialWorld() {
    // Create an initial world with some features
    // Add a few water spots for visual interest
    for (int i = 0; i < 5; i++) {
        int x = 10 + i * 8;
        int z = 10 + i * 7;
        if (world.isValidPosition(x, z)) {
            world.setCell(x, z, WorldGrid::CellType::WATER);
        }
    }
    
    // Create some pre-tilled dirt patches to guide player
    for (int x = 23; x <= 27; x++) {
        for (int z = 23; z <= 27; z++) {
            if (world.isValidPosition(x, z)) {
                world.setCell(x, z, WorldGrid::CellType::DIRT);
            }
        }
    }
}
