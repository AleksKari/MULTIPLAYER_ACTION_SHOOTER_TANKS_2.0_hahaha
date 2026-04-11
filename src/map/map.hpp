#pragma once
#include "../libraries/entity.hpp"
#include "../libraries/projectile.hpp"
#include "../tile/tile.hpp"
#include "../render/render.hpp"
#include <memory>

class Map {
 public:
  Map(int width, int height);
  void update(double dt);
  void spawn_entity(Entity* ent);
  void spawn_projectile(Vec2 pos, Vec2 vel, int damage, int size);
  bool isBound(int pos_x, int pos_y) const;
  bool isWall(int pos_x, int pos_y) const;
  bool isEmpty(int pos_x, int pos_y) const;
  bool isSlow(int pos_x, int pos_y) const;
  bool isDamage(int pos_x, int pos_y) const;
  bool isBreakable(int pos_x, int pos_y) const;
  void setTile(int x, int y, Tile::Type type, sf::IntRect rect = {0,0,32,32});
  void render(Renderer& renderer) const;
  friend class Collision;
 private:
  int width_;
  int height_;
  int tile_size_;
  std::vector<std::vector<Tile>> tiles_;
  std::vector<std::unique_ptr<Entity>> entities_;
  std::vector<std::unique_ptr<Projectile>> projectiles_;
  sf::Texture background_texture_;
  mutable sf::Sprite  background_sprite_;
};