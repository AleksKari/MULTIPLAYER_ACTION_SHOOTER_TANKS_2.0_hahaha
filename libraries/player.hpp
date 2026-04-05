#pragma once
#include "entity.hpp"
#include "../src/weapon/weapon.hpp"

class Player : public Entity {
  public:
    Player(Vec2 pos, int hp, float speed, Weapon weapon, int size = 32);
    void move(float timediff);
    void attack();
    bool isDead() const;
    void kill();
    void on_wall_collision();
    void take_damage(int damage);
    void update(float timediff);
    void draw(sf::RenderWindow& window);
  private:
    int hp_;
    float speed_;
    Weapon weapon_;
    
};
