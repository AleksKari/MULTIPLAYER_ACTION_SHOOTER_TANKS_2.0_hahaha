#pragma once
#include "../tile/tile.hpp"

class SlowTile : public Tile {
  public:
    sf::Texture texture_;
    int slowdown = 5;
    int timedown = 5;
    SlowTile();
    bool is_slow() override;
    void draw(Renderer& renderer, Vec2 pos) override;
    void interact(Player& player, Map& map, const Vec2& pos) override;
};