#pragma once
#include "Entity.hpp"
#include <weapon/Weapon.hpp>
#include <memory>

class Weapon;
class Map;

class Player : public Entity {
 private:
  Vec2 mouse_pos_;
  Vec2 prev_position_;
  float speed_;
  int size_ = TILESIZE;
  std::unique_ptr<Weapon> weapon_;
  sf::Texture texture_;

 public:
  int max_hp;
  int hp;

  Player(Vec2 pos, int hp, float speed, std::unique_ptr<Weapon> weapon);
  Vec2 dir() const;
  void move(float timediff);
  void attack(Map& map);
  void kill();
  void on_wall_collision();
  void take_damage(int damage);
  void update(float timediff);
  void set_mouse(sf::Vector2i mouse);
  void set_weapon(std::unique_ptr<Weapon> weapon);
  bool is_dead() const;
};
