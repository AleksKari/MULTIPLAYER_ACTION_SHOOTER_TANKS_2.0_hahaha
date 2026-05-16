#pragma once
#include "entity.hpp"
#include <math/Vec2.h>
#include <chrono>

class Collision;

class Projectile : public Entity {
  friend class Collision;

 private:
  Vec2 velocity_;
  Vec2 prev_position_;
  int hp_;
  int damage_;
  int size_;
  std::chrono::steady_clock::time_point created_;

 public:
  Projectile(Vec2 pos, Vec2 velocity, int damage_, int size = 6, int hp = 1);
  ~Projectile();
  Vec2 get_velocity() const;
  void update(float timediff);
  void kill();
  void on_wall_collision();
  void take_damage(int damage);
  int get_damage() const;
  bool is_dead() const;
  bool can_ricochet() const;
  double lifetime() const;
};
