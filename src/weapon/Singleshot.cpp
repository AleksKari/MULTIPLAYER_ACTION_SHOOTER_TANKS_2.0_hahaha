#include "Singleshot.hpp"
#include "../../libraries/projectile.hpp"

void SingleShot::fire(const WeaponStates& states, const Vec2& pos, const Vec2& dir) {
    Projectile* p = Projectile(pos, dir.normilized() * states.projectile_speed, states.damage, 32);
}
