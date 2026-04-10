#pragma once

#include <cmath>

class Vec2 {
  public:
    Vec2() : x(0), y(0) {}
    Vec2(double x, double y): x(x), y(y) {}
    Vec2(double angle) {
        x = std::cos(angle);
        y = std::sin(angle);
    }

    // Сложение
    Vec2 operator+(const Vec2& other) {
        return Vec2(x + other.x, y + other.y);
    }
    // Вычитание
    Vec2 operator-(const Vec2& other) {
        return Vec2(x - other.x, y - other.y);
    }
    Vec2 operator*(double scalar) {
        return Vec2(x * scalar, y * scalar);
    }
    double length() const {
        return std::sqrt(x * x + y * y);
    }
    Vec2 normilized() const {
        double len = length();
        if (len == 0) return Vec2(0, 0);
        return Vec2(x / len, y / len);
    }
    double angle() const {
        return std::atan2(y, x);
    }
    Vec2 rotate(double angle) {
        return Vec2(x * cos(angle) + y * sin(angle), y * cos(angle) - x * sin(angle));
    }
    double x;
    double y;
};