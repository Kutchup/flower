#pragma once

#include "math_utils.h"
#include <string>

// Base Entity class for all game objects in the world
// This provides a unified interface for objects that exist in 3D space
class Entity {
public:
    enum class Type {
        STATIC,      // Non-moving objects (trees, rocks, etc.)
        DYNAMIC,     // Moving objects (player, NPCs)
        INTERACTIVE, // Objects player can interact with
        DECORATIVE   // Visual-only objects
    };

    Entity();
    Entity(const Vec3& position, const Vec3& rotation, const Vec3& scale);
    virtual ~Entity();

    // Update entity state each frame
    virtual void update(float deltaTime);

    // Position and transformation
    Vec3 getPosition() const { return position; }
    Vec3 getRotation() const { return rotation; }
    Vec3 getScale() const { return scale; }
    
    void setPosition(const Vec3& pos);
    void setRotation(const Vec3& rot);
    void setScale(const Vec3& scl);
    
    // Movement
    void translate(const Vec3& offset);
    void rotate(const Vec3& anglesDeg);
    
    // Entity properties
    Type getType() const { return type; }
    void setType(Type t) { type = t; }
    
    std::string getName() const { return name; }
    void setName(const std::string& n) { name = n; }
    
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }
    
    bool isVisible() const { return visible; }
    void setVisible(bool v) { visible = v; }
    
    // Physics/collision
    Vec3 getVelocity() const { return velocity; }
    void setVelocity(const Vec3& vel) { velocity = vel; }
    
    float getMass() const { return mass; }
    void setMass(float m) { mass = m; }
    
    // Surface normal for slope handling
    Vec3 getSurfaceNormal() const { return surfaceNormal; }
    void setSurfaceNormal(const Vec3& normal) { surfaceNormal = normal; }
    
    // Bounding box for collision detection
    struct BoundingBox {
        Vec3 min;
        Vec3 max;
        
        BoundingBox() : min(Vec3::zero()), max(Vec3::zero()) {}
        BoundingBox(const Vec3& min, const Vec3& max) : min(min), max(max) {}
        
        bool contains(const Vec3& point) const {
            return point.x >= min.x && point.x <= max.x &&
                   point.y >= min.y && point.y <= max.y &&
                   point.z >= min.z && point.z <= max.z;
        }
        
        bool intersects(const BoundingBox& other) const {
            return !(max.x < other.min.x || min.x > other.max.x ||
                     max.y < other.min.y || min.y > other.max.y ||
                     max.z < other.min.z || min.z > other.max.z);
        }
    };
    
    BoundingBox getBoundingBox() const { return boundingBox; }
    void setBoundingBox(const BoundingBox& box) { boundingBox = box; }
    void updateBoundingBox();
    
    // Color for rendering
    Color getColor() const { return color; }
    void setColor(const Color& c) { color = c; }

protected:
    // Transform
    Vec3 position;
    Vec3 rotation;  // Euler angles in degrees
    Vec3 scale;
    
    // Physics
    Vec3 velocity;
    float mass;
    Vec3 surfaceNormal;  // Normal of surface entity is standing on
    
    // Properties
    Type type;
    std::string name;
    bool active;
    bool visible;
    
    // Collision
    BoundingBox boundingBox;
    
    // Rendering
    Color color;
};
