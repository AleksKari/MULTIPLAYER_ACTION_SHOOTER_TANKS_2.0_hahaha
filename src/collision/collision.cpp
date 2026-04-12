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
  // пиксельные границы карты — проверяем все 4 угла хитбокса
  if (e.position.x < 0 || e.position.y < 0 || e.position.x + e.size > map.width_ * 32 ||
      e.position.y + e.size > map.height_ * 32) { return true; }
  int x0 = static_cast<int>(e.position.x) / 32;
  int y0 = static_cast<int>(e.position.y) / 32;
  int x1 = static_cast<int>(e.position.x + e.size - 1) / 32;
  int y1 = static_cast<int>(e.position.y + e.size - 1) / 32;
  return map.tiles_[x0][y0].isWall() || map.tiles_[x1][y0].isWall() ||
          map.tiles_[x0][y1].isWall() || map.tiles_[x1][y1].isWall();
}

bool Collision::projectile_tile(const Projectile& proj, const Map& map) {
  if (proj.position.x < 0 || proj.position.y < 0 || proj.position.x + proj.size > map.width_ * 32 ||
      proj.position.y + proj.size > map.height_ * 32) { return true; }
  int x0 = static_cast<int>(proj.position.x) / 32;
  int y0 = static_cast<int>(proj.position.y) / 32;
  int x1 = static_cast<int>(proj.position.x + proj.size - 1) / 32;
  int y1 = static_cast<int>(proj.position.y + proj.size - 1) / 32;
  return map.tiles_[x0][y0].isWall() || map.tiles_[x1][y0].isWall() ||
          map.tiles_[x0][y1].isWall() || map.tiles_[x1][y1].isWall();
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