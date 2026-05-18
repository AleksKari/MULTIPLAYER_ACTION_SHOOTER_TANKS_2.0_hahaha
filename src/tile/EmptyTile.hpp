#pragma once
#include <tile/Tile.hpp>

class EmptyTile : public Tile {
 public:
  sf::Texture texture_;

  EmptyTile();
  bool is_empty() override;
  void draw(Renderer& renderer, Vec2 pos) override;
  void interact(Player& player, Map& map, const Vec2& pos) override;
  ~EmptyTile();
};