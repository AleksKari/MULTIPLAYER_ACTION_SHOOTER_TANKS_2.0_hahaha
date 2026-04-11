#pragma once
#include "../libraries/entity.hpp"
#include "../libraries/projectile.hpp"
#include "../tile/tile.hpp"
#include "../collision/collision.hpp"
#include "../src/render/render.hpp"
#include "../render/render.hpp"
#include <memory>

class Map {
 public:
  Map(int width, int height);
  void update(double dt);
  void spawn_entity(Entity* ent);
  void spawn_projectile(Vec2 pos, Vec2 vel, int damage, int size);
  bool isBound(int pos_x, int pos_y) const;//тайл в пределах карты
  bool isWall(int pos_x, int pos_y) const; // проверяет что текущий ТАЙЛ это стена
  bool isEmpty(int pos_x, int pos_y) const;//тайл пуст
  bool isSlow(int pos_x, int pos_y) const;//тайл замедляет
  bool isDamage(int pos_x, int pos_y) const;//тайл наносит урон
  bool isBreakable(int pos_x, int pos_y) const;//тайл разрушаем
  void render(Renderer& renderer) const;
  friend class Collision;
 private:
  int width_;
  int height_;
  int tile_size_;
  std::vector<std::vector<Tile>> tiles_;
  std::vector<std::unique_ptr<Entity>> entities_;
  std::vector<std::unique_ptr<Projectile>> projectiles_;
};