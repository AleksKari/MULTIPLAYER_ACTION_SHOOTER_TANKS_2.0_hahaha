#pragma once
#include "entity.hpp"
#include "weapon.hpp"
class Character : public Entity {
 protected:
  Weapon weapon_;
 public:
  Character(float cord_x, float cord_y, int hp, float speed, Weapon weapon);
  ~Character() = default;
  void update(float timediff) override;
  void draw(sf::RenderWindow& window) const override;
  virtual void move(float timediff) = 0;
  virtual void attack();
  Weapon GetWeapon() const;
};