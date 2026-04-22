#include "tile.hpp"

class WeaponTile : public Tile {
  public:
    sf::Texture texture_;
    WeaponTile() {
        texture_.loadFromFile("textures/map.png"); //должна быть своя текстура
    }
    bool is_weapon() override {return true;}
};