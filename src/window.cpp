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
        while(window.pollEvent(event)) {
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            ++move.x;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            --move.x;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            ++move.y;
        }
        move.normilized();
        player_pos = player_pos + move * speed * delta_time;
        player.setPosition(player_pos.x, player_pos.y);
        window.clear(sf::Color::Black);
        window.draw(player);
        window.display();
    }
    return 0;
}