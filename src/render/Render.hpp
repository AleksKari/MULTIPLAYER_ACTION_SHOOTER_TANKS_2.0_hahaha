#pragma once

#include <math/Vec2.h>

#include <Entity.hpp>
#include <Player.hpp>
#include <SFML/Graphics.hpp>
#include <concepts>
#include <tile/ConceptTile.hpp>

class Tile;
class Projectile;
class Renderer {
 public:
  Renderer(int screen_width, int screen_height, int tile_size);
  void BeginFrame();
  void EndFrame();
  template <is_tile T>
  void DrawTile(const T& tile, Vec2 pos);
  template <is_entity T>
  void DrawEntity(const T& entity);
  void DrawProjectile(const Projectile& proj);
  void DrawHpBar(const Player& player);
  sf::RenderWindow& Window();

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
void Renderer::DrawTile(const T& tile, Vec2 pos) {
  static_assert(!std::same_as<T, Tile>);
  tile_sprite_.setTexture(tile.texture_);
  tile_sprite_.setTextureRect({0, 0, TILESIZE, TILESIZE});
  tile_sprite_.setPosition(pos.cord_x * TILESIZE, pos.cord_y * TILESIZE);
  window_.draw(tile_sprite_);
}

template <is_entity T>
void Renderer::DrawEntity(const T& entity) {
  if (entity.IsDead()) {
    return;
  }

  if constexpr (requires { entity.GetSprite(); }) {
    sf::Sprite draw_spr = entity.GetSprite();
    draw_spr.setPosition(
        entity.position.cord_x + (entity.size / NUMBER_TO_DIV),
        entity.position.cord_y + (entity.size / NUMBER_TO_DIV));
    draw_spr.setRotation(entity.cornrotate * FULL_ANGLE / M_PI);
    window_.draw(draw_spr);
  } else {
    entity_sprite_.setPosition(
        entity.position.cord_x + (entity.size / NUMBER_TO_DIV),
        entity.position.cord_y + (entity.size / NUMBER_TO_DIV));
    entity_sprite_.setRotation(entity.cornrotate * FULL_ANGLE / M_PI);
    window_.draw(entity_sprite_);
  }
}
