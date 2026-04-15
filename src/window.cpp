#include "render/render.hpp"
#include "map/map.hpp"
#include "../libraries/player.hpp"
#include "weapon/gun.hpp"

int main() {

Renderer renderer(1920, 1080, 32);
sf::Clock clock;

// карта 60x34 тайла(квадрата) (1920/32 x 1080/32)
Map map(57, 31);


auto wep = std::make_unique<Gun>();

Player* player = new Player(Vec2(100, 100), 100, 150.0f, std::move(wep));
map.spawn_entity(player);

while (renderer.window().isOpen()) {
  // Сзакрытие окна и выстрел по событию
  sf::Event event;
  while (renderer.window().pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      renderer.window().close();
    if (event.type == sf::Event::MouseButtonPressed ||
      (event.type == sf::Event::KeyPressed &&
      event.key.code == sf::Keyboard::Space))
      player->attack(map);
  }
  //  движение, коллизии
  player->set_mouse(sf::Mouse::getPosition(renderer.window()));
  float dt = clock.restart().asSeconds();
  map.update(dt);
  //  отрисовка вроде такая логика +-
  renderer.beginframe();   // clear
  map.render(renderer);    // тайлы + players + снаряды
  renderer.endframe();     // display
}
return 0;
}