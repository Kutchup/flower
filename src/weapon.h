#pragma once

#include "math_utils.h"
#include <string>

// Weapon represents peaceful tools like watering can, camera, seed planter
class Weapon {
public:
    enum class Type {
        WATERING_CAN,
        SEED_PLANTER,
        CAMERA
    };
    
    Weapon(const Vec3& position, Type type);
    ~Weapon();
    
    void update(float deltaTime);
    void use();
    
    Vec3 getPosition() const { return position; }
    Type getType() const { return type; }
    Color getColor() const { return color; }
    std::string getName() const;
    
    bool isActive() const { return active; }
    void setActive(bool active) { this->active = active; }
    
private:
    Vec3 position;
    Type type;
    Color color;
    bool active;
    float cooldown;
};
