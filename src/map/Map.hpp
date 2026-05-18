#pragma once
#include <SFML/Network.hpp>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "math/Vec2.h"
#include "tile/ConceptTile.hpp"

class Player;
class Projectile;
class Renderer;
class Tile;

class Map {
 public:
  Map(int width, int height);
  void update(double dt);
  void spawn_entity(Player* ent);
  void spawn_projectile(Vec2 pos, Vec2 vel, int damage, int size, int hp = 1,
                        const Player* source = nullptr);

  template <is_tile T>
  void set_tile(const Vec2& pos, std::unique_ptr<T> tl);

  bool is_bound(double pos_x, double pos_y) const;
  bool is_wall(double pos_x, double pos_y) const;
  bool is_empty(double pos_x, double pos_y) const;
  bool is_slow(double pos_x, double pos_y) const;
  bool is_damage(double pos_x, double pos_y) const;
  void render(Renderer& renderer) const;
  std::optional<std::tuple<int, int, int>> generate_weapon();

  // Сетевые методы для NetworkManager
  void update_remote_player(sf::Uint32 id, sf::Packet& packet);
  void spawn_remote_projectile(sf::Packet& packet);
  void update_player_hp(sf::Uint32 id, int hp);
  void spawn_weapon_at(int x, int y, int type);
  void serialize_game_state(sf::Packet& packet) const;
  void apply_game_state(sf::Packet& packet);
  std::vector<Vec2> consume_removed_weapon_tiles();

  int width_;
  int height_;
  std::vector<std::vector<std::unique_ptr<Tile>>> tiles_;
  std::vector<std::unique_ptr<Player>> entities_;
  std::vector<std::unique_ptr<Projectile>> projectiles_;
  std::vector<Vec2> removed_weapon_tiles_;

 private:
  sf::Texture background_texture_;
  mutable sf::Sprite background_sprite_;
};

template <is_tile T>
void Map::set_tile(const Vec2& pos, std::unique_ptr<T> tl) {
  if (tiles_[pos.x][pos.y] && tiles_[pos.x][pos.y]->is_weapon() &&
      tl->is_empty()) {
    removed_weapon_tiles_.push_back(pos);
  }
  tiles_[pos.x][pos.y] = std::move(tl);
}
