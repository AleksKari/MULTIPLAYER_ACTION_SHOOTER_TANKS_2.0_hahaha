#pragma once
#include "Entity.hpp"
#include <weapon/Weapon.hpp>
#include <memory>
#include <string>
#include <SFML/Network.hpp>

class Weapon;
class Map;

class Player : public Entity {
 private:
  Vec2 mouse_pos_;
  Vec2 prev_position_;
  float speed_;
  float slow_coeff_ = 1.0f;
  float tile_damage_cooldown_ = 0.0f;
  int size_ = TILESIZE;
  std::unique_ptr<Weapon> weapon_;
  sf::Texture texture_;
  sf::Sprite sprite_;

 public:
  int max_hp;
  int hp;
  sf::Uint32 network_id = 0;
  bool is_local = false;
  bool input_w = false, input_a = false, input_s = false, input_d = false;

  sf::Sprite& getSprite() { return sprite_; }
  const sf::Sprite& getSprite() const { return sprite_; }

  Player(Vec2 pos, int hp, float speed, std::unique_ptr<Weapon> weapon);
  Vec2 dir() const;
  void move(float timediff);
  void attack(Map& map);
  void kill();
  void on_wall_collision();
  void take_damage(int damage);
  void update(float timediff);
  void set_mouse(sf::Vector2i mouse);
  void loadSkin(const std::string& path);
  void set_weapon(std::unique_ptr<Weapon> weapon);
  bool is_dead() const;
  void slow_down();
  void take_tile_damage(int damage);

  void serialize(sf::Packet& packet) const;
  void deserialize(sf::Packet& packet);
  void set_input(bool w, bool a, bool s, bool d);
};
