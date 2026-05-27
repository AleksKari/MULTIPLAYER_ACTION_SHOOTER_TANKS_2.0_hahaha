#pragma once

#include <math/Vec2.h>

#include <Player.hpp>
#include <map/Map.hpp>

const float VELOCITY_OF_PROJECTILE = 800.F;
const int CENTER_OF_PLAYER = 16;
const int SIZE_OF_PROJECTILE = 6;

class FireMode {
 public:
  virtual ~FireMode() = default;
  virtual void Shoot(Map& map, Player& owner, int damage) = 0;
};