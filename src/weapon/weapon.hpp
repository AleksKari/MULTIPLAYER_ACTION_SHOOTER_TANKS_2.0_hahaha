#pragma once
#include "../math/Vec2.h"

class Weapon {
  public:
    virtual ~Weapon() = default;
    virtual void shoot(const Vec2& pos, const Vec2& dir) = 0;
    virtual double getcooldown() const = 0;
};
