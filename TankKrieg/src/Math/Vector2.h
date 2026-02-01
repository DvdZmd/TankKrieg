#pragma once
#include <cmath>

struct Vector2 {
    float x = 0.0f;
    float y = 0.0f;

    float Length() const {
        return std::sqrt(x * x + y * y);
    }

    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
        }
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2{x + other.x, y + other.y};
	}

    Vector2 operator-(const Vector2& other) const {
        return Vector2{ x - other.x, y - other.y };
    }

    Vector2 operator*(float scalar) const {
        return Vector2{x * scalar, y * scalar};
	}
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
	}
};
