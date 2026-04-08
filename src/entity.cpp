#include "../libraries/entity.hpp"

Entity::Entity() : position(Vec2()), size(0)  {}
Entity::Entity(Vec2 pos, int dmg, int sz = 32) :
            position(pos), damage(dmg), size(sz) {}
