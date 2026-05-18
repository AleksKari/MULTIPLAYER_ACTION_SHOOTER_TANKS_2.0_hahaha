#pragma once
#include <math/Vec2.h>

#include <render/Render.hpp>
#include <weapon/Shotgun.hpp>

#include <tile/WeaponTile.hpp>

class WeaponShotgunTile : public WeaponTile {
 public:
  sf::Texture texture_;

  WeaponShotgunTile() {
    texture_.loadFromFile("textures/shotgun.png");  // должна быть своя текстура
  }
  void draw(Renderer& render, Vec2 pos) { render.draw_tile(*this, pos); }
  void interact(Player& player, Map& map, const Vec2& pos) override {
    player.set_weapon(std::make_unique<Shotgun>());
    map.set_tile(pos, std::make_unique<EmptyTile>());
  }
};
