#pragma once

#include <SFML/Graphics.hpp>


// требует полной переработка. Каждый тип tile должен обрадать своими уникальными характеристиками(жизни, урон, собственная текстура)
class Tile {
  public:   
    virtual bool is_empty() {return false;}
    virtual bool is_wall() {return false;}
    virtual bool is_breakable() {return false;}
    virtual bool is_slow() {return false;}
    virtual bool is_damage() {return false;}
    virtual bool is_weapon() {return false;}
};

/*
Empty,
Wall,
Breakable,
Slow,
Damage,
Weapon
*/