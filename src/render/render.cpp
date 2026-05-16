//класс отрисовки

#include "render.hpp"
#include <entity.hpp>
#include <projectile.hpp>
#include <player.hpp>
#include <cmath>
#include <iostream>

Renderer::Renderer(int screen_width, int screen_height, int tile_size)
    : window_(sf::VideoMode(screen_width, screen_height), "Soul Knife Tanks"), tile_size_(tile_size) {
  if (!tile_texture_.loadFromFile("textures/stone.png")) {
    std::cout << "failed to load map\n";
  }
  if (!entity_texture_.loadFromFile("textures/player.png")) {
    std::cout << "failed to load entity\n";
  }
  if (!projectile_texture_.loadFromFile("textures/projectile.png")) {
    std::cout << "failed to load projectile\n";
  }
  tile_sprite_.setTexture(tile_texture_);
  entity_sprite_.setTexture(entity_texture_);
  entity_sprite_.setOrigin(16, 16);  // центр 32×32 текстуры
  projectile_sprite_.setTexture(projectile_texture_);
  projectile_sprite_.setOrigin(16, 16);
}

void Renderer::draw_entity(const Entity& e) {
  if (e.is_dead()) return;
  entity_sprite_.setPosition(e.position.x + e.size / 2.0, e.position.y + e.size / 2.0); // 16 16
  entity_sprite_.setRotation(e.cornrotate * 180.0 / M_PI);
  window_.draw(entity_sprite_);
}

void Renderer::draw_projectile(const Projectile& p) {
  if (p.is_dead()) return;
  projectile_sprite_.setPosition(p.position.x + p.size / 2.0, p.position.y + p.size / 2.0);
  window_.draw(projectile_sprite_);
}

void Renderer::draw_hp_bar(const Player& p) {
  if(p.is_dead()) return;
  float perct = (float)p.hp / p.max_hp;
//  std::cout << "hp=" << p.hp << " max=" << p.max_hp << " pct=" << (float)p.hp/p.max_hp << "\n";
  sf::RectangleShape backgr_for_hp({32.f, 4.f});
  backgr_for_hp.setFillColor(sf::Color(80, 80, 80));
  backgr_for_hp.setPosition(p.position.x, p.position.y - 7.f);//выше танка 5 px
  window_.draw(backgr_for_hp);
  sf::RectangleShape green_hp({32.f * perct, 4.f});
  green_hp.setFillColor(sf::Color(50, 200, 50));
  green_hp.setPosition(p.position.x, p.position.y - 7.f);
  window_.draw(green_hp);
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
