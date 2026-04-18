#pragma once
#include "entity.hpp"
#include "../src/math/Vec2.h"

class Projectile : public Entity {
  public:
    Vec2 velocity_;
    int damage_;
    int size_;
    bool dead = false;
    Projectile(Vec2 pos, Vec2 velocity, int damage_, int size = 32);
    ~Projectile();
    void update(float timediff) override;
    int GetDamage() const;
    Vec2 GetVelocity() const;
    bool isDead() const;
    void kill();
    void on_wall_collision();
    void take_damage(int damage);
};
