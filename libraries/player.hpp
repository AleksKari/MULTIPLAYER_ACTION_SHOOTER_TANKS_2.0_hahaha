#pragma once
#include "character.hpp"
#include <SFML/Window/Keyboard.hpp>

class Player : public Character {
public:
    Player(float cord_x, float cord_y, int hp, float speed, Weapon weapon);
    void move(float timediff) override;
    void attack() override;
};
