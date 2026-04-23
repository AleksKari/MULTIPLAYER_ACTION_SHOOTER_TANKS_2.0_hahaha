#pragma once

#include <SFML/Graphics.hpp>
#include "../tile/tile.hpp"
#include "../../libraries/entity.hpp"
#include "../../libraries/projectile.hpp"
#include <concepts>

template<typename T>
concept is_tile = std::derived_from<T, Tile> || std::same_as<T, Tile>;

class Renderer {
  public:
    Renderer(int screen_width, int screen_height, int tile_size);
    void beginframe();
    void endframe();
    template<is_tile T>
    void draw_tile(const T& tile, int x, int y);
    void draw_entity(const Entity& entity);
    void draw_projectile(const Projectile& proj);
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

template <is_tile T>
void Renderer::draw_tile(const T& tile, int x, int y) {
  tile_sprite.setTextureRect({0, 0, 32, 32});
  tile_sprite.setPosition(x * 32, y * 32);
  window_.draw(tile_sprite);
}