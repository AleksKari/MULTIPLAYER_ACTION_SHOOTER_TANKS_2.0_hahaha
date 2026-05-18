#pragma once
#include "Weapon.hpp"
#include "Firemodes/RicochetShot.hpp"
#include <Entity.hpp>
#include <map/Map.hpp>

class RicochetGun : public Weapon {
 private:
  double cooldown_ = 1;
  RicochetShot fire_;
 public:
  RicochetGun() {
    damage_ = 10;
    shots_left_ = 5;
  }
  void shoot(Map& map, Player& owner, int damage) override {
    if (shot_clock_.getElapsedTime().asSeconds() < cooldown_) return;
    shot_clock_.restart();
    fire_.shoot(map, owner, damage);
    --shots_left_;
  }
};