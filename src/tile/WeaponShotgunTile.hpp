#pragma once
#include "WeaponTile.hpp"
#include <render/render.hpp>
#include <math/Vec2.h>
#include <weapon/shotgun.hpp>

class WeaponShotgunTile : public WeaponTile {
 public:
  sf::Texture texture_;

  WeaponShotgunTile() {
    texture_.loadFromFile("textures/shotgun.png"); //должна быть своя текстура
  }
  void draw(Renderer& render, Vec2 pos) {
    render.draw_tile(*this, pos);
  }
  void interact(Player& player, Map& map, const Vec2& pos) override {
    player.set_weapon(std::make_unique<ShotGun>());
    map.set_tile(pos, std::make_unique<EmptyTile>());
  }
};

