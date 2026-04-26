#include "WeaponTile.hpp"
#include "../render/render.hpp"
#include "../math/Vec2.h"

class WeaponShotgunTile : public WeaponTile {
  public:
    sf::Texture texture_;
    WeaponShotgunTile() {
        texture_.loadFromFile("textures/shotgun.png"); //должна быть своя текстура
    }
    void draw(Renderer& render, Vec2 pos) {
      render.draw_tile(*this, pos);
    }
};