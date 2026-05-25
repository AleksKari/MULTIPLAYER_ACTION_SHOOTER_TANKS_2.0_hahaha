#pragma once
#include <math/Vec2.h>

#include <SFML/Network.hpp>
#include <memory>
#include <optional>
#include <tile/ConceptTile.hpp>
#include <tuple>
#include <utility>
#include <vector>

#include <Player.hpp>
#include <Projectile.hpp>

class Renderer;
class Tile;

class Map {
 public:
  Map(int width, int height);
  void Update(double difftime);
  void SpawnEntity(Player* ent);
  void SpawnProjectile(Vec2 pos, Vec2 vel, int damage, int size,
                       int heatpoint = 1, const Player* source = nullptr);

  template <is_tile T>
  void SetTile(const Vec2& pos, std::unique_ptr<T> tile);

  bool IsBound(double pos_x, double pos_y) const;
  bool IsWall(double pos_x, double pos_y) const;
  bool IsEmpty(double pos_x, double pos_y) const;
  bool IsSlow(double pos_x, double pos_y) const;
  bool IsDamage(double pos_x, double pos_y) const;
  void ResetWorld();
  void Render(Renderer& renderer) const;
  std::optional<std::tuple<int, int, int>> GenerateWeapon();

  // Сетевые методы для NetworkManager
  void UpdateRemotePlayer(sf::Uint32 user_id, sf::Packet& packet);
  void SpawnRemoteProjectile(sf::Packet& packet);
  void UpdatePlayerHp(sf::Uint32 user_id, int heatpoint);
  void SpawnWeaponAt(int cord_x, int cord_y, int type);
  void SerializeGameState(sf::Packet& packet) const;
  void ApplyGameState(sf::Packet& packet);
  std::vector<Vec2> ConsumeRemovedWeaponTiles();

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
void Map::SetTile(const Vec2& pos, std::unique_ptr<T> tile) {
  if (tiles_[pos.cord_x][pos.cord_y] &&
      tiles_[pos.cord_x][pos.cord_y]->IsWeapon() && tile->IsEmpty()) {
    removed_weapon_tiles_.push_back(pos);
  }
  tiles_[pos.cord_x][pos.cord_y] = std::move(tile);
}
