#pragma once
#include <math/Vec2.h>

#include <render/Render.hpp>
#include <tile/EmptyTile.hpp>
#include <tile/WeaponTile.hpp>
#include <weapon/RicochetGun.hpp>

class WeaponRicochetTile : public WeaponTile {
 public:
  sf::Texture texture_;

  WeaponRicochetTile() {
    texture_.loadFromFile("textures/ricochet.png");  // должна быть своя
                                                     // текстура
  }
  void draw(Renderer& render, Vec2 pos) override {
    render.draw_tile(*this, pos);
  }
  void interact(Player& player, Map& map, const Vec2& pos) override {
    player.SetWeapon(std::make_unique<RicochetGun>());
    map.set_tile(pos, std::make_unique<EmptyTile>());
  }
};