#include "entity.h"
#include <algorithm>

Entity::Entity()
    : position(Vec3::zero())
    , rotation(Vec3::zero())
    , scale(Vec3::one())
    , velocity(Vec3::zero())
    , mass(1.0f)
    , surfaceNormal(Vec3::up())
    , type(Type::STATIC)
    , name("Entity")
    , active(true)
    , visible(true)
    , color(Color::white())
{
    updateBoundingBox();
}

Entity::Entity(const Vec3& position, const Vec3& rotation, const Vec3& scale)
    : position(position)
    , rotation(rotation)
    , scale(scale)
    , velocity(Vec3::zero())
    , mass(1.0f)
    , surfaceNormal(Vec3::up())
    , type(Type::STATIC)
    , name("Entity")
    , active(true)
    , visible(true)
    , color(Color::white())
{
    updateBoundingBox();
}

Entity::~Entity() {
}

void Entity::update(float deltaTime) {
    // Base entity update - override in derived classes for specific behavior
    
    // Apply velocity if dynamic
    if (type == Type::DYNAMIC && active) {
        position += velocity * deltaTime;
        updateBoundingBox();
    }
}

void Entity::setPosition(const Vec3& pos) {
    position = pos;
    updateBoundingBox();
}

void Entity::setRotation(const Vec3& rot) {
    rotation = rot;
    
    // Normalize rotation angles to [0, 360) range
    rotation.x = std::fmod(rotation.x, 360.0f);
    rotation.y = std::fmod(rotation.y, 360.0f);
    rotation.z = std::fmod(rotation.z, 360.0f);
    
    if (rotation.x < 0) rotation.x += 360.0f;
    if (rotation.y < 0) rotation.y += 360.0f;
    if (rotation.z < 0) rotation.z += 360.0f;
}

void Entity::setScale(const Vec3& scl) {
    scale = scl;
    
    // Prevent negative or zero scale
    scale.x = std::max(0.01f, scale.x);
    scale.y = std::max(0.01f, scale.y);
    scale.z = std::max(0.01f, scale.z);
    
    updateBoundingBox();
}

void Entity::translate(const Vec3& offset) {
    position += offset;
    updateBoundingBox();
}

void Entity::rotate(const Vec3& anglesDeg) {
    rotation += anglesDeg;
    setRotation(rotation);  // Normalize angles
}

void Entity::setVelocity(const Vec3& vel) {
    velocity = vel;
}

void Entity::setMass(float m) {
    mass = std::max(0.001f, m);  // Prevent zero or negative mass
}

void Entity::setSurfaceNormal(const Vec3& normal) {
    // Ensure the surface normal is normalized
    surfaceNormal = normal.normalized();
    
    // If normalization fails (zero vector), default to up
    if (surfaceNormal.length() < 0.01f) {
        surfaceNormal = Vec3::up();
    }
}

void Entity::updateBoundingBox() {
    // Create bounding box based on scale
    // Default unit cube centered at origin
    Vec3 halfExtents = scale * 0.5f;
    
    boundingBox.min = position - halfExtents;
    boundingBox.max = position + halfExtents;
}
