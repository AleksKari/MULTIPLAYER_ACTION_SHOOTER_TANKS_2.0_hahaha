#pragma once

class Weapon {
  public:
    virtual ~Weapon() = default;
    virtual void shoot(Map& map, Entity& owner) = 0;
  protected:
    int damage_ = 0;
    double cooldown = 0.8;
    double cooldown_timer = 0;
};
