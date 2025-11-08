#pragma once

#include "math_utils.h"

// Limb represents animated parts like flower petals, stems, leaves
// These can sway in the wind or animate when interacted with
class Limb {
public:
    enum class Type {
        PETAL,
        STEM,
        LEAF
    };
    
    Limb(const Vec3& position, Type type, const Vec3& parentPosition);
    ~Limb();
    
    void update(float deltaTime);
    void animate(float amount);
    
    Vec3 getPosition() const { return position; }
    Type getType() const { return type; }
    Color getColor() const { return color; }
    float getSize() const { return size; }
    
    Vec3 getRotation() const { return rotation; }
    
private:
    Vec3 position;
    Vec3 parentPosition;
    Vec3 rotation;
    Type type;
    Color color;
    float size;
    float animationTime;
};
