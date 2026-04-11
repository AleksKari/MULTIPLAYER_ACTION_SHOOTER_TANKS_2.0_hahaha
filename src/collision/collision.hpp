#pragma once
#include "../math/Vec2.h"
#include "../map/map.hpp"
#include "../../libraries/entity.hpp"
#include "../../libraries/projectile.hpp"

class Map; //forward declaration

class Collision {
  public:
    static bool checkAABB(const Vec2& posA, const Vec2& dirA,
                          const Vec2& posB, const Vec2& dirB);
    static bool entity_tile(const Entity& entity, const Map& map);
    static bool projectile_tile(const Projectile& proj, const Map& map);
    static void resolve(Map& map);
};