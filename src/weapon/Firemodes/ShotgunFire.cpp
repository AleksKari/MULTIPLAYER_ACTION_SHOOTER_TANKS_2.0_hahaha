#include <Player.hpp>
#include <numbers>
#include <weapon/Firemodes/ShotgunFire.hpp>

const int BASE_ANGEL = 20;

static double rad(double degrees) {
  return degrees * std::numbers::pi / FULL_ANGLE;
}

void ShotgunFire::shoot(Map& map, Player& owner, int damage) {
  // 5 пуль веером
  for (int i = -2; i <= 2; ++i) {
    Vec2 dir = owner.dir().rotate(rad(BASE_ANGEL) * i);
    Vec2 vel = dir.normalized() * VELOCITY_OF_PROJECTILE;
    map.spawn_projectile(
        owner.position + Vec2(CENTER_OF_PLAYER, CENTER_OF_PLAYER), vel, damage,
        SIZE_OF_PROJECTILE);  // +vec для выстрела с центра танка а не с
                              // верхнего левого угла
  }
}
