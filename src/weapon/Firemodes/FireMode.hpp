#pragma once

#include <math/Vec2.h>
#include <map/Map.hpp>
#include <Player.hpp>

class FireMode {
 public:
  virtual ~FireMode() = default;
  virtual void shoot(Map& map, Player& owner, int damage) = 0;
};