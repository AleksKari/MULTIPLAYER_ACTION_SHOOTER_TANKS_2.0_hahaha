#pragma once

#include <SFML/Graphics.hpp>
#include "../tile/tile.hpp"
#include "../../libraries/entity.hpp"
#include "../../libraries/projectile.hpp"

class Renderer {
  public:
    Renderer(int screen_width, int screen_height, int tile_size);
    void beginframe();
    void endframe();
    void draw_tile(const Tile& tile, int x, int y);
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
