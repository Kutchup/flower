#pragma once

#include "entity.h"
#include "math_utils.h"
#include <vector>
#include <map>
#include <memory>
#include <string>

// World class manages the game world, entities, and custom prefabricated maps
// Provides support for terrain, slopes, and entity management
class World {
public:
    World(int width, int height);
    ~World();
    
    // World update
    void update(float deltaTime);
    
    // World dimensions
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    // Terrain and cell management
    enum class CellType {
        GRASS,
        DIRT,
        FLOWER,
        WATER,
        STONE,
        SAND
    };
    
    struct TerrainCell {
        CellType type;
        float height;        // Height of terrain at this cell
        Vec3 normal;         // Surface normal for slopes
        Color color;         // Visual color
        Entity* entity;      // Entity placed at this cell (can be null)
        
        TerrainCell() 
            : type(CellType::GRASS)
            , height(0.0f)
            , normal(Vec3::up())
            , color(Color::green())
            , entity(nullptr)
        {}
    };
    
    void setCell(int x, int z, CellType type);
    CellType getCellType(int x, int z) const;
    TerrainCell* getCell(int x, int z);
    const TerrainCell* getCell(int x, int z) const;
    bool isValidPosition(int x, int z) const;
    
    // Terrain height and normal queries for slope support
    float getTerrainHeight(int x, int z) const;
    float getTerrainHeight(const Vec3& worldPos) const;
    Vec3 getTerrainNormal(int x, int z) const;
    Vec3 getTerrainNormal(const Vec3& worldPos) const;
    void setTerrainHeight(int x, int z, float height);
    
    // Calculate surface normal from surrounding heights (for slopes)
    void calculateTerrainNormals();
    void calculateCellNormal(int x, int z);
    
    // Entity management
    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);
    std::vector<Entity*>& getEntities() { return entities; }
    const std::vector<Entity*>& getEntities() const { return entities; }
    Entity* getEntityAt(const Vec3& position, float radius = 1.0f);
    
    // Prefabricated map loading
    struct MapData {
        std::string name;
        std::string description;
        int width;
        int height;
        std::vector<CellType> cells;
        std::vector<float> heights;
        std::vector<Vec3> entityPositions;
        std::vector<std::string> entityTypes;
    };
    
    bool loadPrefabricatedMap(const std::string& mapName);
    void savePrefabricatedMap(const std::string& mapName);
    MapData* createCustomMap(const std::string& name, const std::string& description);
    
    // World generation
    void generateFlatTerrain();
    void generateHillyTerrain(float amplitude = 2.0f, float frequency = 0.1f);
    void generateRandomFlowers(int count);
    
    // Lighting support (for future versions)
    struct Light {
        Vec3 position;
        Color color;
        float intensity;
        float radius;
        
        Light() : position(Vec3::zero()), color(Color::white()), intensity(1.0f), radius(10.0f) {}
        Light(const Vec3& pos, const Color& col, float intensity, float radius)
            : position(pos), color(col), intensity(intensity), radius(radius) {}
    };
    
    void addLight(const Light& light);
    void removeLight(size_t index);
    std::vector<Light>& getLights() { return lights; }
    const std::vector<Light>& getLights() const { return lights; }
    Color calculateLightingAt(const Vec3& position) const;
    
    // Grid to world coordinate conversion
    Vec3 gridToWorld(int x, int z) const;
    GridPos worldToGrid(const Vec3& worldPos) const;
    
    // Physics helpers for movement on slopes
    Vec3 projectVelocityOntoSurface(const Vec3& velocity, const Vec3& surfaceNormal) const;
    Vec3 getSlopeDirection(const Vec3& surfaceNormal) const;
    float getSlopeAngle(const Vec3& surfaceNormal) const;

private:
    int width;
    int height;
    std::vector<TerrainCell> cells;
    std::vector<Entity*> entities;
    std::vector<Light> lights;
    
    // Prefabricated maps storage
    std::map<std::string, MapData> prefabricatedMaps;
    
    // Helper for array indexing
    int cellIndex(int x, int z) const {
        return z * width + x;
    }
};
