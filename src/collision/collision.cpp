#include "collision.hpp"

bool Collision::checkAABB(const Vec2& posA, const Vec2& sizeA,
                          const Vec2& posB, const Vec2& sizeB) {
    return !(posA.x + sizeA.x < posB.x ||
             posB.x + sizeB.x < posA.x ||
             posA.y + sizeA.y < posA.y ||
             posB.y + sizeB.y < posA.y);
}

bool Collision::entity_tile(const Entity& e, const Map& map) {
    // tiles!!!!!
}