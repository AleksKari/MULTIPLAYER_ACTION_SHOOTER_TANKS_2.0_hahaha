//класс отрисовки

#include "render.hpp"
#include <cmath>
#include <iostream>

Renderer::Renderer(int screen_width, int screen_height, int tile_size)
    : window_(sf::VideoMode(screen_width, screen_height), "Soul Knife Tanks"), tile_size_(tile_size) {
  if (!tile_texture.loadFromFile("textures/map.png")) {
    std::cout << "failed to load map\n";
  }
  if (!entity_texture.loadFromFile("textures/player.png")) {
    std::cout << "failed to load entity\n";
  }
  if (!projectile_texture.loadFromFile("textures/projectile.png")) {
    std::cout << "failed to load projectile\n";
  }
  tile_sprite.setTexture(tile_texture);
  entity_sprite.setTexture(entity_texture);
  entity_sprite.setOrigin(16, 16);  // центр 32×32 текстуры
  projectile_sprite.setTexture(projectile_texture);
}

void Renderer::draw_tile(const Tile& tile, int x, int y) {
  tile_sprite.setTextureRect(tile.texture_rect);
  tile_sprite.setPosition(x * tile_size_, y * tile_size_);
  window_.draw(tile_sprite);
}

void Renderer::draw_entity(const Entity& e) {
  entity_sprite.setPosition(e.position.x + 16, e.position.y + 16);
  entity_sprite.setRotation(e.cornrotate * 180.0 / M_PI);
  window_.draw(entity_sprite);
}

void Renderer::draw_projectile(const Projectile& p) {
  projectile_sprite.setPosition(p.position.x - 67, p.position.y - 53);
  window_.draw(projectile_sprite);
}

void Renderer::beginframe() {
  window_.clear(sf::Color::Black);
}
void Renderer::endframe() {
  window_.display();
}
sf::RenderWindow& Renderer::window() {
  return window_;
}
