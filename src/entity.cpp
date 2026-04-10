#include "../libraries/entity.hpp"

Entity::Entity() : position(Vec2()), size(0)  {}
Entity::Entity(sf::IntRect texture_rect, Vec2 pos, int dmg, int sz = 32) :
            texture_rect_(texture_rect), position(pos), damage(dmg), size(sz) {}
