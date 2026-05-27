#pragma once

#include <weapon/Firemodes/FireMode.hpp>

class ShotgunFire : public FireMode {
 public:
  void Shoot(Map& map, Player& owner, int damage) override;
};
