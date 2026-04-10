#include "Singleshot.hpp"
#include "../../libraries/projectile.hpp"
#include "WeaponStates.hpp"
#include "../map/map.hpp"

void SingleShot::fire(const WeaponStates& states, const Vec2& pos, const Vec2& dir) {
    Vec2 vel = dir.normilized() * 400.0f;

    map.spawn_projectile(
        pos,
        vel,
        states.damage,
        6 // размер пули
    );
}
