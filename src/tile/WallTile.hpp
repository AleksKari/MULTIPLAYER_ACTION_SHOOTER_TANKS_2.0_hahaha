#pragma once
#include "../tile/tile.hpp"

class WallTile : public Tile {
  public:
    sf::Texture texture_;
    WallTile();
    bool is_wall() override ;
    void draw(Renderer& renderer, Vec2 pos) override;
    void interact(Player& player, Map& map, const Vec2& pos) override;
    ~WallTile();
};