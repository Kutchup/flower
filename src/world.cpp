#include "world.h"
#include <cmath>
#include <algorithm>
#include <iostream>

World::World(int width, int height)
    : width(width)
    , height(height)
{
    cells.resize(width * height);
    
    // Initialize all cells
    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            TerrainCell& cell = cells[cellIndex(x, z)];
            cell.type = CellType::GRASS;
            cell.height = 0.0f;
            cell.normal = Vec3::up();
            cell.color = Color(0.3f, 0.7f, 0.3f);  // Green grass
            cell.entity = nullptr;
        }
    }
    
    // Add default lighting
    lights.push_back(Light(Vec3(width / 2.0f, 20.0f, height / 2.0f), 
                          Color::white(), 1.0f, 100.0f));
}

World::~World() {
    // Clean up entities
    for (auto entity : entities) {
        delete entity;
    }
    entities.clear();
}

void World::update(float deltaTime) {
    // Update all entities
    for (auto entity : entities) {
        if (entity && entity->isActive()) {
            entity->update(deltaTime);
        }
    }
}

void World::setCell(int x, int z, CellType type) {
    if (isValidPosition(x, z)) {
        TerrainCell& cell = cells[cellIndex(x, z)];
        cell.type = type;
        
        // Set color based on type
        switch (type) {
            case CellType::GRASS:
                cell.color = Color(0.3f, 0.7f, 0.3f);
                break;
            case CellType::DIRT:
                cell.color = Color(0.5f, 0.3f, 0.2f);
                break;
            case CellType::FLOWER:
                cell.color = Color(0.3f, 0.7f, 0.3f);  // Grass base
                break;
            case CellType::WATER:
                cell.color = Color(0.2f, 0.4f, 0.8f);
                break;
            case CellType::STONE:
                cell.color = Color(0.5f, 0.5f, 0.5f);
                break;
            case CellType::SAND:
                cell.color = Color(0.9f, 0.8f, 0.6f);
                break;
        }
    }
}

World::CellType World::getCellType(int x, int z) const {
    if (isValidPosition(x, z)) {
        return cells[cellIndex(x, z)].type;
    }
    return CellType::GRASS;
}

World::TerrainCell* World::getCell(int x, int z) {
    if (isValidPosition(x, z)) {
        return &cells[cellIndex(x, z)];
    }
    return nullptr;
}

const World::TerrainCell* World::getCell(int x, int z) const {
    if (isValidPosition(x, z)) {
        return &cells[cellIndex(x, z)];
    }
    return nullptr;
}

bool World::isValidPosition(int x, int z) const {
    return x >= 0 && x < width && z >= 0 && z < height;
}

float World::getTerrainHeight(int x, int z) const {
    if (isValidPosition(x, z)) {
        return cells[cellIndex(x, z)].height;
    }
    return 0.0f;
}

float World::getTerrainHeight(const Vec3& worldPos) const {
    GridPos gridPos = worldToGrid(worldPos);
    return getTerrainHeight(gridPos.x, gridPos.z);
}

Vec3 World::getTerrainNormal(int x, int z) const {
    if (isValidPosition(x, z)) {
        return cells[cellIndex(x, z)].normal;
    }
    return Vec3::up();
}

Vec3 World::getTerrainNormal(const Vec3& worldPos) const {
    GridPos gridPos = worldToGrid(worldPos);
    return getTerrainNormal(gridPos.x, gridPos.z);
}

void World::setTerrainHeight(int x, int z, float height) {
    if (isValidPosition(x, z)) {
        cells[cellIndex(x, z)].height = height;
    }
}

void World::calculateTerrainNormals() {
    // Calculate normals for all cells based on surrounding heights
    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            calculateCellNormal(x, z);
        }
    }
}

void World::calculateCellNormal(int x, int z) {
    if (!isValidPosition(x, z)) return;
    
    // Get heights of neighboring cells
    float h = getTerrainHeight(x, z);
    float hLeft = isValidPosition(x - 1, z) ? getTerrainHeight(x - 1, z) : h;
    float hRight = isValidPosition(x + 1, z) ? getTerrainHeight(x + 1, z) : h;
    float hBack = isValidPosition(x, z - 1) ? getTerrainHeight(x, z - 1) : h;
    float hForward = isValidPosition(x, z + 1) ? getTerrainHeight(x, z + 1) : h;
    
    // Calculate tangent vectors
    Vec3 tangentX(2.0f, hRight - hLeft, 0.0f);
    Vec3 tangentZ(0.0f, hForward - hBack, 2.0f);
    
    // Calculate normal via cross product
    Vec3 normal = Vec3::cross(tangentZ, tangentX).normalized();
    
    // Ensure normal points upward
    if (normal.y < 0) {
        normal = normal * -1.0f;
    }
    
    cells[cellIndex(x, z)].normal = normal;
}

void World::addEntity(Entity* entity) {
    if (entity) {
        entities.push_back(entity);
    }
}

void World::removeEntity(Entity* entity) {
    auto it = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end()) {
        entities.erase(it);
    }
}

Entity* World::getEntityAt(const Vec3& position, float radius) {
    for (auto entity : entities) {
        if (entity && entity->isActive()) {
            float dist = entity->getPosition().distance(position);
            if (dist <= radius) {
                return entity;
            }
        }
    }
    return nullptr;
}

