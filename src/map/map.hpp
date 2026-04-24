#pragma once
#include "../libraries/player.hpp"
#include "../libraries/projectile.hpp"
#include "../tile/EmptyTile.hpp"
#include "../render/render.hpp"
#include <memory>
#include <concepts>

class Player;

class Map {
 public:
  Map(int width, int height);
  void update(double dt);
  void spawn_entity(Player* ent);
  void spawn_projectile(Vec2 pos, Vec2 vel, int damage, int size);
  template<is_tile T>
  void set_tile(Vec2 pos, const T& tl);
  bool isBound(int pos_x, int pos_y) const;
  bool isWall(int pos_x, int pos_y) const;
  bool isEmpty(int pos_x, int pos_y) const;
  bool isSlow(int pos_x, int pos_y) const;
  bool isDamage(int pos_x, int pos_y) const;
  bool isBreakable(int pos_x, int pos_y) const;
  //void setTile(int x, int y, Tile::Type type, sf::IntRect rect = {0, 0, 32, 32});
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
void Map::set_tile(Vec2 pos, const T& tl) {
  tiles_[pos.x][pos.y] = std::make_unique<T>(tl);
}