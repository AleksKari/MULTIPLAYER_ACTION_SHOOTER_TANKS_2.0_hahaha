#pragma once
#include <tile/Tile.hpp>

class EmptyTile : public Tile {
 public:
  sf::Texture texture_;

  EmptyTile();
  bool IsEmpty() override;
  void Draw(Renderer& renderer, Vec2 pos) override;
  void Interact(Player& player, Map& map, const Vec2& pos) override;
  ~EmptyTile();
};