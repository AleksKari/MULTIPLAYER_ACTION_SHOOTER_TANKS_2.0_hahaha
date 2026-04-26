#include "../tile/tile.hpp"

class BreakableTile : public Tile {
  private:
    int hp_ = 50;
  public:
    sf::Texture texture_;
    BreakableTile();
    bool is_breakable() override;
    bool is_not_broke();
    void draw(Renderer& renderer, Vec2 pos) override;
    ~BreakableTile();
};