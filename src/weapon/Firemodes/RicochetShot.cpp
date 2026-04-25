#include "RicochetShot.hpp"
#include "../../../libraries/projectile.hpp"
#include "../../map/map.hpp"

void RicochetShot::shoot(Map& map, Player& owner, int damage) {
  Vec2 center(owner.position.x + 16, owner.position.y + 16);
  Vec2 vel = owner.dir().normilized() * 400.0f;
  map.spawn_projectile(center, vel, damage, 6, 3);
}
