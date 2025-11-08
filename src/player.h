#pragma once

#include "math_utils.h"

// Player represents the first-person character
class Player {
public:
    Player();
    ~Player();
    
    // Update player state
    void update(float deltaTime);
    
    // Movement controls
    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);
    
    // Camera controls
    void rotate(float yaw, float pitch);
    
    // Position and orientation
    Vec3 getPosition() const { return position; }
    Vec3 getForward() const { return forward; }
    Vec3 getRight() const { return right; }
    Vec3 getUp() const { return up; }
    
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    
    void setPosition(const Vec3& pos);
    GridPos getGridPosition() const;
    
    // Stats
    int getFlowersPlanted() const { return flowersPlanted; }
    int getFlowersWatered() const { return flowersWatered; }
    int getPhotographsTaken() const { return photographsTaken; }
    
    void incrementFlowersPlanted() { flowersPlanted++; }
    void incrementFlowersWatered() { flowersWatered++; }
    void incrementPhotographsTaken() { photographsTaken++; }
    
private:
    void updateVectors();
    
    Vec3 position;
    Vec3 forward;
    Vec3 right;
    Vec3 up;
    
    float yaw;
    float pitch;
    float movementSpeed;
    
    // Statistics
    int flowersPlanted;
    int flowersWatered;
    int photographsTaken;
};
