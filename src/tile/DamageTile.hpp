#pragma once
#include <tile/Tile.hpp>

const int DAMAGE_FROM_TILE = 5;

class DamageTile : public Tile {
 public:
  sf::Texture texture_;
  int damage_ = DAMAGE_FROM_TILE;

  DamageTile();
  bool IsDamage() override;
  void Draw(Renderer& renderer, Vec2 pos) override;
  void Interact(Player& player, Map& map, const Vec2& pos) override;
  ~DamageTile();
};