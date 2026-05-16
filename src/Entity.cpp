#include <Entity.hpp>

Entity::Entity() : position(Vec2()), size(TILESIZE)  {}
Entity::Entity(Vec2 pos, int dmg, int sz) :
            position(pos), damage(dmg), size(sz) {}
