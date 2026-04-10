#pragma once

#include "../math/Vec2.h"
#include "../map/map.hpp"
#include "../../libraries/player.hpp"

class FireMode {
  public:
    virtual ~FireMode() = default;
    virtual void shoot(Map& map, Player& owner, int damage) = 0;
};