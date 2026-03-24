#include <SFML/Graphics.hpp>
#include "math/Vec2.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "My game");
    
    sf::RectangleShape player(sf::Vector2f(40, 40));//игрок это квадратик 40 на 40
    player.setFillColor(sf::Color::Cyan);
    Vec2 player_pos(400, 300);
    double speed = static_cast<double>(200);
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent()) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        //delta_time - время между кадрами
        double delta_time = clock.restart().asSeconds();
        Vec2 move(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            --move.y;
        }
    }

}