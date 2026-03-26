#pragma once
#include <cmath>

struct Vector2 {
    float x = 0.0f;
    float y = 0.0f;

    /**
     * @brief Compute the Euclidean length of the vector.
     * @return The vector magnitude.
     */
    float Length() const {
        return std::sqrt(x * x + y * y);
    }

    /**
     * @brief Normalize the vector in place when it has non-zero length.
     */
    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
        }
    }

    /**
     * @brief Add another vector to this vector and return the result.
     * @param other Vector added component-wise.
     * @return The component-wise sum.
     */
    Vector2 operator+(const Vector2& other) const {
        return Vector2{x + other.x, y + other.y};
	}

    /**
     * @brief Subtract another vector from this vector and return the result.
     * @param other Vector subtracted component-wise.
     * @return The component-wise difference.
     */
    Vector2 operator-(const Vector2& other) const {
        return Vector2{ x - other.x, y - other.y };
    }

    /**
     * @brief Multiply the vector by a scalar and return the result.
     * @param scalar Scalar multiplier applied to both components.
     * @return The scaled vector.
     */
    Vector2 operator*(float scalar) const {
        return Vector2{x * scalar, y * scalar};
	}
    /**
     * @brief Add another vector to this vector in place.
     * @param other Vector added component-wise.
     * @return A reference to this vector after modification.
     */
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
	}
};
