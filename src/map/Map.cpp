#include <iostream>
#include <chrono>
#include <random>
#include <variant>
#include <concepts>
#include <type_traits>
#include "Map.hpp"
#include <collision/Collision.hpp>
#include <weapon/Gun.hpp>
#include <weapon/Shotgun.hpp>
#include <tile/EmptyTile.hpp>
#include <tile/WallTile.hpp>
#include <tile/DamageTile.hpp>
#include <tile/SlowTile.hpp>
#include <tile/WeaponShotgunTile.hpp>
#include <tile/WeaponRicochetTile.hpp>
#include <render/Render.hpp>
#include <Projectile.hpp>
#include <Player.hpp>


const int RANDOM_GENERATE_INDEX = 5000;
const int WEAPON_CNT = 2;

static bool is_wall_tile(int i, int j) {
  return ((i == 26 || i == 27 || i == 31 || i == 32) && (j == 13 || j == 14 || j == 18 || j == 19)) ||
         (i >= 3 && i <= 8 && j >= 3 && j <= 4) || (i >= 3 && i <= 4 && j >= 3 && j <= 8) ||
         (i >= 50 && i <= 55 && j >= 3 && j <= 4) || (i >= 54 && i <= 55 && j >= 3 && j <= 8) ||
         (i >= 3 && i <= 8 && j >= 28 && j <= 29) || (i >= 3 && i <= 4 && j >= 24 && j <= 29) ||
         (i >= 50 && i <= 55 && j >= 28 && j <= 29) || (i >= 54 && i <= 55 && j >= 24 && j <= 29) ||
         (i >= 12 && i <= 20 && (j == 8 || j == 9)) || (i >= 38 && i <= 46 && (j == 8 || j == 9)) ||
         (i >= 12 && i <= 20 && (j == 23 || j == 24)) || (i >= 38 && i <= 46 && (j == 23 || j == 24)) ||
         (i == 22 && j >= 6 && j <= 12) || (i >= 22 && i <= 25 && j == 12) ||
         (i == 36 && j >= 6 && j <= 12) || (i >= 33 && i <= 36 && j == 12) ||
         (i == 22 && j >= 20 && j <= 26) || (i >= 22 && i <= 25 && j == 20) ||
         (i == 36 && j >= 20 && j <= 26) || (i >= 33 && i <= 36 && j == 20);
}

static bool is_damage_sheep(int i, int j) {
  return (i >= 28 && i <= 30 && j >= 15 && j <= 17) ||
         (i >= 14 && i <= 18 && (j == 2 || j == 3)) ||
         (i >= 40 && i <= 44 && (j == 2 || j == 3)) ||
         (i >= 14 && i <= 18 && (j == 29 || j == 30)) ||
         (i >= 40 && i <= 44 && (j == 29 || j == 30));
}

static bool is_swamp(int i, int j) {
  return (i >= 6  && i <= 10 && j >= 12 && j <= 16) ||
         (i >= 48 && i <= 52 && j >= 12 && j <= 16) ||
         (i >= 6  && i <= 10 && j >= 18 && j <= 22) ||
         (i >= 48 && i <= 52 && j >= 18 && j <= 22);
}

// конструктор карты
Map::Map(int width, int height) : width_(width), height_(height) {
  tiles_.resize(width_);
  for (auto& column : tiles_) column.resize(height_);
  for (int i = 0; i < width_; ++i) {
    for (int j = 0; j < height_; ++j) {
      if (is_wall_tile(i, j))
        tiles_[i][j] = std::make_unique<WallTile>();
      else if (is_damage_sheep(i, j))
        tiles_[i][j] = std::make_unique<DamageTile>();
      else if (is_swamp(i, j))
        tiles_[i][j] = std::make_unique<SlowTile>();
      else
        tiles_[i][j] = std::make_unique<EmptyTile>();
    }
  }
}

// обновление карты
void Map::update(double diff) {
  for (auto& ent : entities_) { ent->update(diff); }
  for (auto& pr : projectiles_) { pr->update(diff); }
  this->generate_weapon();
  //Collision::resolve(*this);
}

