#include <Projectile.hpp>
#include <map/Map.hpp>
#include <weapon/Firemodes/Singleshot.hpp>

void SingleShot::shoot(Map& map, Player& owner, int damage) {
  Vec2 vel = owner.Dir().normalized() * VELOCITY_OF_PROJECTILE;
  map.spawn_projectile(
      owner.position + Vec2(CENTER_OF_PLAYER, CENTER_OF_PLAYER), vel, damage,
      SIZE_OF_PROJECTILE);  // +vec для выстрела с центра танка а не с верхнего
                            // левого угла
}
