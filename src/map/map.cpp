#include "map.hpp"

Map::Map(int width, int height) : width_(width), height_(height) {}

void Map::update(double dt) {
  for(auto& ent : entities_) { ent->update(dt); }
  for(auto& pr : projectiles_) { pr->update(dt); }
  Collision::resolve(*this);
}
//отслеживаем игроков
void Map::spawn_entity(Entity* ent) {
  entities_.push_back(std::unique_ptr<Entity>(ent));
}
//отслеживаем снаряды
void Map::spawn_projectile(Vec2 pos, Vec2 vel, int damage, int size) {
  projectiles_.push_back(std::make_unique<Projectile>(pos, vel, damage, size));
}

bool Map::isBound(int pos_x, int pos_y) const {
  return pos_x >= 0 && pos_y >= 0 &&
  pos_x < width_ && pos_y < height_;
}
// не очень ясно как tile заполняется в этом проблема
bool Map::isWall(int x, int y) const {
    if (!isBound(x, y)) return true;   // край карты = стена
    return tiles_[x][y].isWall();
}

bool Map::isEmpty(int pos_x, int pos_y) const {
  if (!isBound(pos_x,pos_y)) return false;
  return tiles_[pos_x][pos_y].isEmpty();
}

bool Map::isSlow(int pos_x, int pos_y) const{
  if (!isBound(pos_x,pos_y)) return false;
  return tiles_[pos_x][pos_y].isSlow();
}

bool Map::isDamage(int x, int y) const {
    if (!isBound(x, y)) return false;
    return tiles_[x][y].isDamage();
}

bool Map::isBreakable(int x, int y) const {
    if (!isBound(x, y)) return false;
    return tiles_[x][y].isBreakable();
}
//не очень понял концепцию ибо методы не реализованы но тип суть на мой взгляд
void Map::render(Renderer& renderer) const {
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            renderer.draw_tile(tiles_[y][x], x, y);
        }
    }
    for (const auto& e : entities_) {
        renderer.draw_entity(*e);
    }
    for (const auto& pr : projectiles_) {
        renderer.draw_projectile(*pr);
    }
}