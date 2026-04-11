#pragma once
#include "map/map.hpp"
#include "player.hpp"

class Map;
class Player;

class Weapon {
  public:
    virtual ~Weapon();
    virtual void shoot(Map& map, Player& player, int damage) = 0;
  protected:
    int damage_ = 0;
    double cooldown = 0.8;
    double cooldown_timer = 0;
};
