#pragma once
#include <SFML/Graphics.hpp>
#include <math/Vec2.h>
#include <concepts>

const int TILESIZE = 32;

class Entity {
 private:
  sf::IntRect texture_rect_{0, 0, TILESIZE, TILESIZE};

 public:
  Vec2 position;
  double cornrotate = 0.0;  // угол в рад
  int damage = 0;
  int size;

  Entity() : position(Vec2()), size(TILESIZE)  {}
  Entity(Vec2 pos, int dmg, int sz = TILESIZE) : position(pos), damage(dmg), size(sz) {}
  ~Entity() = default;
};

template<typename T>
concept is_entity = std::derived_from<T, Entity>;