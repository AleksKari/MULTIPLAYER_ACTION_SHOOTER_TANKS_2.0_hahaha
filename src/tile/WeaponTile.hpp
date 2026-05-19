#pragma once
#include <tile/Tile.hpp>
class Renderer;
class Vec2;

class WeaponTile : public Tile {
 public:
  sf::Texture texture_;

  WeaponTile() = default;
  virtual void Draw(Renderer& renderer, Vec2 pos) = 0;
  virtual void Interact(Player& player, Map& map, const Vec2& pos) = 0;
};