#pragma once

#include "FireMode.hpp"

class SingleShot : public FireMode {
  public:
    void shoot(Map& map, Player& owner, int damage) override;
};