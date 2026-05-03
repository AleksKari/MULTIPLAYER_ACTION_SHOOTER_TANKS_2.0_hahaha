#pragma once
#include "../math/Vec2.h"
#include "../map/map.hpp"
#include "../../libraries/player.hpp"
#include "../../libraries/projectile.hpp"

class Map; //forward declaration

class Collision {
 public:
  static bool entity_projectile(const Vec2& posA, const int dirA, const Vec2& posB, const int dirB);
  static bool entity_tile(const Player& entity, const Map& map);
  static bool projectile_tile(const Projectile& proj, const Map& map);
  static void ricochet(Projectile& projectile, const Map& map);
  static void resolve(Map& map);
};