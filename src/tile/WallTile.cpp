#include "../tile/WallTile.hpp"
#include "../render/render.hpp"
#include "../math/Vec2.h"

WallTile::WallTile() {
  texture_.loadFromFile("textures/stone.png");
}
bool WallTile::is_wall() {return true;}
void WallTile::draw(Renderer& renderer, Vec2 pos) {
    renderer.draw_tile(*this, pos);
}
WallTile::~WallTile() = default;