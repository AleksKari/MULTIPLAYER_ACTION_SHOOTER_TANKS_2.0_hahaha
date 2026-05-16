#pragma once

#include <SFML/Graphics.hpp>
#include <concepts>
#include <math/Vec2.h>
#include <tile/concept_tile.hpp>


class Tile;
class Entity;
class Projectile;
class Player;
class Renderer {
  public:
    Renderer(int screen_width, int screen_height, int tile_size);
    void beginframe();
    void endframe();
    template<is_tile T>
    void draw_tile(const T& tile, Vec2 pos);
    void draw_entity(const Entity& entity);
    void draw_projectile(const Projectile& proj);
    void draw_hp_bar(const Player& p);
    sf::RenderWindow& window();
  private:
    sf::RenderWindow window_;
    int tile_size_;
    sf::Texture tile_texture;
    sf::Texture entity_texture;
    sf::Texture projectile_texture;
    sf::Sprite tile_sprite;
    sf::Sprite entity_sprite;
    sf::Sprite projectile_sprite;
};

template <is_tile T>  // в зависимости от того какой тайл пришел тот и отрисовывать
void Renderer::draw_tile(const T& tile, Vec2 pos) {
  static_assert(!std::same_as<T, Tile>);
  tile_sprite.setTexture(tile.texture_);
  tile_sprite.setTextureRect({0, 0, 32, 32});
  tile_sprite.setPosition(pos.x * 32, pos.y * 32);
  window_.draw(tile_sprite);
}