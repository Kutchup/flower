#include "tool.h"

Tool::Tool(const Vec3& position, Type type)
    : position(position)
    , type(type)
    , active(false)
    , cooldown(0.0f)
    , maxCooldown(0.5f)
    , useCount(0)
    , efficiency(1.0f)
{
    // Set color and properties based on type
    switch (type) {
        case Type::WATERING_CAN:
            color = Color(0.4f, 0.6f, 0.9f);  // Blue
            maxCooldown = 0.3f;  // Quick watering
            break;
        case Type::SEED_PLANTER:
            color = Color(0.6f, 0.4f, 0.2f);  // Brown
            maxCooldown = 0.5f;  // Moderate planting speed
            break;
        case Type::CAMERA:
            color = Color(0.2f, 0.2f, 0.2f);  // Black
            maxCooldown = 1.0f;  // Slower photography
            break;
        case Type::TROWEL:
            color = Color(0.7f, 0.7f, 0.7f);  // Silver
            maxCooldown = 0.4f;  // Quick digging
            break;
        case Type::FERTILIZER_SPREADER:
            color = Color(0.8f, 0.6f, 0.3f);  // Golden
            maxCooldown = 0.6f;  // Moderate spreading
            break;
    }
}

Tool::~Tool() {
}

void Tool::update(float deltaTime) {
    if (cooldown > 0) {
        cooldown -= deltaTime;
        if (cooldown < 0) {
            cooldown = 0;
        }
    }
    
    // Deactivate after a short time
    if (active) {
        static float activeTime = 0.0f;
        activeTime += deltaTime;
        if (activeTime > 0.2f) {
            active = false;
            activeTime = 0.0f;
        }
    }
}

void Tool::use() {
    if (canUse()) {
        active = true;
        cooldown = maxCooldown / efficiency;  // Efficiency reduces cooldown
        useCount++;
    }
}

void Tool::reset() {
    active = false;
    cooldown = 0.0f;
    useCount = 0;
}

float Tool::getCooldownProgress() const {
    if (maxCooldown <= 0) return 1.0f;
    return 1.0f - (cooldown / maxCooldown);
}

void Tool::upgradeEfficiency(float amount) {
    efficiency += amount;
    if (efficiency > 3.0f) {
        efficiency = 3.0f;  // Cap at 3x efficiency
    }
}

std::string Tool::getName() const {
    switch (type) {
        case Type::WATERING_CAN:
            return "Watering Can";
        case Type::SEED_PLANTER:
            return "Seed Planter";
        case Type::CAMERA:
            return "Camera";
        case Type::TROWEL:
            return "Trowel";
        case Type::FERTILIZER_SPREADER:
            return "Fertilizer Spreader";
        default:
            return "Unknown Tool";
    }
}

std::string Tool::getDescription() const {
    switch (type) {
        case Type::WATERING_CAN:
            return "Waters flowers to help them grow and flourish";
        case Type::SEED_PLANTER:
            return "Plants seeds to create beautiful flowers";
        case Type::CAMERA:
            return "Captures the beauty of your garden for eternity";
        case Type::TROWEL:
            return "Prepares the soil for planting new flowers";
        case Type::FERTILIZER_SPREADER:
            return "Enriches the soil to boost flower growth";
        default:
            return "A mysterious gardening implement";
    }
}
