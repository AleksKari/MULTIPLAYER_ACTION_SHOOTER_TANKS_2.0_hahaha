#include <math/Vec2.h>

#include <Player.hpp>
#include <render/Render.hpp>
#include <tile/WallTile.hpp>

WallTile::WallTile() { texture_.loadFromFile("textures/stone.png"); }
bool WallTile::IsWall() { return true; }
void WallTile::Draw(Renderer& renderer, Vec2 pos) {
  renderer.DrawTile(*this, pos);
}
WallTile::~WallTile() = default;

void WallTile::Interact(Player& player, Map& map, const Vec2& pos) {
  player.OnWallCollision();
}