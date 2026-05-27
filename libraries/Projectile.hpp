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
  Vec2 GetVelocity() const;
  void Update(float timediff);
  void Kill();
  void OnWallCollision();
  void TakeDamage(int damage);
  int GetDamage() const;
  int GetHp() const;
  bool IsDead() const;
  bool CanRicochet() const;
  double Lifetime() const;
  bool CrossedTileX() const;
  bool CrossedTileY() const;
  void Reflect(bool by_x, bool by_y);
};
