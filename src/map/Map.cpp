#include <Player.hpp>
#include <Projectile.hpp>
#include <algorithm>
#include <chrono>
#include <collision/Collision.hpp>
#include <concepts>
#include <fstream>
#include <iostream>
#include <map/Map.hpp>
#include <random>
#include <render/Render.hpp>
#include <tile/DamageTile.hpp>
#include <tile/EmptyTile.hpp>
#include <tile/SlowTile.hpp>
#include <tile/WallTile.hpp>
#include <tile/WeaponRicochetTile.hpp>
#include <tile/WeaponShotgunTile.hpp>
#include <type_traits>
#include <variant>
#include <weapon/Gun.hpp>
#include <weapon/Shotgun.hpp>

#include "json.hpp"

using json = nlohmann::json;

const int RANDOM_GENERATE_INDEX = 5000;
const int WEAPON_CNT = 2;

void build_tiles(Map& map) {
  map.tiles_.clear();
  map.tiles_.resize(map.width_);

  json jsn;
  std::ifstream file("../textures/map.json");
  file >> jsn;

  const auto& layer = jsn["layers"][0];
  const auto& data = layer["data"];  // массив ID тайлов

  for (auto& column : map.tiles_) {
    column.resize(map.height_);
  }

  for (int i = 0; i < map.width_; ++i) {
    for (int j = 0; j < map.height_; ++j) {
      int index = data[(j * map.width_) + i];
      switch (index) {
        case 1:
          map.tiles_[i][j] = std::make_unique<EmptyTile>();
          break;
        case 2:
          map.tiles_[i][j] = std::make_unique<WallTile>();
          break;
        case 3:
          map.tiles_[i][j] = std::make_unique<DamageTile>();
          break;
        case 4:
          map.tiles_[i][j] = std::make_unique<SlowTile>();
          break;
        default:
          map.tiles_[i][j] = std::make_unique<EmptyTile>();
          break;
      }
    }
  }
}

// конструктор карты
Map::Map(int width, int height) : width_(width), height_(height) {
  build_tiles(*this);
}

void Map::reset_world() {
  entities_.clear();
  projectiles_.clear();
  removed_weapon_tiles_.clear();
  build_tiles(*this);
}

// обновление карты
void Map::update(double diff) {
  for (auto& ent : entities_) {
    ent->update(diff);
  }
  for (auto& projectile : projectiles_) {
    projectile->update(diff);
  }
  projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(),
                                    [](const auto& projectile) {
                                      return projectile->is_dead();
                                    }),
                     projectiles_.end());
}

// отслеживаем игроков
void Map::spawn_entity(Player* ent) {
  entities_.push_back(std::unique_ptr<Player>(std::move(ent)));
}

// отслеживаем снаряды
void Map::spawn_projectile(Vec2 pos, Vec2 vel, int damage, int size,
                           int heatpoint, const Player* source) {
  projectiles_.push_back(
      std::make_unique<Projectile>(pos, vel, damage, size, heatpoint, source));
}

// проверка границ карты
bool Map::is_bound(double pos_x, double pos_y) const {
  return pos_x < 0 || pos_y < 0 || pos_x >= width_ || pos_y >= height_;
}

// не доходило до тайла добавил проверку
bool Map::is_wall(double pos_x, double pos_y) const {
  if (is_bound(pos_x / TILESIZE, pos_y / TILESIZE)) {
    return true;
  }  // край карты = стена
  return tiles_[pos_x / TILESIZE][pos_y / TILESIZE]->is_wall();
}

// пустой
bool Map::is_empty(double pos_x, double pos_y) const {
  if (is_bound(pos_x / TILESIZE, pos_y / TILESIZE)) {
    return false;
  }
  return tiles_[pos_x / TILESIZE][pos_y / TILESIZE]->is_empty();
}

// замедляющий
bool Map::is_slow(double pos_x, double pos_y) const {
  if (is_bound(pos_x / TILESIZE, pos_y / TILESIZE)) {
    return false;
  }
  return tiles_[pos_x / TILESIZE][pos_y / TILESIZE]->is_slow();
}

// наносящий урон
bool Map::is_damage(double pos_x, double pos_y) const {
  if (is_bound(pos_x / TILESIZE, pos_y / TILESIZE)) {
    return false;
  }
  return tiles_[pos_x / TILESIZE][pos_y / TILESIZE]->is_damage();
}

// отрисовка объектов
void Map::render(Renderer& renderer) const {
  for (size_t cord_x = 0; cord_x < width_; ++cord_x) {  // 2. только стены
    for (size_t cord_y = 0; cord_y < height_; ++cord_y) {
      tiles_[cord_x][cord_y]->draw(renderer, Vec2(cord_x, cord_y));
    }
  }

  for (const auto& entity : entities_) {
    renderer.draw_entity(*entity);  // 3. танки
    renderer.draw_hp_bar(*entity);
  }

  for (const auto& proj : projectiles_) {
    renderer.draw_projectile(*proj);  // 4. пули
  }
}

void Map::spawn_weapon_at(int cord_x, int cord_y, int type) {
  if (cord_x < 0 || cord_y < 0 || cord_x >= width_ || cord_y >= height_) {
    return;
  }
  if (type == 0) {
    set_tile(Vec2(cord_x, cord_y), std::make_unique<WeaponShotgunTile>());
  } else if (type == 1) {
    set_tile(Vec2(cord_x, cord_y), std::make_unique<WeaponRicochetTile>());
  }
}

