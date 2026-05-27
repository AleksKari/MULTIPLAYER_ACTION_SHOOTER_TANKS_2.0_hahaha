#pragma once
#include <Entity.hpp>
#include <map/Map.hpp>
#include <weapon/Firemodes/ShotgunFire.hpp>
#include <weapon/Weapon.hpp>

class Shotgun : public Weapon {
 private:
  int damage_ = 0;
  double cooldown_ = 1;
  ShotgunFire fire_;

 public:
  Shotgun() {
    damage_ = DAMAGE_FROM_SHOOT;
    shots_left_ = NUMBER_OF_PROJECTILES;
  }
  void Shoot(Map& map, Player& owner, int damage) override {
    if (shot_clock_.getElapsedTime().asSeconds() < cooldown_) {
      return;
    }
    shot_clock_.restart();
    fire_.Shoot(map, owner, damage);
    --shots_left_;
  }
};