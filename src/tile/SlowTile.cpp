#include <math/Vec2.h>

#include <Player.hpp>
#include <render/Render.hpp>
#include <tile/SlowTile.hpp>

SlowTile::SlowTile() { texture_.loadFromFile("textures/slow.png"); }
bool SlowTile::IsSlow() { return true; }
void SlowTile::Draw(Renderer& renderer, Vec2 pos) {
  renderer.DrawTile(*this, pos);
}
void SlowTile::Interact(Player& player, Map& map, const Vec2& pos) {
  player.SlowDown();
}