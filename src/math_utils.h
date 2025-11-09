#pragma once

#include <cmath>
#include <algorithm>

// Constants
const float PI = 3.14159265358979323846f;
const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

// Basic 3D vector math
struct Vec3 {
    float x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(float scalar) : x(scalar), y(scalar), z(scalar) {}
    
    // Arithmetic operators
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    
    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
    
    Vec3 operator/(float scalar) const {
        if (scalar != 0.0f) {
            return Vec3(x / scalar, y / scalar, z / scalar);
        }
        return Vec3(0, 0, 0);
    }
    
    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    Vec3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Vec3& operator/=(float scalar) {
        if (scalar != 0.0f) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
        }
        return *this;
    }
    
    // Comparison operators
    bool operator==(const Vec3& other) const {
        const float epsilon = 0.0001f;
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }
    
    bool operator!=(const Vec3& other) const {
        return !(*this == other);
    }
    
    // Vector operations
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    float lengthSquared() const {
        return x * x + y * y + z * z;
    }
    
    Vec3 normalized() const {
        float len = length();
        if (len > 0) {
            return Vec3(x / len, y / len, z / len);
        }
        return Vec3(0, 0, 0);
    }
    
    void normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
            z /= len;
        }
    }
    
    float distance(const Vec3& other) const {
        return (*this - other).length();
    }
    
    float distanceSquared(const Vec3& other) const {
        return (*this - other).lengthSquared();
    }
    
    // Static vector operations
    static Vec3 cross(const Vec3& a, const Vec3& b) {
        return Vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }
    
    static float dot(const Vec3& a, const Vec3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
    static Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
        t = std::max(0.0f, std::min(1.0f, t));  // Clamp t to [0, 1]
        return a + (b - a) * t;
    }
    
    static Vec3 min(const Vec3& a, const Vec3& b) {
        return Vec3(
            std::min(a.x, b.x),
            std::min(a.y, b.y),
            std::min(a.z, b.z)
        );
    }
    
    static Vec3 max(const Vec3& a, const Vec3& b) {
        return Vec3(
            std::max(a.x, b.x),
            std::max(a.y, b.y),
            std::max(a.z, b.z)
        );
    }
    
    static Vec3 reflect(const Vec3& v, const Vec3& normal) {
        return v - normal * (2.0f * dot(v, normal));
    }
    
    // Common vector constants
    static Vec3 zero() { return Vec3(0, 0, 0); }
    static Vec3 one() { return Vec3(1, 1, 1); }
    static Vec3 up() { return Vec3(0, 1, 0); }
    static Vec3 down() { return Vec3(0, -1, 0); }
    static Vec3 left() { return Vec3(-1, 0, 0); }
    static Vec3 right() { return Vec3(1, 0, 0); }
    static Vec3 forward() { return Vec3(0, 0, -1); }
    static Vec3 back() { return Vec3(0, 0, 1); }
};

// 2D grid position
struct GridPos {
    int x, z;
    
    GridPos() : x(0), z(0) {}
    GridPos(int x, int z) : x(x), z(z) {}
    
    bool operator==(const GridPos& other) const {
        return x == other.x && z == other.z;
    }
    
    bool operator!=(const GridPos& other) const {
        return !(*this == other);
    }
    
    bool operator<(const GridPos& other) const {
        if (x != other.x) return x < other.x;
        return z < other.z;
    }
    
    GridPos operator+(const GridPos& other) const {
        return GridPos(x + other.x, z + other.z);
    }
    
    GridPos operator-(const GridPos& other) const {
        return GridPos(x - other.x, z - other.z);
    }
    
    int manhattanDistance(const GridPos& other) const {
        return std::abs(x - other.x) + std::abs(z - other.z);
    }
    
    Vec3 toVec3(float y = 0.0f) const {
        return Vec3(static_cast<float>(x), y, static_cast<float>(z));
    }
};

// Color structure
struct Color {
    float r, g, b, a;
    
    Color() : r(1), g(1), b(1), a(1) {}
    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    
    Color operator*(float scalar) const {
        return Color(r * scalar, g * scalar, b * scalar, a);
    }
    
    Color operator+(const Color& other) const {
        return Color(
            std::min(r + other.r, 1.0f),
            std::min(g + other.g, 1.0f),
            std::min(b + other.b, 1.0f),
            std::min(a + other.a, 1.0f)
        );
    }
    
    static Color lerp(const Color& a, const Color& b, float t) {
        t = std::max(0.0f, std::min(1.0f, t));
        return Color(
            a.r + (b.r - a.r) * t,
            a.g + (b.g - a.g) * t,
            a.b + (b.b - a.b) * t,
            a.a + (b.a - a.a) * t
        );
    }
    
    // Common colors
    static Color white() { return Color(1, 1, 1); }
    static Color black() { return Color(0, 0, 0); }
    static Color red() { return Color(1, 0, 0); }
    static Color green() { return Color(0, 1, 0); }
    static Color blue() { return Color(0, 0, 1); }
    static Color yellow() { return Color(1, 1, 0); }
    static Color cyan() { return Color(0, 1, 1); }
    static Color magenta() { return Color(1, 0, 1); }
};

// Utility math functions
namespace MathUtils {
    inline float clamp(float value, float min, float max) {
        return std::max(min, std::min(value, max));
    }
    
    inline float lerp(float a, float b, float t) {
        return a + (b - a) * clamp(t, 0.0f, 1.0f);
    }
    
    inline float smoothstep(float edge0, float edge1, float x) {
        float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }
    
    inline float toRadians(float degrees) {
        return degrees * DEG_TO_RAD;
    }
    
    inline float toDegrees(float radians) {
        return radians * RAD_TO_DEG;
    }
}
