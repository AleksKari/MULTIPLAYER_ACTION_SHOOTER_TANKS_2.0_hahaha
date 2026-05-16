#pragma once
#include <tile/tile.hpp>

class DamageTile : public Tile {
  public:
    sf::Texture texture_;
    int damage_ = 5;
    DamageTile();
    bool is_damage() override;
    void draw(Renderer& renderer, Vec2 pos) override;
    void interact(Player& player, Map& map, const Vec2& pos) override;
    ~DamageTile();
};