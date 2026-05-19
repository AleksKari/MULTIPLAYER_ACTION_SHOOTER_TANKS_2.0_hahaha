#pragma once
#include <tile/Tile.hpp>

class SlowTile : public Tile {
 public:
  sf::Texture texture_;

  SlowTile();
  bool IsSlow() override;
  void Draw(Renderer& renderer, Vec2 pos) override;
  void Interact(Player& player, Map& map, const Vec2& pos) override;
};