#pragma once

#include "math_utils.h"

// Limb represents animated parts like flower petals, stems, leaves
// These components create living, breathing flowers that respond to wind
// and player interactions, making the garden feel alive and dynamic
class Limb {
public:
    enum class Type {
        PETAL,   // Flower petals that rotate and bob gently
        STEM,    // Stems that sway in the breeze
        LEAF     // Leaves that flutter and fold
    };
    
    Limb(const Vec3& position, Type type, const Vec3& parentPosition);
    ~Limb();
    
    // Update animation state based on wind and time
    void update(float deltaTime);
    
    // Trigger a special animation (e.g., when flower is watered)
    void animate(float amount);
    
    // Reset animation to initial state
    void resetAnimation();
    
    // Position and visual properties
    Vec3 getPosition() const { return position; }
    Vec3 getWorldPosition() const;
    void setPosition(const Vec3& pos) { position = pos; }
    
    Type getType() const { return type; }
    Color getColor() const { return color; }
    Color getAnimatedColor() const;
    float getSize() const { return size; }
    
    // Animation state
    Vec3 getRotation() const { return rotation; }
    float getAnimationProgress() const;
    
    // Environmental effects
    void setWindStrength(float strength);
    
private:
    Vec3 position;        // Local position relative to parent
    Vec3 parentPosition;  // Position of parent flower
    Vec3 rotation;        // Current rotation angles (degrees)
    Type type;            // What kind of limb this is
    Color color;          // Base color
    float size;           // Size/scale of the limb
    float animationTime;  // Time accumulator for animations
};
