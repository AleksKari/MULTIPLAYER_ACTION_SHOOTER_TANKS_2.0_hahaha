#pragma once

#include <SFML/Graphics.hpp>

class Renderer;
class Vec2;

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
};
