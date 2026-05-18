#pragma once

#include <SFML/Graphics.hpp>
#include <concepts>
#include <math/Vec2.h>
#include <tile/ConceptTile.hpp>
#include <Entity.hpp>
#include <Player.hpp>

class Tile;
class Projectile;
class Renderer {
 public:
  Renderer(int screen_width, int screen_height, int tile_size);
  void beginframe();
  void endframe();
  template<is_tile T>
  void draw_tile(const T& tile, Vec2 pos);
  template<is_entity T>
  void draw_entity(const T& entity);
  void draw_projectile(const Projectile& proj);
  void draw_hp_bar(const Player& p);
  sf::RenderWindow& window();

 private:
  sf::RenderWindow window_;
  int tile_size_;
  sf::Texture tile_texture_;
  sf::Texture entity_texture_;
  sf::Texture projectile_texture_;
  sf::Sprite tile_sprite_;
  sf::Sprite entity_sprite_;
  sf::Sprite projectile_sprite_;
};

template <is_tile T>
void Renderer::draw_tile(const T& tile, Vec2 pos) {
  static_assert(!std::same_as<T, Tile>);
  tile_sprite_.setTexture(tile.texture_);
  tile_sprite_.setTextureRect({0, 0, TILESIZE, TILESIZE});
  tile_sprite_.setPosition(pos.x * TILESIZE, pos.y * TILESIZE);
  window_.draw(tile_sprite_);
}

template<is_entity T>
void Renderer::draw_entity(const T& e) {
  if (e.is_dead()) return;

  if constexpr (requires { e.getSprite(); }) {
    sf::Sprite draw_spr = e.getSprite();
    draw_spr.setPosition(e.position.x + e.size / 2.0f, e.position.y + e.size / 2.0f);
    draw_spr.setRotation(e.cornrotate * 180.0 / M_PI);
    window_.draw(draw_spr);
  } else {
    entity_sprite_.setPosition(e.position.x + e.size / 2.0f, e.position.y + e.size / 2.0f);
    entity_sprite_.setRotation(e.cornrotate * 180.0 / M_PI);
    window_.draw(entity_sprite_);
  }
}
