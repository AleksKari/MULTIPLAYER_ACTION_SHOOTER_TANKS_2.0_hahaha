#include <math/Vec2.h>

#include <Player.hpp>
#include <render/Render.hpp>
#include <tile/DamageTile.hpp>

DamageTile::DamageTile() { texture_.loadFromFile("textures/damage.png"); }
bool DamageTile::IsDamage() { return true; }
void DamageTile::Draw(Renderer& renderer, Vec2 pos) {
  renderer.DrawTile(*this, pos);
}
DamageTile::~DamageTile() = default;
void DamageTile::Interact(Player& player, Map& map, const Vec2& pos) {
  player.TakeTileDamage(damage_);
}