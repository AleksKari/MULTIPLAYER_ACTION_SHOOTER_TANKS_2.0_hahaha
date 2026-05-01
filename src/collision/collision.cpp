#include "collision.hpp"
#include "map/map.hpp"
#include <iostream>

bool Collision::checkAABB(const Vec2& posA, const int sizeA,
                          const Vec2& posB, const int sizeB) {
  double dist = sqrt((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y));
  return (dist - ((sizeA + sizeB) / 2.0) > 0);
}

bool Collision::entity_tile(const Player& e, const Map& map) {
  if (e.position.x < 0 || e.position.y < 0 ||
      ((e.position.x + e.size) / 32) >= map.width_ ||
      ((e.position.y + e.size) / 32) >= map.height_) { return true; }
  int x0 = static_cast<int>(e.position.x) / 32;
  int y0 = static_cast<int>(e.position.y) / 32;
  int x1 = static_cast<int>(e.position.x + e.size - 1) / 32;
  int y1 = static_cast<int>(e.position.y + e.size - 1) / 32;
  return (map.tiles_[x0][y0] && map.tiles_[x0][y0]->is_wall()) ||
         (map.tiles_[x1][y0] && map.tiles_[x1][y0]->is_wall()) ||
         (map.tiles_[x0][y1] && map.tiles_[x0][y1]->is_wall()) ||
         (map.tiles_[x1][y1] && map.tiles_[x1][y1]->is_wall());
}

bool Collision::projectile_tile(const Projectile& proj, const Map& map) {
  if (proj.position.x < 0 || proj.position.y < 0 ||
      proj.position.x + proj.size > map.width_ * 32 ||
      proj.position.y + proj.size > map.height_ * 32) { return true; }
  int x0 = static_cast<int>(proj.position.x) / 32;
  int y0 = static_cast<int>(proj.position.y) / 32;
  int x1 = static_cast<int>(proj.position.x + proj.size - 1) / 32;
  int y1 = static_cast<int>(proj.position.y + proj.size - 1) / 32;
  return (map.tiles_[x0][y0] && map.tiles_[x0][y0]->is_wall()) ||
         (map.tiles_[x1][y0] && map.tiles_[x1][y0]->is_wall()) ||
         (map.tiles_[x0][y1] && map.tiles_[x0][y1]->is_wall()) ||
         (map.tiles_[x1][y1] && map.tiles_[x1][y1]->is_wall());
}

void Collision::resolve(Map& map) {
  for (auto& p : map.projectiles_) {
    if (!projectile_tile(*p, map)) continue;
    if (p->hp_ > 1) {
      if (p->lifetime() > 7) { p->kill(); continue; }
      bool oob_x = p->position.x < 0 || p->position.x + p->size > map.width_ * 32;
      bool oob_y = p->position.y < 0 || p->position.y + p->size > map.height_ * 32;
      if (oob_x || oob_y) {
        if (oob_x) p->velocity_.x = -p->velocity_.x;
        if (oob_y) p->velocity_.y = -p->velocity_.y;
      } else {
        int curr_x = (int)(p->position.x + p->size) / 32;
        int curr_y = (int)(p->position.y + p->size) / 32;
        int prev_x = (int)(p->prev_position_.x + p->size) / 32;
        int prev_y = (int)(p->prev_position_.y + p->size) / 32;
        if (curr_x != prev_x) p->velocity_.x = -p->velocity_.x;
        if (curr_y != prev_y) p->velocity_.y = -p->velocity_.y;
        if (curr_x == prev_x && curr_y == prev_y) {
          p->velocity_.x = -p->velocity_.x;
          p->velocity_.y = -p->velocity_.y;
        }
      }

      p->position = p->prev_position_;
      p->position.x += p->velocity_.x > 0 ? 1 : -1;
      p->position.y += p->velocity_.y > 0 ? 1 : -1;
      p->take_damage(1);
    } else {
      p->kill();
    }
  }

  for (auto& p : map.projectiles_) {
    if (p->isDead()) continue;
    if (p->lifetime() < 0.1) continue;
    for (auto& e : map.entities_) {
      if (e->isDead()) continue;
      if (!checkAABB(p->position, p->size, e->position, e->size)) {
        e->take_damage(p->damage);
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