bool World::loadPrefabricatedMap(const std::string& mapName) {
    auto it = prefabricatedMaps.find(mapName);
    if (it == prefabricatedMaps.end()) {
        std::cerr << "Map not found: " << mapName << std::endl;
        return false;
    }
    
    const MapData& mapData = it->second;
    
    // Load terrain
    if (mapData.width == width && mapData.height == height) {
        for (int z = 0; z < height; z++) {
            for (int x = 0; x < width; x++) {
                int idx = cellIndex(x, z);
                if (idx < mapData.cells.size()) {
                    cells[idx].type = mapData.cells[idx];
                }
                if (idx < mapData.heights.size()) {
                    cells[idx].height = mapData.heights[idx];
                }
            }
        }
        
        calculateTerrainNormals();
        
        std::cout << "Loaded prefabricated map: " << mapName << std::endl;
        return true;
    }
    
    std::cerr << "Map dimensions don't match world dimensions" << std::endl;
    return false;
}

void World::savePrefabricatedMap(const std::string& mapName) {
    MapData mapData;
    mapData.name = mapName;
    mapData.description = "Custom saved map";
    mapData.width = width;
    mapData.height = height;
    
    // Save terrain data
    for (const auto& cell : cells) {
        mapData.cells.push_back(cell.type);
        mapData.heights.push_back(cell.height);
    }
    
    // Save entity positions
    for (const auto& entity : entities) {
        if (entity) {
            mapData.entityPositions.push_back(entity->getPosition());
            mapData.entityTypes.push_back(entity->getName());
        }
    }
    
    prefabricatedMaps[mapName] = mapData;
    std::cout << "Saved prefabricated map: " << mapName << std::endl;
}

World::MapData* World::createCustomMap(const std::string& name, const std::string& description) {
    MapData mapData;
    mapData.name = name;
    mapData.description = description;
    mapData.width = width;
    mapData.height = height;
    
    prefabricatedMaps[name] = mapData;
    return &prefabricatedMaps[name];
}

void World::generateFlatTerrain() {
    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            cells[cellIndex(x, z)].type = CellType::GRASS;
            cells[cellIndex(x, z)].height = 0.0f;
            cells[cellIndex(x, z)].normal = Vec3::up();
        }
    }
}

void World::generateHillyTerrain(float amplitude, float frequency) {
    // Generate simple hills using sine waves
    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            float h = amplitude * (
                std::sin(x * frequency) * std::cos(z * frequency) +
                std::sin(x * frequency * 0.7f) * std::cos(z * frequency * 1.3f) * 0.5f
            );
            
            cells[cellIndex(x, z)].height = h;
            
            // Set cell type based on height
            if (h < -0.5f) {
                cells[cellIndex(x, z)].type = CellType::WATER;
            } else if (h > 1.5f) {
                cells[cellIndex(x, z)].type = CellType::STONE;
            } else if (h > 1.0f) {
                cells[cellIndex(x, z)].type = CellType::DIRT;
            } else {
                cells[cellIndex(x, z)].type = CellType::GRASS;
            }
        }
    }
    
    calculateTerrainNormals();
}

void World::generateRandomFlowers(int count) {
    for (int i = 0; i < count; i++) {
        int x = rand() % width;
        int z = rand() % height;
        
        if (getCellType(x, z) == CellType::GRASS) {
            setCell(x, z, CellType::FLOWER);
        }
    }
}

void World::addLight(const Light& light) {
    lights.push_back(light);
}

void World::removeLight(size_t index) {
    if (index < lights.size()) {
        lights.erase(lights.begin() + index);
    }
}

Color World::calculateLightingAt(const Vec3& position) const {
    Color finalColor = Color::black();
    
    // Accumulate light from all sources
    for (const auto& light : lights) {
        float distance = light.position.distance(position);
        
        if (distance < light.radius) {
            // Simple distance-based attenuation
            float attenuation = 1.0f - (distance / light.radius);
            attenuation = std::max(0.0f, std::min(1.0f, attenuation));
            
            Color contribution = light.color * (light.intensity * attenuation);
            finalColor = finalColor + contribution;
        }
    }
    
    // Ensure color components don't exceed 1.0
    finalColor.r = std::min(1.0f, finalColor.r);
    finalColor.g = std::min(1.0f, finalColor.g);
    finalColor.b = std::min(1.0f, finalColor.b);
    
    return finalColor;
}

Vec3 World::gridToWorld(int x, int z) const {
    return Vec3(static_cast<float>(x) + 0.5f, 
                getTerrainHeight(x, z), 
                static_cast<float>(z) + 0.5f);
}

GridPos World::worldToGrid(const Vec3& worldPos) const {
    return GridPos(static_cast<int>(std::floor(worldPos.x)),
                   static_cast<int>(std::floor(worldPos.z)));
}

Vec3 World::projectVelocityOntoSurface(const Vec3& velocity, const Vec3& surfaceNormal) const {
    // Project velocity vector onto the surface plane defined by the normal
    // This ensures movement follows the slope
    Vec3 normalComponent = surfaceNormal * Vec3::dot(velocity, surfaceNormal);
    Vec3 tangentComponent = velocity - normalComponent;
    return tangentComponent;
}

Vec3 World::getSlopeDirection(const Vec3& surfaceNormal) const {
    // Get the direction of steepest descent on a slope
    Vec3 down = Vec3::down();
    Vec3 slopeDir = projectVelocityOntoSurface(down, surfaceNormal);
    return slopeDir.normalized();
}

float World::getSlopeAngle(const Vec3& surfaceNormal) const {
    // Calculate angle between surface normal and vertical up
    Vec3 up = Vec3::up();
    float dotProduct = Vec3::dot(surfaceNormal, up);
    
    // Clamp to valid range for acos
    dotProduct = std::max(-1.0f, std::min(1.0f, dotProduct));
    
    // Return angle in degrees
    float angleRad = std::acos(dotProduct);
    return angleRad * RAD_TO_DEG;
}
