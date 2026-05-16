#pragma once
#include "weapon.hpp"
#include "Firemodes/ShotgunFire.hpp"
#include <entity.hpp>
#include <map/map.hpp>

class ShotGun : public  Weapon {
 private:
  int damage_ = 0;
  double cooldown = 1;
  ShotgunFire fire_;
 public:
  ShotGun() {
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