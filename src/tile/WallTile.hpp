#include "tile.hpp"

class WallTile : public Tile {
  public:
    sf::Texture texture_;
    WallTile() {
        texture_.loadFromFile("textures/map.png"); //должна быть своя текстура
    }
    bool is_wall() override {return true;}
};