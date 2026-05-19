#include <Projectile.hpp>
#include <map/Map.hpp>
#include <weapon/Firemodes/RicochetShot.hpp>

const int HEATPOINT_OF_PROJECTILE = 5;
void RicochetShot::shoot(Map& map, Player& owner, int damage) {
  Vec2 vel = owner.dir().normalized() * VELOCITY_OF_PROJECTILE;
  map.spawn_projectile(
      owner.position + Vec2(CENTER_OF_PLAYER, CENTER_OF_PLAYER), vel, damage,
      SIZE_OF_PROJECTILE,
      HEATPOINT_OF_PROJECTILE);  // +vec для выстрела с центра танка а не с
                                 // верхнего левого угла
}
