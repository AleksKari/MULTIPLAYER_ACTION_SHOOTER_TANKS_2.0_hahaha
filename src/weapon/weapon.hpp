#pragma once
#include "map/map.hpp"
#include "player.hpp"

class Map;
class Player;

class Weapon {
 public:
  virtual ~Weapon();
  virtual void shoot(Map& map, Player& player, int damage) = 0;
  bool no_ammo() const { return shots_left_ == 0; }
 protected:
  int shots_left_ = -1; // бесконечно
  int damage_ = 0;
  double cooldown = 0.8;
  sf::Clock shot_clock;
};
