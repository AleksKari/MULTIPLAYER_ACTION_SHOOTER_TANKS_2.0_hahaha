#pragma once

#include <weapon/Firemodes/FireMode.hpp>

class ShotgunFire : public FireMode {
 public:
  void shoot(Map& map, Player& owner, int damage) override;
};
