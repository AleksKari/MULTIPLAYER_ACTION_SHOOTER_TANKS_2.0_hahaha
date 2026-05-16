#include <render/Render.hpp>
#include <map/Map.hpp>
#include <Player.hpp>
#include <weapon/Gun.hpp>
//#include <weapon/Shotgun.hpp>
//#include <weapon/RicochetGun.hpp>
#include <iostream>
#include <tile/EmptyTile.hpp>
#include <Projectile.hpp>
#include <collision/Collision.hpp>

enum class GameState { Menu, Gaming };

int main() {

  Renderer renderer(1920, 1080, 32);
  sf::Clock clock;
  sf::Texture menuBgTex, skin1Tex, skin2Tex;
  menuBgTex.loadFromFile("textures/menu_bg.png"); 
  skin1Tex.loadFromFile("textures/player.png");
  skin2Tex.loadFromFile("textures/pink_player.png");

  sf::Sprite menuBg(menuBgTex);
  menuBg.setScale(
    static_cast<float>(renderer.window().getSize().x) / menuBgTex.getSize().x,
    static_cast<float>(renderer.window().getSize().y) / menuBgTex.getSize().y
  );
  sf::Sprite preview1(skin1Tex);
  sf::Sprite preview2(skin2Tex);

  preview1.setScale(5.0f, 5.0f);
  preview2.setScale(5.0f, 5.0f);
  preview1.setPosition(600, 400);
  preview2.setPosition(1100, 400);
  int selected = 1;
  GameState currentState = GameState::Menu;
 
  // карта 60x34 тайла(квадрата) (1920/32 x 1080/32)
  Map map(57, 31);
  
  // каждому игроку создаем свое оружие
  auto wep1 = std::make_unique<Gun>();
  auto wep2 = std::make_unique<Gun>();

  Player* player = new Player(Vec2(100, 100), 100, 150.0f, std::move(wep1));
  Player* weak = new Player(Vec2(500, 500), 100, 150.0f, std::move(wep2));

  map.spawn_entity(player);
  map.spawn_entity(weak);

  while (renderer.window().isOpen()) {
    sf::Event event;
    while (renderer.window().pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        renderer.window().close();

      if (currentState == GameState::Menu) {
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Left)  selected = 1;
          if (event.key.code == sf::Keyboard::Right) selected = 2;
          if (event.key.code == sf::Keyboard::Enter) {
            std::string skinPath = (selected == 1) ? "textures/player.png" : "textures/pink_player.png";
            player->loadSkin(skinPath);
            currentState = GameState::Gaming;
            clock.restart();
          }
        }
      }
      else if (currentState == GameState::Gaming) {
        if (event.type == sf::Event::MouseButtonPressed ||
           (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)) {
          if (!player->is_dead()) player->attack(map);
        }
      }
    }

    if (currentState == GameState::Gaming) {
      player->set_mouse(sf::Mouse::getPosition(renderer.window()));
      float dt = clock.restart().asSeconds();
      map.update(dt);
      Collision::resolve(map);
    } else {
      clock.restart();
    }

    renderer.beginframe();

    if (currentState == GameState::Menu) {
      preview1.setColor(selected == 1 ? sf::Color::White : sf::Color(100, 100, 100));
      preview2.setColor(selected == 2 ? sf::Color::White : sf::Color(100, 100, 100));
      renderer.window().draw(menuBg);
      renderer.window().draw(preview1);
      renderer.window().draw(preview2);
    } else {
      map.render(renderer);
    }

    renderer.endframe();
  }
  return 0;
}
