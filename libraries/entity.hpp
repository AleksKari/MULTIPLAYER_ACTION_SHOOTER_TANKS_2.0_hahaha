#pragma once
#include <SFML/Graphics.hpp>
#include <math/Vec2.h>


class Entity {
 private:
  sf::IntRect texture_rect_{0, 0, 32, 32};

 public:
  Vec2 position;
  double cornrotate = 0.0;  // угол в рад
  int damage = 0;
  int size;

  Entity();
  Entity(Vec2 pos, int damage_, int size = 32);
  virtual ~Entity() = default;
  virtual void update(float timediff) = 0;
  virtual bool isDead() const = 0;
  virtual void kill() = 0;
  virtual void on_wall_collision() = 0;
  virtual void take_damage(int damage) = 0;
};