#pragma once
#include "tile.hpp"

class EmptyTile : public Tile {
  public:
    sf::Texture texture_;
    EmptyTile() {
        texture_.loadFromFile("textures/stone.png"); //должна быть своя текстура
    }
    bool is_empty() override {return true;}
};