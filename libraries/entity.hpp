#pragma once
#include <SFML/Graphics.hpp>
#include "../src/math/Vec2.h"


class Entity {
 public:
  Vec2 position;
  int damage = 0;
  int size;
  Entity();
  Entity(Vec2 pos, int damage_, int size = 32);
  virtual ~Entity() = default;
  virtual void update(float timediff) = 0;
  virtual void draw(sf::RenderWindow& window) const = 0;
  virtual bool isDead() const = 0;
  virtual void kill() = 0;
  virtual void on_wall_collision() = 0;
  virtual void take_damage(int damage) = 0;
};