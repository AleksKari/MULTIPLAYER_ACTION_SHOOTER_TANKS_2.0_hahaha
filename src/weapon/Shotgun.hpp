#pragma once
#include "Weapon.hpp"
#include "Firemodes/ShotgunFire.hpp"
#include <Entity.hpp>
#include <map/Map.hpp>

class Shotgun : public Weapon {
 private:
  int damage_ = 0;
  double cooldown_ = 1;
  ShotgunFire fire_;
 public:
  Shotgun() {
    damage_ = 10;
    shots_left_ = 5;
  }
  void shoot(Map& map, Player& owner, int damage) override {
    if (shot_clock_.getElapsedTime().asSeconds() < cooldown_) return;
    shot_clock_.restart();
    fire_.shoot(map, owner, damage);
    shots_left_--;
  }
};