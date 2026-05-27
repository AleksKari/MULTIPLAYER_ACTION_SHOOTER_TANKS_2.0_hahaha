#pragma once
#include <tile/Tile.hpp>

class WallTile : public Tile {
 public:
  sf::Texture texture_;

  WallTile();
  bool IsWall() override;
  void Draw(Renderer& renderer, Vec2 pos) override;
  void Interact(Player& player, Map& map, const Vec2& pos) override;
  ~WallTile();
};