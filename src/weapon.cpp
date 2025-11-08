#include "weapon.h"

Weapon::Weapon(const Vec3& position, Type type)
    : position(position)
    , type(type)
    , active(false)
    , cooldown(0.0f)
{
    // Set color based on type
    switch (type) {
        case Type::WATERING_CAN:
            color = Color(0.4f, 0.6f, 0.9f);  // Blue
            break;
        case Type::SEED_PLANTER:
            color = Color(0.6f, 0.4f, 0.2f);  // Brown
            break;
        case Type::CAMERA:
            color = Color(0.2f, 0.2f, 0.2f);  // Black
            break;
    }
}

Weapon::~Weapon() {
}

void Weapon::update(float deltaTime) {
    if (cooldown > 0) {
        cooldown -= deltaTime;
    }
}

void Weapon::use() {
    if (cooldown <= 0) {
        active = true;
        cooldown = 0.5f;  // Half second cooldown
    }
}

std::string Weapon::getName() const {
    switch (type) {
        case Type::WATERING_CAN:
            return "Watering Can";
        case Type::SEED_PLANTER:
            return "Seed Planter";
        case Type::CAMERA:
            return "Camera";
        default:
            return "Unknown Tool";
    }
}
