#include "collision.hpp"

bool Collision::checkAABB(const Vec2& posA, const Vec2& sizeA,
                          const Vec2& posB, const Vec2& sizeB) {
    return !(posA.x + sizeA.x < posB.x ||
             posB.x + sizeB.x < posA.x ||
             posA.y + sizeA.y < posA.y ||
             posB.y + sizeB.y < posA.y);
}

bool Collision::entity_tile(const Entity& e, const Map& map) {
    int tile_x = static_cast<int>(e.pos.x / 32); //32 x 32 размер тайла в игре
    int tile_y = static_cast<int>(e.pos.y / 32);
    if (!map.isBound(tile_x, tile_y)) {
        return true;
    }
    return map.tiles[tile_x][tile_y].isWall();
}

bool Collision::projectile_tile(const Projectile& proj, const Map& map) {
    int tile_x = static_cast<int>(proj.pos.x / 32);
    int tile_y = static_cast<int>(proj.pos.y / 32);
    if (!map.isBound(tile_x, tile_y)) {
        return true;
    }
    return map.tiles[tile_x][tile_y].isWall();
} 

void Collision::resolve(Map& map) {
    for (auto& p : map.projectiles) {
        if (projectile_tile(*p, map)) {
            p->kill();
            continue;
        }
    }
    for (auto& p : map.projectiles) {
        if (p->isDead()) continue;
        for (auto& e : map.entities) {
            if (e->isDead()) continue;
            if (checkAABB(p->pos, p->size, e->pos, e->size)) {
                e->take_damage(e->damage_);
                p->kill();
                break;
            }
        }
    }
    for (auto& e : map.entities) {
        if (entity_tile(*e, map)) {
            e->on_wall_collision();
        }
    }

}