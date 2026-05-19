#pragma once
#include <math/Vec2.h>

#include <Player.hpp>
#include <Projectile.hpp>
#include <map/Map.hpp>

class Collision {
 public:
  static bool EntityProjectile(const Vec2& pos_projectile, int size_projectile,
                               const Vec2& pos_entity, int size_entity);
  static void EntityTile(Player& entity, Map& map);
  static bool ProjectileTile(const Projectile& proj, const Map& map);
  static void Ricochet(Projectile& projectile, const Map& map);
  static void Resolve(Map& map);
};