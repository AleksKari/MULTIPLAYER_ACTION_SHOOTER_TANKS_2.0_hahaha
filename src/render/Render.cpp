// класс отрисовки

#include <Player.hpp>
#include <Projectile.hpp>
#include <cmath>
#include <iostream>
#include <render/Render.hpp>

const sf::Color GREEN = sf::Color(50, 200, 50);
const sf::Color GREY = sf::Color(80, 80, 80);
const float WIDTH_HP_BAR = 4.F;
const float WIDTH_BETWEEN_PLAYER_AND_HP = 7.F;

Renderer::Renderer(int screen_width, int screen_height, int tile_size)
    : window_(sf::VideoMode(screen_width, screen_height), "Soul Knife Tanks"),
      tile_size_(tile_size) {
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
  entity_sprite_.setOrigin(TILESIZE / 2, TILESIZE / 2);
  projectile_sprite_.setTexture(projectile_texture_);
  projectile_sprite_.setOrigin(TILESIZE / 2, TILESIZE / 2);
}

void Renderer::DrawProjectile(const Projectile& projectile) {
  if (projectile.IsDead()) {
    return;
  }
  projectile_sprite_.setPosition(
      projectile.position.cord_x + (projectile.size / NUMBER_TO_DIV),
      projectile.position.cord_y + (projectile.size / NUMBER_TO_DIV));
  window_.draw(projectile_sprite_);
}

void Renderer::DrawHpBar(const Player& player) {
  if (player.IsDead()) {
    return;
  }
  float perct = static_cast<float>(player.heatpoint) / player.max_hp;

  sf::RectangleShape backgr_for_hp(
      {static_cast<float>(TILESIZE), WIDTH_HP_BAR});
  backgr_for_hp.setFillColor(GREY);
  backgr_for_hp.setPosition(
      player.position.cord_x,
      player.position.cord_y - WIDTH_BETWEEN_PLAYER_AND_HP);
  window_.draw(backgr_for_hp);
  sf::RectangleShape green_hp(
      {static_cast<float>(TILESIZE) * perct, WIDTH_HP_BAR});
  green_hp.setFillColor(GREEN);
  green_hp.setPosition(player.position.cord_x,
                       player.position.cord_y - WIDTH_BETWEEN_PLAYER_AND_HP);
  window_.draw(green_hp);
}

void Renderer::BeginFrame() { window_.clear(sf::Color::Black); }
void Renderer::EndFrame() { window_.display(); }
sf::RenderWindow& Renderer::Window() { return window_; }
