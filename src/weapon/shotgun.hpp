#include "weapon.hpp"
#include "Firemodes/ShotgunFire.hpp"
#include "../../libraries/entity.hpp"
#include "../map/map.hpp"

class ShotGun : public  Weapon {
  private:
    int damage_ = 0;
    double cooldown = 0.8;
    ShotgunFire fire_;
  public:
    ShotGun() {
        damage_ = 10;
    }
    void shoot(Map& map, Player& owner, int damage) override {
        fire_.shoot(map, owner, damage);
    }
};