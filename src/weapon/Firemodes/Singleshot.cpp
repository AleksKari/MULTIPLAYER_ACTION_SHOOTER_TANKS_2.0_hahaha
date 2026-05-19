#include <Projectile.hpp>
#include <map/Map.hpp>
#include <weapon/Firemodes/Singleshot.hpp>

void SingleShot::shoot(Map& map, Player& owner, int damage) {
  Vec2 vel = owner.dir().normalized() * 800.0f;
  map.spawn_projectile(
      owner.position + Vec2(16, 16), vel, damage,
      6);  // +vec для выстрела с центра танка а не с верхнего левого угла
}
