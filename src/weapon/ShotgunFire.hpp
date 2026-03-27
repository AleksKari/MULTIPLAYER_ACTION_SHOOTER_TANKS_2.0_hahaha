#pragma once

#include "FireMode.hpp"

class ShotgunFire : public FireMode {
    void fire(const WeaponStates& states, const Vec2& pos, const Vec2& dir) override;
};
