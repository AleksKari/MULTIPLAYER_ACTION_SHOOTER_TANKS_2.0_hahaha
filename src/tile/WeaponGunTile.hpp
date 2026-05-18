#pragma once
#include <math/Vec2.h>

#include <render/Render.hpp>

#include <tile/WeaponTile.hpp>

class WeaponGunTile : public WeaponTile {
 public:
  sf::Texture texture_;

  WeaponGunTile() {
    texture_.loadFromFile("textures/stone.png");  // должна быть своя текстура
  }
  bool is_weapon() { return true; }
  void draw(Renderer& render, Vec2 pos) { render.draw_tile(*this, pos); }
  void interact(Player& player, Map& map, const Vec2& pos) override {
    map.set_tile(pos, std::make_unique<EmptyTile>());
  }
};
