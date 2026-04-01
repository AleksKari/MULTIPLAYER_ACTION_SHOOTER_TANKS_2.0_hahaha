#include "ShotgunFire.hpp"
#include "../../libraries/projectile.hpp"
#include "../math/Vec2.h"

void ShotgunFire::fire(const WeaponStates& states, const Vec2& pos, const Vec2& dir) {
    double part_angel = states.spread_angle / states.projectile_per_shoot;
    double base_angel = dir.angle() - part_angel * (states.projectile_per_shoot / 2);
    for (int i = 0; i < states.projectile_per_shoot; ++i) {
        Projectile p(pos, base_angel + i * part_angel, states.damage);
        //Добавление в игру
    }
}