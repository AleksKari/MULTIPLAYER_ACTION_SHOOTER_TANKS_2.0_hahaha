// ShotgunFire.cpp
#include "ShotgunFire.hpp"
#include "../../../libraries/player.hpp"

double rad(double degrees) {
    return degrees * 3.14159265358979323846 / 180.0;
}

void ShotgunFire::shoot(Map& map, Player& owner, int damage) {
    // 5 пуль веером
    for (int i = -2; i <= 2; ++i) {
        Vec2 dir = owner.dir().rotate(rad(20) * i);
        Vec2 vel = dir.normilized() * 400.0f;

        map.spawn_projectile(
            owner.position,
            vel,
            damage,
            6 // размер пули
        );
    }
}
