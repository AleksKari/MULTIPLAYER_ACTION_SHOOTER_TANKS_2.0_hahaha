#include "WeaponTile.hpp"
#include "../render/render.hpp"
#include "../math/Vec2.h"

class WeaponRicochetTile : public WeaponTile {
  public:
    sf::Texture texture_;
    WeaponRicochetTile() {
        texture_.loadFromFile("textures/ricochet.png"); //должна быть своя текстура
    }
    void draw(Renderer& render, Vec2 pos) {
      render.draw_tile(*this, pos);
    }
};