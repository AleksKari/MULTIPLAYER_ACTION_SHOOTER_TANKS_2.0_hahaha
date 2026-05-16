#pragma once
#include "entity.hpp"
#include <math/Vec2.h>
#include <chrono>

class Projectile : public Entity {
  public:
    Vec2 velocity_;
    Vec2 prev_position_;
    int hp_;
    int damage_;
    int size_;
    bool dead = false;
    std::chrono::steady_clock::time_point created;
    Projectile(Vec2 pos, Vec2 velocity, int damage_, int size = 6, int hp = 1);
    ~Projectile();
    void update(float timediff) override;
    int GetDamage() const;
    Vec2 GetVelocity() const;
    bool isDead() const;
    void kill();
    void on_wall_collision();
    void take_damage(int damage);
    double lifetime() const;
};
