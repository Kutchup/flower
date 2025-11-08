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
    
    // Calculate wind effect - multiple sine waves for natural motion
    float windBase = std::sin(animationTime * 2.0f) * 0.1f;
    float windDetail = std::sin(animationTime * 5.0f) * 0.02f;
    float sway = windBase + windDetail;
    
    // Add variation based on limb position for more organic look
    float positionVariation = std::sin(position.x * 0.5f + position.z * 0.3f) * 0.05f;
    sway += positionVariation;
    
    // Apply different animation styles based on limb type
    switch (type) {
        case Type::PETAL:
            // Petals rotate and slightly bob
            rotation.y = sway * 10.0f;
            rotation.x = std::cos(animationTime * 1.5f) * 3.0f;
            
            // Subtle size pulsing (breathing effect)
            {
                float pulse = 1.0f + std::sin(animationTime * 2.0f) * 0.05f;
                // This would scale the petal (applied in rendering)
            }
            break;
            
        case Type::STEM:
            {
                // Stems sway side to side with more pronounced movement at the top
                // Calculate distance from base (parent position)
                float heightFactor = (position.y - parentPosition.y) * 2.0f;
                rotation.x = sway * 5.0f * (1.0f + heightFactor);
                rotation.z = std::cos(animationTime * 1.8f) * 3.0f * (1.0f + heightFactor);
            }
            break;
            
        case Type::LEAF:
            // Leaves flutter more dramatically
            rotation.z = sway * 15.0f;
            rotation.y = std::cos(animationTime * 3.0f) * 8.0f;
            
            // Leaves can fold slightly
            rotation.x = std::sin(animationTime * 4.0f) * 5.0f;
            break;
    }
    
    // Apply damping to prevent extreme rotations
    const float maxRotation = 45.0f;  // Maximum rotation in degrees
    rotation.x = std::max(-maxRotation, std::min(rotation.x, maxRotation));
    rotation.y = std::max(-maxRotation, std::min(rotation.y, maxRotation));
    rotation.z = std::max(-maxRotation, std::min(rotation.z, maxRotation));
}

void Limb::animate(float amount) {
    // Trigger a special animation (e.g., when flower is watered)
    // This causes an excited movement
    animationTime += amount * 5.0f;  // Speed up animation temporarily
    
    // Add an impulse to the rotation based on type
    switch (type) {
        case Type::PETAL:
            // Petals spin slightly when excited
            rotation.y += amount * 20.0f;
            break;
        case Type::STEM:
            // Stems bounce
            rotation.x += amount * 15.0f;
            break;
        case Type::LEAF:
            // Leaves flutter rapidly
            rotation.z += amount * 25.0f;
            break;
    }
}

void Limb::setWindStrength(float strength) {
    // This could be used to simulate changing wind conditions
    // Store wind strength for use in update
    // For now, it affects the sway amplitude
    // Implementation would modify the sway calculation in update()
}

Vec3 Limb::getWorldPosition() const {
    // Calculate the actual world position considering parent
    // and any offset from animation
    Vec3 offset = position - parentPosition;
    
    // Apply rotation to offset (simplified - would use proper rotation matrix in full impl)
    // For now, return the base position
    return position;
}

Color Limb::getAnimatedColor() const {
    // Color could pulse or change based on animation state
    // For watered flowers, might glow slightly
    float pulse = 1.0f + std::sin(animationTime * 3.0f) * 0.1f;
    
    return Color(
        color.r * pulse,
        color.g * pulse,
        color.b * pulse,
        color.a
    );
}

float Limb::getAnimationProgress() const {
    // Return normalized animation progress (0-1) for the current cycle
    float cycleTime = 2.0f;  // Duration of one animation cycle
    return std::fmod(animationTime, cycleTime) / cycleTime;
}

void Limb::resetAnimation() {
    animationTime = 0.0f;
    rotation = Vec3(0, 0, 0);
}
