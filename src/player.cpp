#include "player.h"
#include <cmath>

Player::Player() 
    : position(0, 1.7f, 0)  // Eye height
    , forward(0, 0, -1)
    , right(1, 0, 0)
    , up(0, 1, 0)
    , yaw(-90.0f)  // Looking forward (negative Z)
    , pitch(0.0f)
    , movementSpeed(5.0f)
    , flowersPlanted(0)
    , flowersWatered(0)
    , photographsTaken(0)
{
    updateVectors();
}

Player::~Player() {
}

void Player::update(float deltaTime) {
    // Player update logic
}

void Player::moveForward(float amount) {
    position += forward * amount;
}

void Player::moveRight(float amount) {
    position += right * amount;
}

void Player::moveUp(float amount) {
    position.y += amount;
}

void Player::rotate(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw;
    pitch += deltaPitch;
    
    // Constrain pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    updateVectors();
}

void Player::setPosition(const Vec3& pos) {
    position = pos;
}

GridPos Player::getGridPosition() const {
    return GridPos(static_cast<int>(std::floor(position.x)), 
                   static_cast<int>(std::floor(position.z)));
}

void Player::updateVectors() {
    // Calculate forward vector from yaw and pitch
    float yawRad = yaw * 3.14159f / 180.0f;
    float pitchRad = pitch * 3.14159f / 180.0f;
    
    forward.x = std::cos(yawRad) * std::cos(pitchRad);
    forward.y = std::sin(pitchRad);
    forward.z = std::sin(yawRad) * std::cos(pitchRad);
    forward = forward.normalized();
    
    // Calculate right vector
    Vec3 worldUp(0, 1, 0);
    right = Vec3::cross(forward, worldUp).normalized();
    
    // Calculate up vector
    up = Vec3::cross(right, forward).normalized();
}
