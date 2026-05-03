#include "collision.hpp"
#include "map/map.hpp"
#include <iostream>


bool Collision::entity_projectile(const Vec2& pos_projectile, const int size_projectile, const Vec2& pos_entity, const int size_entity = 32) {
  double center_projectile_x = pos_projectile.x + (size_projectile / 2.0);
  double center_projectile_y = pos_projectile.y + (size_projectile / 2.0);
  double center_entity_x = pos_entity.x + (size_entity / 2.0);
  double center_entity_y = pos_entity.y + (size_entity / 2.0);
  double dist_x = center_projectile_x - center_entity_x;
  double dist_y = center_projectile_y - center_entity_y;
  double radius_projectile = size_projectile / 2.0;
  double radius_entity = size_entity / 2.0;
  double distance_between_projectile_and_entity = (dist_x * dist_x) + (dist_y * dist_y);
  double minimum_distance_between_projectile_and_entity = (radius_projectile + radius_entity) * (radius_projectile + radius_entity);
  return distance_between_projectile_and_entity < minimum_distance_between_projectile_and_entity;
}
// x0 x1 y0 y1 координаты в тайлах где находится плеер
bool Collision::entity_tile (const Player& entity, const Map& map) {
  if (entity.position.x < 0 || entity.position.y < 0 ||
      ((entity.position.x + entity.size) / 32) >= map.width_ ||
      ((entity.position.y + entity.size) / 32) >= map.height_) { return true; }
  int x0 = static_cast<int>(entity.position.x) / 32;
  int y0 = static_cast<int>(entity.position.y) / 32;
  int x1 = static_cast<int>(entity.position.x + entity.size) / 32;
  int y1 = static_cast<int>(entity.position.y + entity.size) / 32;
  return (map.tiles_[x0][y0]->is_wall()) || (map.tiles_[x1][y0]->is_wall()) ||
        (map.tiles_[x0][y1]->is_wall()) || (map.tiles_[x1][y1]->is_wall());
}
//x0 x1 y0 y1 также для пули
bool Collision::projectile_tile(const Projectile& proj, const Map& map) {
  if (proj.position.x < 0 || proj.position.y < 0 ||
      proj.position.x + proj.size > map.width_ * 32 ||
      proj.position.y + proj.size > map.height_ * 32) { return true; }
  int x0 = static_cast<int>(proj.position.x) / 32;
  int y0 = static_cast<int>(proj.position.y) / 32;
  int x1 = static_cast<int>(proj.position.x + proj.size) / 32;
  int y1 = static_cast<int>(proj.position.y + proj.size) / 32;
  return (map.tiles_[x0][y0] && map.tiles_[x0][y0]->is_wall()) || (map.tiles_[x1][y0]->is_wall()) ||
        (map.tiles_[x0][y1] && map.tiles_[x0][y1]->is_wall()) ||(map.tiles_[x1][y1]->is_wall());
}

void Collision::resolve(Map& map) {
  for (auto& projectile : map.projectiles_) {
    if (!projectile_tile(*projectile, map)) continue;
    if (projectile->hp_ > 1) {
      if (projectile->lifetime() > 7) { projectile->kill(); continue; }
      bool not_on_map_x = projectile->position.x < 0 || projectile->position.x + projectile->size > map.width_ * 32;
      bool not_on_map_y = projectile->position.y < 0 || projectile->position.y + projectile->size > map.height_ * 32;
      if (not_on_map_x || not_on_map_y) {
        if (not_on_map_x) projectile->velocity_.x = -projectile->velocity_.x;
        if (not_on_map_y) projectile->velocity_.y = -projectile->velocity_.y;
      } else {
        int curr_projectile_x = static_cast<int>(projectile->position.x + projectile->size) / 32;
        int curr_projectile_y = static_cast<int>(projectile->position.y + projectile->size) / 32;
        int prev_projectile_x = static_cast<int>(projectile->prev_position_.x + projectile->size) / 32;
        int prev_projectile_y = static_cast<int>(projectile->prev_position_.y + projectile->size) / 32;
        if (curr_projectile_x != prev_projectile_x) projectile->velocity_.x = -projectile->velocity_.x;
        else if (curr_projectile_y != prev_projectile_y) projectile->velocity_.y = -projectile->velocity_.y;
        if (curr_projectile_x == prev_projectile_x && curr_projectile_y == prev_projectile_y) {
          projectile->velocity_.x = -projectile->velocity_.x;
          projectile->velocity_.y = -projectile->velocity_.y;
        }
      }
      projectile->position = projectile->prev_position_;
      projectile->position.x += projectile->velocity_.x > 0 ? 1 : -1;
      projectile->position.y += projectile->velocity_.y > 0 ? 1 : -1;
      projectile->take_damage(1);
    } else {
      projectile->kill();
    }
  }
  for (auto& projectile : map.projectiles_) {
    if (projectile->isDead()) continue;
    if (projectile->lifetime() < 0.1) continue;
    for (auto& entity : map.entities_) {
      if (entity->isDead()) continue;
      if (entity_projectile(projectile->position, projectile->size, entity->position, entity->size)) {
        entity->take_damage(projectile->damage);
        projectile->kill();
        break;
      }
    }
  }
  for (auto& entity : map.entities_) {
    if (entity_tile(*entity, map)) {
      entity->on_wall_collision();
    }
  }
}
