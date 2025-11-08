#include "limb.h"
#include <cmath>

Limb::Limb(const Vec3& position, Type type, const Vec3& parentPosition)
    : position(position)
    , parentPosition(parentPosition)
    , rotation(0, 0, 0)
    , type(type)
    , size(0.1f)
    , animationTime(0.0f)
{
    // Set properties based on type
    switch (type) {
        case Type::PETAL:
            color = Color(1.0f, 0.5f, 0.7f);  // Pink
            size = 0.15f;
            break;
        case Type::STEM:
            color = Color(0.2f, 0.6f, 0.2f);  // Green
            size = 0.05f;
            break;
        case Type::LEAF:
            color = Color(0.3f, 0.7f, 0.3f);  // Light green
            size = 0.1f;
            break;
    }
}

Limb::~Limb() {
}

void Limb::update(float deltaTime) {
    animationTime += deltaTime;
    
    // Gentle swaying animation like wind
    float sway = std::sin(animationTime * 2.0f) * 0.1f;
    
    switch (type) {
        case Type::PETAL:
            // Petals rotate slightly
            rotation.y = sway * 10.0f;
            break;
        case Type::STEM:
            // Stems sway side to side
            rotation.x = sway * 5.0f;
            break;
        case Type::LEAF:
            // Leaves flutter
            rotation.z = sway * 15.0f;
            break;
    }
}

void Limb::animate(float amount) {
    // Trigger a special animation (e.g., when flower is watered)
    animationTime += amount;
}
