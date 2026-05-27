#include <Game.hpp>

int main() {
    srand(time(NULL));
    Renderer renderer(1920, 1080, 32);
    Game game(renderer);
    sf::Clock clock;

    while (renderer.Window().isOpen()) {
        sf::Event event;
        while (renderer.Window().pollEvent(event)) {
            if (event.type == sf::Event::Closed) renderer.Window().close();
            if (event.type == sf::Event::Resized) game.SetRelayout();
            game.HandleEvents(event);
        }
        game.Update(clock.restart().asSeconds());
        game.Draw();
    }
    return 0;
}
