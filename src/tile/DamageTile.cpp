#include "DamageTile.hpp"
#include <render/Render.hpp>
#include <math/Vec2.h>
#include <Player.hpp>
    
DamageTile::DamageTile() {
    texture_.loadFromFile("textures/map.png"); //должна быть своя текстура
}
bool DamageTile::is_damage() {return true;}
void DamageTile::draw(Renderer& renderer, Vec2 pos) {
  renderer.draw_tile(*this, pos);
}
DamageTile::~DamageTile() = default;
void DamageTile::interact(Player& player, Map& map, const Vec2& pos) {
    player.take_damage(1);
}