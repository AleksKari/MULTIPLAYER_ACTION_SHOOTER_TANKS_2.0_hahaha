#include "RicochetShot.hpp"
#include "../../../libraries/projectile.hpp"
#include "../../map/map.hpp"

void RicochetShot::shoot(Map& map, Player& owner, int damage) {
  Vec2 vel = owner.dir().normilized() * 400.0f;
  map.spawn_projectile(owner.position + Vec2(16, 16), vel, damage, 6, 3);// +vec для выстрела с центра танка а не с верхнего левого угла
}
