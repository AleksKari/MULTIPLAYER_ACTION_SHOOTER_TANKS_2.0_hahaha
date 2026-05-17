#include <render/Render.hpp>
#include <map/Map.hpp>
#include <Player.hpp>
#include <weapon/Gun.hpp>
#include <iostream>
#include <tile/EmptyTile.hpp>
#include <Projectile.hpp>
#include <collision/Collision.hpp>
#include <network/NetworkManager.hpp>
#include <algorithm>

enum class GameState { Menu, NetworkMode, JoinInput, HostingInfo, Gaming };

int main() {
  // Адаптивное окно под размер экрана
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  unsigned int window_width = std::min(desktop.width, 1920u);
  unsigned int window_height = std::min(desktop.height, 1080u);
  
  Renderer renderer(window_width, window_height, 32);
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
  preview1.setPosition(window_width * 0.3f, window_height * 0.4f);
  preview2.setScale(5.0f, 5.0f);
  preview2.setPosition(window_width * 0.6f, window_height * 0.4f);

  sf::Font font;
  if (!font.loadFromFile("textures/font.ttf")) {
      std::cerr << "Failed to load font.ttf!" << std::endl;
  }
  sf::Text uiText("", font, window_height / 25);
  uiText.setFillColor(sf::Color::White);
  uiText.setPosition(window_width * 0.2f, window_height * 0.7f);

  int selected = 1;
  GameState currentState = GameState::Menu;
  std::string inputCode = "";
  std::string lobbyInfoStr = "";
 
  Map map(59, 33);
  
  // Изначально создаем ТОЛЬКО себя (наш локальный танк)
  Player* local_player = new Player(Vec2(200, 200), 100, 300.0f, std::make_unique<Gun>());
  map.spawn_entity(local_player);

  // Указатель на танк соперника (появится только по сети)
  Player* remote_player = nullptr;

  NetworkManager net;

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
            std::string playerSkin = (selected == 1) ? "textures/player.png" : "textures/pink_player.png";
            local_player->loadSkin(playerSkin);
            
            currentState = GameState::NetworkMode;
            uiText.setString("Press [H] to Host Lobby\nPress [J] to Join Lobby");
          }
        }
      }
      else if (currentState == GameState::NetworkMode) {
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::H) {
              auto code = net.host_lobby(35678); 
              local_player->network_id = 1; // Хост всегда имеет ID = 1
              
              std::string server_ip = "185.79.139.24";
              std::string display_code = server_ip;
              std::replace(display_code.begin(), display_code.end(), '.', 'A');
              
              lobbyInfoStr = "LOBBY CREATED SUCCESSFULLY!\n\n"
                             "Server IP:  " + server_ip + "\n"
                             "Lobby Code: " + display_code + "\n\n"
                             "Waiting for friend to connect...\n"
                             "Press [Enter] to Start Match when they are ready!";
              uiText.setString(lobbyInfoStr);
              currentState = GameState::HostingInfo;
          }
          if (event.key.code == sf::Keyboard::J) {
              currentState = GameState::JoinInput;
              uiText.setString("Enter Lobby Code: ");
          }
        }
      }
      else if (currentState == GameState::HostingInfo) {
          // Хост ждет подключения и нажимает Enter, чтобы начать игру
          if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
              currentState = GameState::Gaming;
              clock.restart();
          }
      }
      else if (currentState == GameState::JoinInput) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b' && !inputCode.empty()) {
                inputCode.pop_back();
            } else if (event.text.unicode > 31 && event.text.unicode < 127) {
                inputCode += static_cast<char>(event.text.unicode);
            }
            uiText.setString("Enter Lobby Code: " + inputCode);
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            if (net.try_join_lobby(inputCode, 35678)) {
                local_player->network_id = 2; // Клиент всегда имеет ID = 2
                
                // Спавним танк Хоста (ID 1) на экране клиента
                remote_player = new Player(Vec2(500, 500), 100, 300.0f, std::make_unique<Gun>());
                remote_player->network_id = 1;
                remote_player->loadSkin((selected == 1) ? "textures/pink_player.png" : "textures/player.png");
                map.spawn_entity(remote_player);

                currentState = GameState::Gaming;
                clock.restart();
            } else {
                inputCode = "";
                uiText.setString("Connection Failed! Try again: ");
            }
        }
      }
      else if (currentState == GameState::Gaming) {
        if (event.type == sf::Event::MouseButtonPressed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)) {
          if (!local_player->is_dead()) {
              local_player->attack(map);
          }
        }
      }
    }

    if (currentState == GameState::Gaming) {
      // НАСТОЯЩЕЕ РАЗДЕЛЕНИЕ: Управляем ТОЛЬКО своим локальным танком
      local_player->set_input(
          sf::Keyboard::isKeyPressed(sf::Keyboard::W),
          sf::Keyboard::isKeyPressed(sf::Keyboard::A),
          sf::Keyboard::isKeyPressed(sf::Keyboard::S),
          sf::Keyboard::isKeyPressed(sf::Keyboard::D)
      );
      local_player->set_mouse(sf::Mouse::getPosition(renderer.window()));
      
      float dt = clock.restart().asSeconds();
      
      // Получаем пакеты от другого игрока
      net.update(map);
      
      // Если мы ХОСТ (ID 1) и к нам подключился клиент (ID 2), но его еще нет на карте — спавним его
      if (local_player->network_id == 1 && remote_player == nullptr) {
          // Ищем, появился ли в сетевом менеджере клиент с ID 2
          // NetworkManager при получении пакетов от ID 2 автоматически добавит его или обновит.
          // Чтобы отобразить его сразу, создаем его локальное представление:
          remote_player = new Player(Vec2(500, 500), 100, 300.0f, std::make_unique<Gun>());
          remote_player->network_id = 2;
          remote_player->loadSkin("textures/pink_player.png");
          map.spawn_entity(remote_player);
      }
      
      // Отправляем наше состояние (координаты, поворот) на сервер
      sf::Packet statePacket;
      statePacket << PacketType::UpdateState;
      local_player->serialize(statePacket);
      net.send_to_all(statePacket);

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
    } else if (currentState == GameState::NetworkMode || currentState == GameState::JoinInput || currentState == GameState::HostingInfo) {
      renderer.window().draw(menuBg);
      renderer.window().draw(uiText);
    } else {
      map.render(renderer);
    }
    renderer.endframe();
  }
  return 0;
}
