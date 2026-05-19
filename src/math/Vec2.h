#pragma once

#include <cmath>

struct Vec2 {
 public:
  Vec2() : cord_x(0), cord_y(0) {}
  Vec2(double cord_x, double cord_y) : cord_x(cord_x), cord_y(cord_y) {}
  explicit Vec2(double angle) {
    cord_x = std::cos(angle);
    cord_y = std::sin(angle);
  }

  // Сложение
  Vec2 operator+(const Vec2& other) const {
    return Vec2(cord_x + other.cord_x, cord_y + other.cord_y);
  }
  // Вычитание
  Vec2 operator-(const Vec2& other) const {
    return Vec2(cord_x - other.cord_x, cord_y - other.cord_y);
  }
  Vec2 operator*(double scalar) const {
    return Vec2(cord_x * scalar, cord_y * scalar);
  }
  double length() const {
    return std::sqrt((cord_x * cord_x) + (cord_y * cord_y));
  }
  Vec2 normalized() const {
    double len = length();
    if (len == 0) {
      return Vec2(0, 0);
    }
    return Vec2(cord_x / len, cord_y / len);
  }
  double angle() const { return std::atan2(cord_y, cord_x); }
  Vec2 rotate(double angle) const {
    return Vec2((cord_x * cos(angle)) + (cord_y * sin(angle)),
                (cord_y * cos(angle)) - (cord_x * sin(angle)));
  }
  double cord_x;
  double cord_y;
};