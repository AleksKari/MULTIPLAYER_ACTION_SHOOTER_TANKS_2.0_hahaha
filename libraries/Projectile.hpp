#pragma once
#include "Entity.hpp"
#include <math/Vec2.h>
#include <chrono>

class Player;

class Projectile : public Entity {
 private:
  Vec2 velocity_;
  Vec2 prev_position_;
  int hp_;
  int damage_;
  int size_;
  std::chrono::steady_clock::time_point created_;

 public:
  const Player* owner;

  Projectile(Vec2 pos, Vec2 velocity, int damage_, int size = 6, int hp = 1, const Player* source = nullptr);
  ~Projectile();
  Vec2 get_velocity() const;
  void update(float timediff);
  void kill();
  void on_wall_collision();
  void take_damage(int damage);
  int get_damage() const;
  int get_hp() const;
  bool is_dead() const;
  bool can_ricochet() const;
  double lifetime() const;
  bool crossed_tile_x() const;
  bool crossed_tile_y() const;
  void reflect(bool x, bool y);
};
