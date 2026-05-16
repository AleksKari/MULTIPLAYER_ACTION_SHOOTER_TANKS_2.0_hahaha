#pragma once
#include <SFML/Graphics.hpp>
#include <math/Vec2.h>

const int TILESIZE = 32;

class Entity {
 private:
  sf::IntRect texture_rect_{0, 0, TILESIZE, TILESIZE};

 public:
  Vec2 position;
  double cornrotate = 0.0;  // угол в рад
  int damage = 0;
  int size;

  Entity();
  Entity(Vec2 pos, int damage_, int size = TILESIZE);
  ~Entity() = default;
};