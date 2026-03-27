#include "ShotgunFire.hpp"
#include "../../libraries/projectile.hpp"
#include "../math/Vec2.h"

void ShotgunFire::fire(const WeaponStates& states, const Vec2& pos, const Vec2& dir) {
    double base_angel = dir.angle();
    double half = states.spread_angle * 0.5;
    for (int i = 0; i < states.projectile_per_shoot; ++i) {
        double t = static_cast<double>(i) / (states.projectile_per_shoot - 1);
        
    }
}