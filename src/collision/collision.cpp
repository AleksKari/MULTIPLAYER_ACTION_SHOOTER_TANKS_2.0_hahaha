#include "collision.hpp"
#include "map/map.hpp"

bool Collision::checkAABB(const Vec2& posA, const Vec2& sizeA,
                          const Vec2& posB, const Vec2& sizeB) {
    return !(posA.x + sizeA.x < posB.x ||
             posB.x + sizeB.x < posA.x ||
             posA.y + sizeA.y < posB.y ||
             posB.y + sizeB.y < posA.y);
}

bool Collision::entity_tile(const Entity& e, const Map& map) {
    int tile_x = static_cast<int>(e.position.x / 32); //32 x 32 размер тайла в игре
    int tile_y = static_cast<int>(e.position.y / 32);
    if (!map.isBound(tile_x, tile_y)) {
        return true;
    }
    return map.tiles_[tile_x][tile_y].isWall();
}

bool Collision::projectile_tile(const Projectile& proj, const Map& map) {
    int tile_x = static_cast<int>(proj.position.x / 32);
    int tile_y = static_cast<int>(proj.position.y / 32);
    if (!map.isBound(tile_x, tile_y)) {
        return true;
    }
    return map.tiles_[tile_x][tile_y].isWall();
} 

void Collision::resolve(Map& map) {
    for (auto& p : map.projectiles_) {
        if (projectile_tile(*p, map)) {
            p->kill();
            continue;
        }
    }
    for (auto& p : map.projectiles_) {
        if (p->isDead()) continue;
        for (auto& e : map.entities_) {
            if (e->isDead()) continue;
            if (checkAABB(p->position, Vec2(p->size, p->size), e->position, Vec2(e->size, e->size))) {
                e->take_damage(e->damage);
                p->kill();
                break;
            }
        }
    }
    for (auto& e : map.entities_) {
        if (entity_tile(*e, map)) {
            e->on_wall_collision();
        }
    }

}