#pragma once
#include <Entity.hpp>
#include <map/Map.hpp>
#include <weapon/Firemodes/Singleshot.hpp>
#include <weapon/Weapon.hpp>

class Gun : public Weapon {
 private:
  int damage_ = 0;
  double cooldown_ = 1;
  SingleShot fire_;

 public:
  Gun() { damage_ = 10; }
  void shoot(Map& map, Player& owner, int damage) override {
    if (shot_clock_.getElapsedTime().asSeconds() < cooldown_) return;
    shot_clock_.restart();
    fire_.shoot(map, owner, damage);
  }
};