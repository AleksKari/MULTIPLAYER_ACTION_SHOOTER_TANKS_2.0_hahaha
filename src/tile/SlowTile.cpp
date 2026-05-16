#include "SlowTIle.hpp"
#include <render/render.hpp>
#include <math/Vec2.h>

SlowTile::SlowTile() {
  texture_.loadFromFile("textures/map.png"); //должна быть своя текстура
}
bool SlowTile::is_slow() {return true;}
void SlowTile::draw(Renderer& renderer, Vec2 pos) {
  renderer.draw_tile(*this, pos);
}
void SlowTile::interact(Player& player, Map& map, const Vec2& pos) {
    //player.slow_down();
}