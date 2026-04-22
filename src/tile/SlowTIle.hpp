#include "tile.hpp"

class SlowTile : public Tile {
  public:
    sf::Texture texture_;
    int slowdown = 5;
    int timedown = 5;
    SlowTile() {
        texture_.loadFromFile("textures/map.png"); //должна быть своя текстура
    }
    bool is_slow() override {return true;}
};