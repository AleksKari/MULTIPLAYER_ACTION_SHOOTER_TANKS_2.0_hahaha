#include <tile/WallTile.hpp>
#include <render/render.hpp>
#include <math/Vec2.h>
#include <player.hpp>

WallTile::WallTile() {
  texture_.loadFromFile("textures/stone.png");
}
bool WallTile::is_wall() {return true;}
void WallTile::draw(Renderer& renderer, Vec2 pos) {
    renderer.draw_tile(*this, pos);
}
WallTile::~WallTile() = default;

void WallTile::interact(Player& player, Map& map, const Vec2& pos) {
  player.on_wall_collision();
}