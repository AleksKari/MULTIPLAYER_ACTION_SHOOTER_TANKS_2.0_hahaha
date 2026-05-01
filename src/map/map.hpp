#pragma once
#include <memory>
#include <concepts>
#include <vector>
#include "../tile/is_tile.hpp"
#include "../math/Vec2.h"

class Player;
class Projectile;
class EmptyTile;
class Renderer;
class Tile;


class Map {
 public:
  Map(int width, int height);
  void update(double dt);
  void spawn_entity(Player* ent);
  void spawn_projectile(Vec2 pos, Vec2 vel, int damage, int size, int hp = 1);
  template<is_tile T>
  void set_tile(const Vec2& pos, std::unique_ptr<T> tl);
  bool isBound(int pos_x, int pos_y) const;
  bool isWall(int pos_x, int pos_y) const;  
  bool isEmpty(int pos_x, int pos_y) const;
  bool isSlow(int pos_x, int pos_y) const;
  bool isDamage(int pos_x, int pos_y) const;
  bool isBreakable(int pos_x, int pos_y) const;
  void render(Renderer& renderer) const;
  void generate_weapon();
  friend class Collision;
 private:
  int width_;
  int height_;
  std::vector<std::vector<std::unique_ptr<Tile>>> tiles_;
  std::vector<std::unique_ptr<Player>> entities_;
  std::vector<std::unique_ptr<Projectile>> projectiles_;
  sf::Texture background_texture_;
  mutable sf::Sprite background_sprite_;
};

template <is_tile T>
void Map::set_tile(const Vec2& pos, std::unique_ptr<T> tl) {
  tiles_[pos.x][pos.y] = std::move(tl); 
}