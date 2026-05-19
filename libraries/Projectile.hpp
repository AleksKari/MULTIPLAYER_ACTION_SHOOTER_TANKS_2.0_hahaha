#pragma once
#include <math/Vec2.h>

#include <Entity.hpp>
#include <chrono>

class Player;

const int PROJECTILE_SIZE = 6;

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

  Projectile(Vec2 pos, Vec2 velocity, int damage_, int size = PROJECTILE_SIZE,
             int heatpoint = 1, const Player* source = nullptr);
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
  void reflect(bool by_x, bool by_y);
};
