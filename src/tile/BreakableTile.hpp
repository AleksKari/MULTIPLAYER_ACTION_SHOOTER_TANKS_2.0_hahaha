#include "tile.hpp"

class BreakableTile : public Tile {
  private:
    int hp_ = 50;
  public:
    sf::Texture texture_;
    BreakableTile() {
        texture_.loadFromFile("textures/map.png"); //должна быть своя текстура
    }
    bool is_breakable() override {return true;}
    bool is_not_broke() {return hp_ > 0;}
};