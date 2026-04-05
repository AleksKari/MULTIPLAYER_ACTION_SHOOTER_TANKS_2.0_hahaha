#pragma once
#include "character.hpp"
#include <SFML/Window/Keyboard.hpp>

class Player : public Character {
public:
    Player(float cord_x, float cord_y, int hp, float speed, Weapon weapon, int size = 32);
    void move(float timediff) override;
    void attack() override;
    bool isDead() const;
    void kill();
    void on_wall_collision();
    void take_damage(int damage);

};
