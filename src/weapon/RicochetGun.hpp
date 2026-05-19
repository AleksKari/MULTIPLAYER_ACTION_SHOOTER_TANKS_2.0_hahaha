#pragma once
#include <Entity.hpp>
#include <map/Map.hpp>
#include <weapon/Firemodes/RicochetShot.hpp>
#include <weapon/Weapon.hpp>

class RicochetGun : public Weapon {
 private:
  double cooldown_ = 1;
  RicochetShot fire_;

 public:
  RicochetGun() {
    damage_ = DAMAGE_FROM_SHOOT;
    shots_left_ = NUMBER_OF_PROJECTILES;
  }
  void shoot(Map& map, Player& owner, int damage) override {
    if (shot_clock_.getElapsedTime().asSeconds() < cooldown_) {
      return;
    }
    shot_clock_.restart();
    fire_.shoot(map, owner, damage);
    --shots_left_;
  }
};