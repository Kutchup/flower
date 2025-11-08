#pragma once

#include "math_utils.h"

// Player represents the first-person character who explores the world
// and interacts with the garden using peaceful tools
class Player {
public:
    Player();
    ~Player();
    
    // Update player state each frame
    void update(float deltaTime);
    
    // Movement controls - allows the player to explore the garden world
    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);
    
    // Camera controls - first-person view rotation
    void rotate(float yaw, float pitch);
    void lookAt(const Vec3& targetPos);
    
    // Position and orientation accessors
    Vec3 getPosition() const { return position; }
    Vec3 getForward() const { return forward; }
    Vec3 getRight() const { return right; }
    Vec3 getUp() const { return up; }
    Vec3 getViewDirection() const;
    
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    
    void setPosition(const Vec3& pos);
    GridPos getGridPosition() const;
    
    // Utility methods for interaction
    Vec3 getTargetPosition(float distance) const;
    bool isLookingAt(const Vec3& targetPos, float maxAngle = 30.0f) const;
    float getDistanceToPosition(const Vec3& pos) const;
    
    // Movement speed control
    float getMovementSpeed() const;
    void setMovementSpeed(float speed);
    
    // Statistics tracking - player's gardening accomplishments
    int getFlowersPlanted() const { return flowersPlanted; }
    int getFlowersWatered() const { return flowersWatered; }
    int getPhotographsTaken() const { return photographsTaken; }
    
    void incrementFlowersPlanted() { flowersPlanted++; }
    void incrementFlowersWatered() { flowersWatered++; }
    void incrementPhotographsTaken() { photographsTaken++; }
    
    // Reset statistics
    void resetStatistics() {
        flowersPlanted = 0;
        flowersWatered = 0;
        photographsTaken = 0;
    }
    
private:
    void updateVectors();
    
    // Position and orientation in 3D space
    Vec3 position;
    Vec3 forward;
    Vec3 right;
    Vec3 up;
    
    // Camera rotation angles (in degrees)
    float yaw;    // Horizontal rotation
    float pitch;  // Vertical rotation
    
    // Movement properties
    float movementSpeed;
    
    // Player statistics - achievements in the garden
    int flowersPlanted;
    int flowersWatered;
    int photographsTaken;
};
