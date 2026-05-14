#pragma once
#include "../tile/tile.hpp"
class Renderer;
class Vec2;

class WeaponTile : public Tile {
  public:
    sf::Texture texture_;
    WeaponTile() = default;
    virtual void draw(Renderer& renderer, Vec2 pos) = 0;
    virtual void interact(Player& player, Map& map, const Vec2& pos) = 0;
};