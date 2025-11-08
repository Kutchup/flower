#include "pickup.h"

Pickup::Pickup(const Vec3& position, Type type)
    : position(position)
    , type(type)
{
    // Set color based on type
    switch (type) {
        case Type::SUNFLOWER_SEEDS:
            color = Color(1.0f, 0.8f, 0.0f);  // Yellow
            break;
        case Type::ROSE_SEEDS:
            color = Color(1.0f, 0.2f, 0.3f);  // Red
            break;
        case Type::TULIP_SEEDS:
            color = Color(1.0f, 0.4f, 0.6f);  // Pink
            break;
        case Type::DAISY_SEEDS:
            color = Color(0.9f, 0.9f, 1.0f);  // White
            break;
        case Type::LAVENDER_SEEDS:
            color = Color(0.6f, 0.3f, 0.9f);  // Purple
            break;
        case Type::WATER_BUCKET:
            color = Color(0.4f, 0.6f, 0.9f);  // Blue
            break;
    }
}

Pickup::~Pickup() {
}

void Pickup::update(float deltaTime) {
    // Pickups can have idle animations, bobbing, etc.
}

std::string Pickup::getName() const {
    switch (type) {
        case Type::SUNFLOWER_SEEDS:
            return "Sunflower Seeds";
        case Type::ROSE_SEEDS:
            return "Rose Seeds";
        case Type::TULIP_SEEDS:
            return "Tulip Seeds";
        case Type::DAISY_SEEDS:
            return "Daisy Seeds";
        case Type::LAVENDER_SEEDS:
            return "Lavender Seeds";
        case Type::WATER_BUCKET:
            return "Water Bucket";
        default:
            return "Unknown Item";
    }
}
