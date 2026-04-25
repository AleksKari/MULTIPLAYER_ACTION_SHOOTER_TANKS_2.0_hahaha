#pragma once
#include "../tile/EmptyTile.hpp"
#include "../render/render.hpp"
#include "../math/Vec2.h"

EmptyTile::EmptyTile() {
    texture_.loadFromFile("textures/grass.png");
}
bool EmptyTile::is_empty() {return true;}
void EmptyTile::draw(Renderer& renderer, Vec2 pos) {
    renderer.draw_tile(*this, pos);
}
EmptyTile::~EmptyTile() = default;