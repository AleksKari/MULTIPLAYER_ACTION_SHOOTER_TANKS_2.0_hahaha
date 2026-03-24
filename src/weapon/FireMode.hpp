#pragma once
#include "../math/Vec2.h"
#include "WeaponStates.hpp"

class FireMode {
  public:
    virtual ~FireMode() = default;
    virtual void fire(const WeaponStates& stats,
                      const Vec2& pos,
                      const Vec2& dir) = 0;
};