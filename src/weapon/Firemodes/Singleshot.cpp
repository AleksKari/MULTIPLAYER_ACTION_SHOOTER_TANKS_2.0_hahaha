#include "Singleshot.hpp"
#include "../../../libraries/projectile.hpp"
#include "../../map/map.hpp"

void SingleShot::shoot(Map& map, Player& owner, int damage) {
  Vec2 vel = owner.dir().normilized() * 400.0f;
  map.spawn_projectile(owner.position, vel, damage, 6); // размер пули
}
