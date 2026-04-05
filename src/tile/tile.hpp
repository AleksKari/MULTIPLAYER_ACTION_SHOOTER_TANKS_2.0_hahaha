#pragma once

#include <SFML/Graphics.hpp>

class Tile {
  public:
    enum class Type {
        Empty,
        Wall,
        Breakable,
        Slow,
        Damage
    };    
    Tile(Type type = Type::Empty, const sf::IntRect& rect = sf::IntRect());
    bool isEmpty() const;
    bool isWall() const;
    bool isBreakable() const;
    bool isSlow() const;
    bool isDamage() const;
    Type type;
    sf::IntRect texture_rect;
};