#pragma once
#include <Player.hpp>
#include <map/Map.hpp>

class Map;
class Player;

const double DEFAULT_COOLDOWN = 0.8;

class Weapon {
 public:
  virtual ~Weapon() = default;
  virtual void shoot(Map& map, Player& player, int damage) = 0;
  bool no_ammo() const { return shots_left_ == 0; }

 protected:
  ssize_t shots_left_ = -1;  // бесконечно
  int damage_ = 0;
  double cooldown_ = DEFAULT_COOLDOWN;
  sf::Clock shot_clock_;
};
