#include <iostream>
#include <chrono>
#include <random>
#include <variant>
#include <concepts>
#include <type_traits>
#include "map.hpp"
#include "../collision/collision.hpp"
#include "../weapon/gun.hpp"
#include "../weapon/shotgun.hpp"
#include "../tile/EmptyTile.hpp" 
#include "../tile/WallTile.hpp" 
#include "../tile/WeaponShotgunTile.hpp"
#include "../tile/WeaponRicochetTile.hpp"
#include "../render/render.hpp"
#include "../../libraries/projectile.hpp"
#include "../../libraries/player.hpp"


// конструктор карты
Map::Map(int width, int height) : width_(width), height_(height) {
  tiles_.resize(width_ + 1);
  for (auto& column : tiles_) {
    column.resize(height_ + 1);
  }
  
  for (int i = 0; i <= width_; ++i) {
    for (int j = 0; j <= height_; ++j) {
      tiles_[i][j] = std::make_unique<EmptyTile>();
    }
  }
}

// обновление карты
void Map::update(double diff) {
  for (auto& ent : entities_) { ent->update(diff); }
  for (auto& pr : projectiles_) { pr->update(diff); }
  this->generate_weapon();
  Collision::resolve(*this);
}

//отслеживаем игроков
void Map::spawn_entity(Player* ent) {
  entities_.push_back(std::unique_ptr<Player>(std::move(ent)));
}

//отслеживаем снаряды
void Map::spawn_projectile(Vec2 pos, Vec2 vel, int damage, int size, int hp) {
  projectiles_.push_back(std::make_unique<Projectile>(pos, vel, damage, size, hp));
}


//проверка границ карты
bool Map::is_bound(double pos_x, double pos_y) const {
  return pos_x < 0 || pos_y < 0 ||
      pos_x >= width_ || pos_y >= height_;
}

// не доходило до тайла добавил проверку
bool Map::is_wall(double x, double y) const {
  if (is_bound(x / 32, y / 32)) return true;  // край карты = стена
  return tiles_[x / 32][y / 32]->is_wall();
}

//пустой 
bool Map::is_empty(double pos_x, double pos_y) const {
  if (is_bound(pos_x / 32, pos_y / 32)) return false;
  return tiles_[pos_x / 32][pos_y / 32]->is_empty();
}


//замедляющий
bool Map::is_slow(double pos_x, double pos_y) const {
  if (is_bound(pos_x / 32, pos_y / 32)) return false;
  return tiles_[pos_x / 32][pos_y / 32]->is_slow();
}

// наносящий урон
bool Map::is_damage(double x, double y) const {
  if (is_bound(x / 32, y / 32)) return false;
  return tiles_[x / 32][y / 32]->is_damage();
}

// ломающийся 
bool Map::is_breakable(double x, double y) const {
  if (is_bound(x / 32, y / 32)) return false;
  return tiles_[x / 32][y / 32]->is_breakable();
}

// отрисовка объектов 
void Map::render(Renderer& renderer) const {

  for (int y = 0; y <= height_; ++y) {
    for (int x = 0; x <= width_; ++x) {               // 2. только стены
      tiles_[x][y]->draw(renderer, Vec2(x, y));
    }
  }

  for (const auto& e : entities_) {
    renderer.draw_entity(*e);                        // 3. танки
    renderer.draw_hp_bar(*e);   
  }
  for (const auto& pr : projectiles_) {
    renderer.draw_projectile(*pr);                   // 4. пули
  }
}
// генерация оружия
void Map::generate_weapon() {
  std::mt19937 mt(std::chrono::steady_clock::now().time_since_epoch().count());
  long long random_number = mt();
  if (random_number % 5000) return;
  std::vector<Vec2> empty_tiles;  
  for (int i = 0; i < tiles_.size(); ++i) {
    for (int j = 0; j < tiles_[i].size(); ++j) {
      if (tiles_[i][j]->is_empty()) {
        empty_tiles.push_back(Vec2(i, j));
      }
    }
  }
  if (empty_tiles.size() == 0) return;
  int number_tile = random_number % empty_tiles.size();
  int weapon_number = mt() % 2;
  std::vector<std::unique_ptr<WeaponTile>> mixed_weapon;
  mixed_weapon.push_back(std::make_unique<WeaponShotgunTile>());
  mixed_weapon.push_back(std::make_unique<WeaponRicochetTile>());
  this->set_tile(empty_tiles[number_tile], std::move(mixed_weapon[weapon_number])); // пока ток 1
}