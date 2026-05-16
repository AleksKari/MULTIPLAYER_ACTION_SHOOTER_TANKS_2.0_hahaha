#pragma once
#include "entity.hpp"
#include <weapon/weapon.hpp>
#include <memory>

class Weapon;
class Map;

class Player : public Entity {
 public:
  int max_hp_;
  int hp_;
  Player(Vec2 pos, int hp, float speed, std::unique_ptr<Weapon> weapon);
  void move(float timediff);
  Vec2 dir() const;
  void attack(Map& map);
  bool isDead() const;
  void kill();
  void on_wall_collision();
  void take_damage(int damage);
  void update(float timediff);
  void set_mouse(sf::Vector2i mouse);
  void set_weapon(std::unique_ptr<Weapon> weapon);
 private:
  Vec2 mouse_pos_;
  Vec2 prev_position_;
  float speed_;
  std::unique_ptr<Weapon> weapon_;
  bool alive = true;
  int size_ = 32;
  sf::Texture texture_;
};
