#pragma once

#include "FireMode.hpp"

class RicochetShot : public FireMode {
  public:
    void shoot(Map& map, Player& owner, int damage) override;
};