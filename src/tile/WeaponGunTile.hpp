#pragma once
#include <math/Vec2.h>

#include <render/Render.hpp>
#include <tile/EmptyTile.hpp>
#include <tile/WeaponTile.hpp>

class WeaponGunTile : public WeaponTile {
 public:
  sf::Texture texture_;

  WeaponGunTile() {
    texture_.loadFromFile("textures/stone.png");  // должна быть своя текстура
  }
  bool IsWeapon() override { return true; }
  void Draw(Renderer& render, Vec2 pos) override {
    render.DrawTile(*this, pos);
  }
  void Interact(Player& player, Map& map, const Vec2& pos) override {
    map.SetTile(pos, std::make_unique<EmptyTile>());
  }
};
