#pragma once
#include <math/Vec2.h>

#include <render/Render.hpp>
#include <weapon/RicochetGun.hpp>

#include <tile/WeaponTile.hpp>

class WeaponRicochetTile : public WeaponTile {
 public:
  sf::Texture texture_;

  WeaponRicochetTile() {
    texture_.loadFromFile("textures/ricochet.png");  // должна быть своя
                                                     // текстура
  }
  void draw(Renderer& render, Vec2 pos) { render.draw_tile(*this, pos); }
  void interact(Player& player, Map& map, const Vec2& pos) override {
    player.set_weapon(std::make_unique<RicochetGun>());
    map.set_tile(pos, std::make_unique<EmptyTile>());
  }
};