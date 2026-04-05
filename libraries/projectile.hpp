#pragma once
#include "entity.hpp"
#include "../src/math/Vec2.h"

class Projectile : public Entity {
  public:
    int damage_;
    Vec2 velocity_;
    bool dead = false;
    Projectile(Vec2 pos, Vec2 velocity, int damage_, int size = 32);
    void update(float timediff) override;
    void draw(sf::RenderWindow& window) const override;
    int GetDamage() const;
    Vec2 GetVelocity() const;
    bool isDead() const;
    void kill();
};
