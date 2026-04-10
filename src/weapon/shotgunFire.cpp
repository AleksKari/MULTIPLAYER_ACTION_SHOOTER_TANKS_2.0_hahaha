// ShotgunFire.cpp
#include "ShotgunFire.hpp"
#include "../../libraries/player.hpp"

void ShotgunFire::shoot(Map& map, Player& owner, int damage) {
    // 5 пуль веером
    for (int i = -2; i <= 2; ++i) {
        Vec2 dir = owner.dir().rotate(10.0 * i);
        Vec2 vel = dir.normilized() * 400.0f;

        map.spawn_projectile(
            owner.position,
            vel,
            damage,
            6 // размер пули
        );
    }
}