// генерация оружия
std::optional<std::tuple<int, int, int>> Map::generate_weapon() {
  std::mt19937 random_generate(
      std::chrono::steady_clock::now().time_since_epoch().count());
  int64_t random_number = random_generate();

  if ((random_number % RANDOM_GENERATE_INDEX) != 0) {
    return std::nullopt;
  }

  std::vector<Vec2> empty_tiles;

  for (size_t i = 0; i < tiles_.size(); ++i) {
    for (size_t j = 0; j < tiles_[i].size(); ++j) {
      if (tiles_[i][j]->is_empty()) {
        empty_tiles.push_back(Vec2(i, j));
      }
    }
  }

  if (empty_tiles.empty()) {
    return std::nullopt;
  }

  int number_tile = random_number % empty_tiles.size();
  int weapon_number = random_generate() % WEAPON_CNT;

  const Vec2& tile_pos = empty_tiles[number_tile];
  spawn_weapon_at(static_cast<int>(tile_pos.cord_x),
                  static_cast<int>(tile_pos.cord_y), weapon_number);
  return std::make_tuple(static_cast<int>(tile_pos.cord_x),
                         static_cast<int>(tile_pos.cord_y), weapon_number);
}
void Map::update_remote_player(sf::Uint32 user_id, sf::Packet& packet) {
  for (auto& entity : entities_) {
    if (entity->network_id == user_id) {
      entity->deserialize(packet);
      return;
    }
  }
}

void Map::spawn_remote_projectile(sf::Packet& packet) {
  float px, py, vx, vy;
  int dmg, sz, hp;
  sf::Uint32 owner_id;
  packet >> px >> py >> vx >> vy >> dmg >> sz >> hp >> owner_id;

  const Player* src = nullptr;
  for (auto& entity : entities_) {
    if (entity->network_id == owner_id) {
      src = entity.get();
      break;
    }
  }
  spawn_projectile(Vec2(px, py), Vec2(vx, vy), dmg, sz, hp, src);
}

void Map::update_player_hp(sf::Uint32 user_id, int heatpoint) {
  for (auto& entity : entities_) {
    if (entity->network_id == user_id) {
      entity->heatpoint = heatpoint;
      if (heatpoint <= 0) {
        entity->kill();
      }
      return;
    }
  }
}

std::vector<Vec2> Map::consume_removed_weapon_tiles() {
  std::vector<Vec2> removed;
  removed.swap(removed_weapon_tiles_);
  return removed;
}

void Map::serialize_game_state(sf::Packet& packet) const {
  packet << static_cast<sf::Uint16>(entities_.size());
  for (const auto& entity : entities_) {
    packet << entity->network_id << static_cast<float>(entity->position.cord_x)
           << static_cast<float>(entity->position.cord_y)
           << static_cast<float>(entity->cornrotate)
           << static_cast<sf::Int32>(entity->heatpoint);
  }

  packet << static_cast<sf::Uint16>(projectiles_.size());
  for (const auto& projectile : projectiles_) {
    Vec2 velocity = projectile->get_velocity();
    sf::Uint32 owner_id =
        (projectile->owner != nullptr) ? projectile->owner->network_id : 0;
    packet << static_cast<float>(projectile->position.cord_x)
           << static_cast<float>(projectile->position.cord_y)
           << static_cast<float>(velocity.cord_x)
           << static_cast<float>(velocity.cord_y) << projectile->get_damage()
           << projectile->size << projectile->get_hp() << owner_id;
  }
}

void Map::apply_game_state(sf::Packet& packet) {
  sf::Uint16 player_count = 0;
  packet >> player_count;

  for (sf::Uint16 i = 0; i < player_count; ++i) {
    sf::Uint32 user_id;
    float x = 0.0F;
    float y = 0.0F;
    float angle = 0.0F;
    sf::Int32 hp = 0;
    packet >> user_id >> x >> y >> angle >> hp;

    for (auto& entity : entities_) {
      if (entity->network_id != user_id) {
        continue;
      }
      entity->position = Vec2(x, y);
      entity->cornrotate = angle;
      entity->heatpoint = hp;
      if (entity->heatpoint <= 0) {
        entity->kill();
      }
      break;
    }
  }

  sf::Uint16 projectile_count = 0;
  packet >> projectile_count;
  projectiles_.clear();
  projectiles_.reserve(projectile_count);

  for (sf::Uint16 i = 0; i < projectile_count; ++i) {
    float px = 0.0F;
    float py = 0.0F;
    float vx = 0.0F;
    float vy = 0.0F;
    int dmg = 0;
    int sz = 0;
    int hp = 0;
    sf::Uint32 owner_id = 0;
    packet >> px >> py >> vx >> vy >> dmg >> sz >> hp >> owner_id;

    const Player* src = nullptr;
    for (const auto& entity : entities_) {
      if (entity->network_id == owner_id) {
        src = entity.get();
        break;
      }
    }
    spawn_projectile(Vec2(px, py), Vec2(vx, vy), dmg, sz, hp, src);
  }
}
