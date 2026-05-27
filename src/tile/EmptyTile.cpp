#include <math/Vec2.h>

#include <render/Render.hpp>
#include <tile/EmptyTile.hpp>

EmptyTile::EmptyTile() { texture_.loadFromFile("textures/grass.png"); }
bool EmptyTile::IsEmpty() { return true; }
void EmptyTile::Draw(Renderer& renderer, Vec2 pos) {
  renderer.DrawTile(*this, pos);
}
EmptyTile::~EmptyTile() = default;
void EmptyTile::Interact(Player& player, Map& map, const Vec2& pos) {}