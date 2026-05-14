#pragma once

#include <SFML/Graphics.hpp>

class Renderer;
class Vec2;
class Player;
class Map;

// требует полной переработка. Каждый тип tile должен обрадать своими уникальными характеристиками(жизни, урон, собственная текстура)
class Tile {
  public: 
    virtual ~Tile() = default;
    virtual bool is_empty() {return false;}
    virtual bool is_wall() {return false;}
    virtual bool is_breakable() {return false;}
    virtual bool is_slow() {return false;}
    virtual bool is_damage() {return false;}
    virtual bool is_weapon() {return false;}
    virtual void draw(Renderer& renderer, Vec2 pos) = 0;
    virtual void interact(Player& player, Map& map, const Vec2& pos) = 0;
};
/*
Empty,
Wall,
Breakable,
Slow,
Damage,
Weapon
*/