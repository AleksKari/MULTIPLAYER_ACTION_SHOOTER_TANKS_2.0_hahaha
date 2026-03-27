#pragma once
#include <SFML/Graphics.hpp>
#include "../src/math/Vec2.h"


class Entity {
 public:
  Vec2 pos;
  Entity(Vec2 pos);
  virtual ~Entity() = default;
  virtual void update(float timediff) = 0;
  virtual void draw(sf::RenderWindow& window) const = 0;
};