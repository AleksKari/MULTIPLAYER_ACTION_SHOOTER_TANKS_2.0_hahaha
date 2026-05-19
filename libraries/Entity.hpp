#pragma once

#include <math/Vec2.h>

#include <SFML/Graphics.hpp>
#include <concepts>

const int TILESIZE = 32;
const float NUMBER_TO_DIV = 2.0F;

class Entity {
 private:
  sf::IntRect texture_rect_{0, 0, TILESIZE, TILESIZE};

 public:
  Vec2 position;
  double cornrotate = 0.0;  // угол в градусах
  int damage = 0;
  int size;

  Entity() : size(TILESIZE) {}
  Entity(Vec2 pos, int dmg, int siz = TILESIZE)
      : position(pos), damage(dmg), size(siz) {}
  ~Entity() = default;
};

template <typename T>
concept is_entity = std::derived_from<T, Entity>;