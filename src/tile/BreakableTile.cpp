#include "BreakableTile.hpp"
#include "../render/render.hpp"
#include "../math/Vec2.h"

BreakableTile::BreakableTile() {
    texture_.loadFromFile("textures/barrel_32x32.png"); //должна быть своя текстура
}
bool BreakableTile::is_breakable() {return true;}
bool BreakableTile::is_not_broke() {return hp_ > 0;}
void BreakableTile::draw(Renderer& renderer, Vec2 pos) {
    renderer.draw_tile(*this, pos);
}
BreakableTile::~BreakableTile() = default;