//отслеживаем игроков
void Map::spawn_entity(Player* ent) {
  entities_.push_back(std::unique_ptr<Player>(std::move(ent)));
}

//отслеживаем снаряды
void Map::spawn_projectile(Vec2 pos, Vec2 vel, int damage, int size, int hp, const Player* source) {
  projectiles_.push_back(std::make_unique<Projectile>(pos, vel, damage, size, hp, source));
}


//проверка границ карты
bool Map::is_bound(double pos_x, double pos_y) const {
  return pos_x < 0 || pos_y < 0 ||
      pos_x >= width_ || pos_y >= height_;
}

// не доходило до тайла добавил проверку
bool Map::is_wall(double x, double y) const {
  if (is_bound(x / TILESIZE, y / TILESIZE)) return true;  // край карты = стена
  return tiles_[x / TILESIZE][y / TILESIZE]->is_wall();
}

//пустой 
bool Map::is_empty(double pos_x, double pos_y) const {
  if (is_bound(pos_x / TILESIZE, pos_y / TILESIZE)) return false;
  return tiles_[pos_x / TILESIZE][pos_y / TILESIZE]->is_empty();
}


//замедляющий
bool Map::is_slow(double pos_x, double pos_y) const {
  if (is_bound(pos_x / TILESIZE, pos_y / TILESIZE)) return false;
  return tiles_[pos_x / TILESIZE][pos_y / TILESIZE]->is_slow();
}

// наносящий урон
bool Map::is_damage(double x, double y) const {
  if (is_bound(x / TILESIZE, y / TILESIZE)) return false;
  return tiles_[x / TILESIZE][y / TILESIZE]->is_damage();
}

// отрисовка объектов 
void Map::render(Renderer& renderer) const {

  for (size_t y = 0; y < height_; ++y) {
    for (size_t x = 0; x < width_; ++x) {               // 2. только стены
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

  if (random_number % RANDOM_GENERATE_INDEX) return;

  std::vector<Vec2> empty_tiles;

  for (size_t i = 0; i < tiles_.size(); ++i) {
    for (size_t j = 0; j < tiles_[i].size(); ++j) {
      if (tiles_[i][j]->is_empty()) {
        empty_tiles.push_back(Vec2(i, j));
      }
    }
  }

  if (empty_tiles.size() == 0) return;

  int number_tile = random_number % empty_tiles.size();
  int weapon_number = mt() % WEAPON_CNT;

  std::vector<std::unique_ptr<WeaponTile>> mixed_weapon;

  mixed_weapon.push_back(std::make_unique<WeaponShotgunTile>());
  mixed_weapon.push_back(std::make_unique<WeaponRicochetTile>());

  this->set_tile(empty_tiles[number_tile], std::move(mixed_weapon[weapon_number]));
}
void Map::update_remote_player(sf::Uint32 id, sf::Packet& packet) {
    for (auto& entity : entities_) {
        if (entity->network_id == id) {
            entity->deserialize(packet);
            return;
        }
    }
}

void Map::spawn_remote_projectile(sf::Packet& packet) {
    float px, py, vx, vy; int dmg, sz, hp; sf::Uint32 owner_id;
    packet >> px >> py >> vx >> vy >> dmg >> sz >> hp >> owner_id;
    
    const Player* src = nullptr;
    for (auto& entity : entities_) {
        if (entity->network_id == owner_id) { src = entity.get(); break; }
    }
    spawn_projectile(Vec2(px, py), Vec2(vx, vy), dmg, sz, hp, src);
}

void Map::update_player_hp(sf::Uint32 id, int hp) {
    for (auto& entity : entities_) {
        if (entity->network_id == id) {
            entity->hp = hp;
            if (hp <= 0) entity->kill();
            return;
        }
    }
}

void Map::spawn_weapon_at(int x, int y, int type) {
    // Временная заглушка для генерации
}
