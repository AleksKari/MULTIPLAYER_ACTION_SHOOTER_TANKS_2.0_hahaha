#include "weapon.hpp"
#include "Singleshot.hpp"
#include "../../libraries/entity.hpp"
#include "../map/map.hpp"

class Gun : public  Weapon {
  private:
    int damage_ = 0;
    double cooldown = 0.8;
    SingleShot fire_;
  public:
    Gun() {
        damage_ = 10;
    }
    void shoot(Map& map, Entity& owner) override {
        
    }
}