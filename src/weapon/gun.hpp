#include "weapon.hpp"
#include "Firemodes/Singleshot.hpp"
#include "../../libraries/entity.hpp"
#include "../map/map.hpp"

class Gun : public  Weapon {
 private:
  int damage_ = 0;
  double cooldown = 1;
  SingleShot fire_;
 public:
  Gun() {
    damage_ = 10;
  }
  void shoot(Map& map, Player& owner, int damage) override {
    if(shot_clock.getElapsedTime().asSeconds() < cooldown) return;
    shot_clock.restart();
    fire_.shoot(map, owner, damage);
  }
};