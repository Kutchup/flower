#pragma once

#include "math_utils.h"
#include <string>

// Pickup represents collectible items like seeds
class Pickup {
public:
    enum class Type {
        SUNFLOWER_SEEDS,
        ROSE_SEEDS,
        TULIP_SEEDS,
        DAISY_SEEDS,
        LAVENDER_SEEDS,
        WATER_BUCKET
    };
    
    Pickup(const Vec3& position, Type type);
    ~Pickup();
    
    void update(float deltaTime);
    
    Vec3 getPosition() const { return position; }
    Type getType() const { return type; }
    Color getColor() const { return color; }
    std::string getName() const;
    
private:
    Vec3 position;
    Type type;
    Color color;
};
