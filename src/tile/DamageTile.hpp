#include "tile.hpp"

class DamageTile : public Tile {
  public:
    sf::Texture texture_;
    int damage_ = 5;
    DamageTile() {
        texture_.loadFromFile("textures/map.png"); //должна быть своя текстура
    }
    bool is_damage() override {return true;}
    void draw(Renderer& renderer, Vec2 pos) override {
      renderer.draw_tile(*this, pos);
    }
    ~DamageTile() = default;
};