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
    // Smooth movement dampening could be added here
    // Animation state updates
    // Stamina regeneration, etc.
}

void Player::moveForward(float amount) {
    // Move in the horizontal direction the player is looking (forward vector projected to XZ plane)
    // This keeps movement on the ground plane regardless of vertical look direction
    Vec3 horizontalForward = Vec3(forward.x, 0, forward.z).normalized();
    position += horizontalForward * amount;
    
    // Optional: Keep player above ground level
    if (position.y < 1.7f) {
        position.y = 1.7f;
    }
}

void Player::moveRight(float amount) {
    // Strafe movement perpendicular to forward direction on the horizontal plane
    // Right vector is already horizontal (perpendicular to forward and world up)
    position += right * amount;
    
    // Optional: Keep player above ground level
    if (position.y < 1.7f) {
        position.y = 1.7f;
    }
}

void Player::moveUp(float amount) {
    // Vertical movement (flying/jumping)
    position.y += amount;
    
    // Optional: Clamp to reasonable heights
    if (position.y < 0.5f) {
        position.y = 0.5f;  // Don't go below ground
    }
    if (position.y > 50.0f) {
        position.y = 50.0f;  // Don't fly too high
    }
}

void Player::rotate(float deltaYaw, float deltaPitch) {
    // Update yaw (horizontal rotation)
    yaw += deltaYaw;
    
    // Normalize yaw to [0, 360) range
    while (yaw >= 360.0f) yaw -= 360.0f;
    while (yaw < 0.0f) yaw += 360.0f;
    
    // Update pitch (vertical rotation)
    pitch += deltaPitch;
    
    // Constrain pitch to prevent camera flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    // Recalculate direction vectors based on new rotation
    updateVectors();
}

void Player::setPosition(const Vec3& pos) {
    position = pos;
    
    // Ensure player stays above minimum height
    if (position.y < 0.5f) {
        position.y = 0.5f;
    }
}

GridPos Player::getGridPosition() const {
    // Convert world position to grid coordinates
    // Floor function ensures we're in the correct grid cell
    return GridPos(static_cast<int>(std::floor(position.x)), 
                   static_cast<int>(std::floor(position.z)));
}

void Player::updateVectors() {
    // Convert angles from degrees to radians for trigonometric functions
    float yawRad = yaw * 3.14159f / 180.0f;
    float pitchRad = pitch * 3.14159f / 180.0f;
    
    // Calculate forward vector from yaw and pitch using spherical coordinates
    // This creates a direction vector the camera is pointing
    forward.x = std::cos(yawRad) * std::cos(pitchRad);
    forward.y = std::sin(pitchRad);
    forward.z = std::sin(yawRad) * std::cos(pitchRad);
    forward = forward.normalized();
    
    // Calculate right vector (perpendicular to forward, in XZ plane)
    // This is used for strafing movement
    Vec3 worldUp(0, 1, 0);
    right = Vec3::cross(forward, worldUp).normalized();
    
    // Calculate up vector (perpendicular to both forward and right)
    // This completes our camera coordinate system
    up = Vec3::cross(right, forward).normalized();
}

Vec3 Player::getViewDirection() const {
    // Return the direction the player is looking (same as forward)
    return forward;
}

Vec3 Player::getTargetPosition(float distance) const {
    // Calculate a point at a specific distance in front of the player
    // Useful for raycasting, placing objects, etc.
    return position + forward * distance;
}

bool Player::isLookingAt(const Vec3& targetPos, float maxAngle) const {
    // Check if player is looking at a specific position
    Vec3 toTarget = (targetPos - position).normalized();
    float dotProduct = Vec3::dot(forward, toTarget);
    
    // Convert max angle to cosine for comparison
    float maxCosAngle = std::cos(maxAngle * 3.14159f / 180.0f);
    
    return dotProduct >= maxCosAngle;
}

float Player::getDistanceToPosition(const Vec3& pos) const {
    // Calculate distance from player to a world position
    return position.distance(pos);
}

void Player::lookAt(const Vec3& targetPos) {
    // Calculate direction to target
    Vec3 direction = (targetPos - position).normalized();
    
    // Calculate yaw from direction
    yaw = std::atan2(direction.z, direction.x) * 180.0f / 3.14159f;
    
    // Calculate pitch from direction
    float horizontalDist = std::sqrt(direction.x * direction.x + direction.z * direction.z);
    pitch = std::atan2(direction.y, horizontalDist) * 180.0f / 3.14159f;
    
    // Clamp pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    // Update vectors
    updateVectors();
}

void Player::setMovementSpeed(float speed) {
    movementSpeed = speed;
    if (movementSpeed < 0.1f) {
        movementSpeed = 0.1f;  // Minimum speed
    }
    if (movementSpeed > 20.0f) {
        movementSpeed = 20.0f;  // Maximum speed
    }
}

float Player::getMovementSpeed() const {
    return movementSpeed;
}
