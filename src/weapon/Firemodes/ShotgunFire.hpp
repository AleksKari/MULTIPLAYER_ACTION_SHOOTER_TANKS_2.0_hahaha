#pragma once

#include "FireMode.hpp"

class ShotgunFire : public FireMode {
  public:
    void shoot(Map& map, Player& owner, int damage) override;
};
