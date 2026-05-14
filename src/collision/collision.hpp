#pragma once
#include "../math/Vec2.h"
#include "../map/map.hpp"
#include "../../libraries/player.hpp"
#include "../../libraries/projectile.hpp"

class Map; //forward declaration

class Collision {
 public:
  static bool entity_projectile(const Vec2& pos_projectile, const int size_projectile, const Vec2& pos_entity, const int size_entity);
  static void entity_tile(Player& entity, Map& map);
  static bool projectile_tile(const Projectile& proj, const Map& map);
  static void ricochet(Projectile& projectile, const Map& map);
  static void resolve(Map& map);
};