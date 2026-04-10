//класс отрисовки

#include "render.hpp"
#include <iostream>

Renderer::Renderer(int screen_width, int screen_height, int tile_size)
    : window_(sf::VideoMode(screen_width, screen_height), "Soul Knife Tanks"), tile_size_(tile_size) {
    if (!tile_texture.loadFromFile("tiles.png")) {
        std::cout << "failed to load\n"; 
    }
    if (!entity_texture.loadFromFile("entity.png")) {
       std::cout << "failed to load\n";
    }  
    if (!projectile_texture.loadFromFile("projectile.png")) {
        std::cout << "failed to load\n";
    }  
    tile_sprite.setTexture(tile_texture);
    entity_sprite.setTexture(entity_texture);
    projectile_sprite.setTexture(projectile_texture);
}

void Renderer::draw_tile(const Tile& tile, int x, int y) {
    tile_sprite.setTextureRect(tile.texture_rect);
    tile_sprite.setPosition(x * tile_size_, y * tile_size_);
    window_.draw(tile_sprite);
}

void Renderer::draw_entity(const Entity& e) {
    entity_sprite.setTextureRect(e.texture_rect_);
    entity_sprite.setPosition(e.position.x, e.position.y);
    window_.draw(entity_sprite);
}

void Renderer::draw_projectile(const Projectile& p) {
    projectile_sprite.setTextureRect({p.position.x, p.position.y, 8, 8});
    projectile_sprite.setPosition(p.position.x, p.position.y);
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
