#pragma once
#include <SFML/Graphics.hpp>


class Entity {
 protected:
  float cord_x_;
  float cord_y_;
  int hp_;
  float speed_;
 public:
  Entity(float cord_x,float cord_y, int hp, float speed);
  virtual ~Entity() = default;
  virtual void update(float timediff) = 0;
  virtual void draw(sf::RenderWindow& window) const = 0;
  virtual void takeDamage(int numb);
  float GetX() const;
  float GetY() const;
  int GetHp() const;
  float GetSpeed() const;
  bool IsAlive() const;
};