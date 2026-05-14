#pragma once
#include "weapon.hpp"
#include "Firemodes/RicochetShot.hpp"
#include "../../libraries/entity.hpp"
#include "../map/map.hpp"

class RicochetGun : public Weapon {
 private:
  double cooldown = 1;
  RicochetShot fire_;
 public:
  RicochetGun() {
    damage_ = 10;
    shots_left_ = 5;
  }
  void shoot(Map& map, Player& owner, int damage) override {
    if (shot_clock.getElapsedTime().asSeconds() < cooldown) return;
    shot_clock.restart();
    fire_.shoot(map, owner, damage);
    shots_left_--;
  }
};