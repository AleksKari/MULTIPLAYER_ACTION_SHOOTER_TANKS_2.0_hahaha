#pragma once

#include <SFML/Graphics.hpp>


// требует полной переработка. Каждый тип tile должен обрадать своими уникальными характеристиками(жизни, урон, собственная текстура)
class Tile {
  public:
    enum class Type {
        Empty,
        Wall,
        Breakable,
        Slow,
        Damage,
        Weapon
    };    
    Tile(Type type = Type::Empty, const sf::IntRect& rect = sf::IntRect());
    bool isEmpty() const;
    bool isWall() const;
    bool isBreakable() const;
    bool isSlow() const;
    bool isDamage() const;
    bool isWeapon() const;
    Type type;
    sf::IntRect texture_rect;
};