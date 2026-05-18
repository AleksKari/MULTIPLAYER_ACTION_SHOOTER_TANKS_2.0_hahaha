#include <collision/Collision.hpp>

#include <iostream>

#include <map/Map.hpp>

static constexpr double MAX_RICOCHET_LIFETIME = 7.0;
static constexpr double MIN_PROJECTILE_LIFETIME = 0.1;

bool Collision::entity_projectile(const Vec2& pos_projectile,
                                  const int size_projectile,
                                  const Vec2& pos_entity,
                                  const int size_entity = TILESIZE) {
  double center_projectile_x = pos_projectile.cord_x + (size_projectile / 2.0);
  double center_projectile_y = pos_projectile.cord_y + (size_projectile / 2.0);

  double center_entity_x = pos_entity.cord_x + (size_entity / 2.0);
  double center_entity_y = pos_entity.cord_y + (size_entity / 2.0);

  double dist_x = center_projectile_x - center_entity_x;
  double dist_y = center_projectile_y - center_entity_y;

  double radius_projectile = size_projectile / 2.0;
  double radius_entity = size_entity / 2.0;

  double distance_between_projectile_and_entity =
      (dist_x * dist_x) + (dist_y * dist_y);

  double minimum_distance_between_projectile_and_entity =
      (radius_projectile + radius_entity) * (radius_projectile + radius_entity);

  return distance_between_projectile_and_entity <
         minimum_distance_between_projectile_and_entity;
}
// x0 x1 y0 y1 координаты в тайлах где находится плеер
void Collision::entity_tile(Player& entity, Map& map) {
  // для проезда в зазор 1 блок уменьшаем счиатемый размер на 1
  if (entity.position.cord_x < 0 || entity.position.cord_y < 0 ||
      ((entity.position.cord_x + entity.size - 1) / TILESIZE) >= map.width_ ||
      ((entity.position.cord_y + entity.size - 1) / TILESIZE) >= map.height_) {
    entity.on_wall_collision();
    return;
  }
  int x0 = static_cast<int>(entity.position.cord_x) / TILESIZE;
  int y0 = static_cast<int>(entity.position.cord_y) / TILESIZE;

  int x1 = static_cast<int>(entity.position.cord_x + entity.size - 1) / TILESIZE;
  int y1 = static_cast<int>(entity.position.cord_y + entity.size - 1) / TILESIZE;

  map.tiles_[x0][y0]->interact(entity, map, Vec2(x0, y0));

  if (x1 != x0) map.tiles_[x1][y0]->interact(entity, map, Vec2(x1, y0));
  if (y1 != y0) map.tiles_[x0][y1]->interact(entity, map, Vec2(x0, y1));
  if (x1 != x0 && y1 != y0)
    map.tiles_[x1][y1]->interact(entity, map, Vec2(x1, y1));
}
// x0 x1 y0 y1 также для пули
bool Collision::projectile_tile(const Projectile& proj, const Map& map) {
  if (proj.position.cord_x < 0 || proj.position.cord_y < 0 ||
      proj.position.cord_x + proj.size > map.width_ * TILESIZE ||
      proj.position.cord_y + proj.size > map.height_ * 32) {
    return true;
  }

  int x0 = static_cast<int>(proj.position.cord_x) / TILESIZE;
  int y0 = static_cast<int>(proj.position.cord_y) / TILESIZE;

  int x1 = static_cast<int>(proj.position.cord_x + proj.size) / TILESIZE;
  int y1 = static_cast<int>(proj.position.cord_y + proj.size) / TILESIZE;

  return (map.tiles_[x0][y0]->is_wall()) || (map.tiles_[x1][y0]->is_wall()) ||
         (map.tiles_[x0][y1]->is_wall()) || (map.tiles_[x1][y1]->is_wall());
}

void Collision::ricochet(Projectile& projectile, const Map& map) {
  if (projectile.lifetime() > MAX_RICOCHET_LIFETIME) {
    projectile.kill();
    return;
  }

  bool not_on_map_x =
      projectile.position.cord_x < 0 ||
      projectile.position.cord_x + projectile.size > map.width_ * TILESIZE;
  bool not_on_map_y =
      projectile.position.cord_y < 0 ||
      projectile.position.cord_y + projectile.size > map.height_ * TILESIZE;

  bool reflect_x, reflect_y;
  if (not_on_map_x || not_on_map_y) {
    reflect_x = not_on_map_x;
    reflect_y = not_on_map_y;
  } else {
    reflect_x = projectile.crossed_tile_x();
    reflect_y = projectile.crossed_tile_y();
  }
  projectile.reflect(reflect_x, reflect_y);
  projectile.take_damage(1);
}

void Collision::resolve(Map& map) {
  for (auto& projectile : map.projectiles_) {
    if (!projectile_tile(*projectile, map)) continue;
    if (projectile->can_ricochet()) {
      ricochet(*projectile, map);
    } else {
      projectile->kill();
    }
  }

  for (auto& projectile : map.projectiles_) {
    if (projectile->is_dead()) continue;
    if (projectile->lifetime() < MIN_PROJECTILE_LIFETIME) continue;

    for (auto& entity : map.entities_) {
      if (entity->is_dead()) continue;
      if (entity_projectile(projectile->position, projectile->size,
                            entity->position, entity->size)) {
        entity->take_damage(projectile->damage);
        projectile->kill();
        break;
      }
    }
  }

  for (auto& entity : map.entities_) {
    entity_tile(*entity, map);
  }
}
