#include "map.hpp"
#include "../collision/collision.hpp"
#include <iostream>

Map::Map(int width, int height) : width_(width), height_(height) {
  tiles_.resize(width_, std::vector<Tile>(height_));
  if (!background_texture_.loadFromFile("textures/map.png")) {
    std::cout << "failed to load background\n";
  }
  background_sprite_.setTexture(background_texture_);
}

void Map::update(double dt) {
  for (auto& ent : entities_) { ent->update(dt); }
  for (auto& pr : projectiles_) { pr->update(dt); }
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
  return pos_x < 0 || pos_y < 0 ||
      pos_x >= width_ || pos_y >= height_;
}

// не очень ясно как tile заполняется в этом проблема
bool Map::isWall(int x, int y) const {
  return (isBound(x / 32, y / 32));  // край карты = стена
  return tiles_[x / 32][y / 32].isWall();
}

bool Map::isEmpty(int pos_x, int pos_y) const {
  if (isBound(pos_x / 32, pos_y / 32)) return false;
  return tiles_[pos_x / 32][pos_y / 32].isEmpty();
}

bool Map::isSlow(int pos_x, int pos_y) const {
  if (isBound(pos_x / 32, pos_y / 32)) return false;
  return tiles_[pos_x / 32][pos_y / 32].isSlow();
}

bool Map::isDamage(int x, int y) const {
  if (isBound(x / 32, y / 32)) return false;
  return tiles_[x / 32][y / 32].isDamage();
}

bool Map::isBreakable(int x, int y) const {
  if (isBound(x / 32, y / 32)) return false;
  return tiles_[x / 32][y / 32].isBreakable();
}

void Map::setTile(int x, int y, Tile::Type type, sf::IntRect rect) {
  if (isBound(x / 32, y / 32)) tiles_[x / 32][y / 32] = Tile(type, rect);
}

//не очень понял концепцию ибо методы не реализованы но тип суть на мой взгляд
void Map::render(Renderer& renderer) const {
  renderer.window().draw(background_sprite_);        // 1. фон

  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      if (!tiles_[x / 32][y / 32].isEmpty())                   // 2. только стены
        renderer.draw_tile(tiles_[x / 32][y / 32], x, y);
    }
  }

  for (const auto& e : entities_) {
    renderer.draw_entity(*e);                        // 3. танки
  }
  for (const auto& pr : projectiles_) {
    renderer.draw_projectile(*pr);                   // 4. пули
  }
}
