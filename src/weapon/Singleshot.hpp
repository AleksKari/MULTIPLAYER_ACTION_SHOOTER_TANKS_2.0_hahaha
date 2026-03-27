#pragma once

#include "FireMode.hpp"

class SingleShot : public FireMode {
  public:
    void fire(const WeaponStates& states, const Vec2& pos, const Vec2& dir) override;
};