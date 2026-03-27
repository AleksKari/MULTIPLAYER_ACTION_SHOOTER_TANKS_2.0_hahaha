#pragma once
#include "entity.hpp"
#include "../src/math/Vec2.h"

class Projectile : public Entity {
  int damage_;
  Vec2 velocity_;
 public:
  Projectile(Vec2 pos, Vec2 velocity, int damage_);
  void update(float timediff) override;
  void draw(sf::RenderWindow& window) const override;
  int GetDamage() const;
  Vec2 GetVelocity() const;
};
