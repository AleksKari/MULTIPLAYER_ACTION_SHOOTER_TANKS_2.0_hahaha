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

void Renderer::beginframe() {
    window_.clear(sf::Color::Black);
}
void Renderer::endframe() {
    window_.display();
}
sf::RenderWindow& Renderer::window() {
    return window_;
}
