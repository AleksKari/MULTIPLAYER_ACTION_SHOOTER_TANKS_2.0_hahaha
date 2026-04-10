#pragma once

class Weapon {
  public:
    Weapon() = default;
    virtual ~Weapon() = default;
    virtual void shoot(Map& map, Player& player, int damage);
  protected:
    int damage_ = 0;
    double cooldown = 0.8;
    double cooldown_timer = 0;
};
