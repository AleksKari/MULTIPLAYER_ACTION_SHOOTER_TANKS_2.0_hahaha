#pragma once
#include <tile/Tile.hpp>

class SlowTile : public Tile {
 public:
  sf::Texture texture_;

  SlowTile();
  bool is_slow() override;
  void draw(Renderer& renderer, Vec2 pos) override;
  void interact(Player& player, Map& map, const Vec2& pos) override;
};