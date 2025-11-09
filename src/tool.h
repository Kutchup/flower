#pragma once

#include "math_utils.h"
#include <string>

// Tool represents peaceful implements like watering can, camera, seed planter
// These are the implements used to beautify the world with flowers
class Tool {
public:
    enum class Type {
        WATERING_CAN,
        SEED_PLANTER,
        CAMERA,
        TROWEL,
        FERTILIZER_SPREADER
    };
    
    Tool(const Vec3& position, Type type);
    ~Tool();
    
    void update(float deltaTime);
    void use();
    void reset();
    
    Vec3 getPosition() const { return position; }
    void setPosition(const Vec3& pos) { position = pos; }
    
    Type getType() const { return type; }
    Color getColor() const { return color; }
    std::string getName() const;
    std::string getDescription() const;
    
    bool isActive() const { return active; }
    void setActive(bool active) { this->active = active; }
    
    bool canUse() const { return cooldown <= 0.0f; }
    float getCooldownProgress() const;
    
    // Tool-specific properties
    int getUseCount() const { return useCount; }
    float getEfficiency() const { return efficiency; }
    void upgradeEfficiency(float amount);
    
private:
    Vec3 position;
    Type type;
    Color color;
    bool active;
    float cooldown;
    float maxCooldown;
    int useCount;
    float efficiency;
};
