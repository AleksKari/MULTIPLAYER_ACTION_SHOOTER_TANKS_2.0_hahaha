#include <render/Render.hpp>
#include <map/Map.hpp>
#include <Player.hpp>
#include <weapon/Gun.hpp>
#include <iostream>
#include <tile/EmptyTile.hpp>
#include <Projectile.hpp>
#include <collision/Collision.hpp>
#include <network/NetworkManager.hpp>
#include <network/Protocol.hpp>
#include <algorithm>
#include <cstdlib>
#include <ctime>

enum class GameState { Menu, NetworkMode, JoinInput, WaitingForOpponent, Gaming };

std::string generateLobbyCode() {
    std::string code = "";
    for (int i = 0; i < 4; ++i) code += (char)('A' + rand() % 26);
    return code;
}

int main() {
  srand(time(NULL));

  const std::string SERVER_IP = "185.79.139.24";
  const unsigned short SERVER_PORT = 35678;

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
  sf::Text uiText("", font, window_height / 20);
  uiText.setFillColor(sf::Color::White);
  uiText.setPosition(window_width * 0.25f, window_height * 0.7f);

  int selected = 1;
  GameState currentState = GameState::Menu;
  std::string inputCode = "";
  std::string myRoomCode = "";
 
  Map map(59, 33);
  Player* local_player = nullptr;
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
            currentState = GameState::NetworkMode;
            uiText.setString("Press [H] to Create Lobby\nPress [J] to Join Lobby");
          }
        }
      }
      else if (currentState == GameState::NetworkMode) {
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::H) {
              if (net.try_join_lobby(SERVER_IP, SERVER_PORT)) {
                  myRoomCode = generateLobbyCode();
                  
                  sf::Packet roomPacket; roomPacket << myRoomCode;
                  net.send_to_all(roomPacket);
                  
                  local_player = new Player(Vec2(200, 540), 100, 300.0f, std::make_unique<Gun>());
                  local_player->network_id = 1;
                  local_player->loadSkin((selected == 1) ? "textures/player.png" : "textures/pink_player.png");
                  map.spawn_entity(local_player);
                  
                  remote_player = new Player(Vec2(1700, 540), 100, 300.0f, std::make_unique<Gun>());
                  remote_player->network_id = 2;
                  remote_player->loadSkin((selected == 1) ? "textures/pink_player.png" : "textures/player.png");
                  map.spawn_entity(remote_player);
                  
                  currentState = GameState::WaitingForOpponent;
              } else {
                  uiText.setString("Server Offline! Could not connect.");
              }
          }
          if (event.key.code == sf::Keyboard::J) {
              currentState = GameState::JoinInput;
              uiText.setString("Enter 4-letter Room Code: ");
          }
        }
      }
      else if (currentState == GameState::JoinInput) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b' && !inputCode.empty()) {
                inputCode.pop_back();
            } else if (std::isalpha(event.text.unicode) && inputCode.length() < 4) {
                inputCode += std::toupper(static_cast<char>(event.text.unicode));
            }
            uiText.setString("Enter 4-letter Room Code: " + inputCode);
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && inputCode.length() == 4) {
            if (net.try_join_lobby(SERVER_IP, SERVER_PORT)) {
                // Шлем запрос на подключение
                sf::Packet roomPacket; roomPacket << sf::Int32(-2) << inputCode;
                net.send_to_all(roomPacket);
                
                // Вызываем наш новый безопасный метод
                int responseResult = net.receive_response();
                
                if (responseResult == 1) { // Успешно! Комната существует
                    local_player = new Player(Vec2(1700, 540), 100, 300.0f, std::make_unique<Gun>());
                    local_player->network_id = 2;
                    local_player->loadSkin((selected == 1) ? "textures/player.png" : "textures/pink_player.png");
                    map.spawn_entity(local_player);
                    
                    remote_player = new Player(Vec2(200, 540), 100, 300.0f, std::make_unique<Gun>());
                    remote_player->network_id = 1;
                    remote_player->loadSkin((selected == 1) ? "textures/pink_player.png" : "textures/player.png");
                    map.spawn_entity(remote_player);
                    
                    currentState = GameState::Gaming;
                    clock.restart();
                } else { // Сервер вернул 0 (комната не найдена)
                    inputCode = "";
                    uiText.setString("Lobby Not Found! Try again: ");
                }
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
              sf::Packet shootPacket;
              shootPacket << PacketType::Shoot;
              net.send_to_all(shootPacket);
          }
        }
      }
    }

    if (currentState == GameState::WaitingForOpponent) {
        net.update(map);
        uiText.setString("LOBBY CODE: " + myRoomCode + "\n\nWaiting for second player...");
        
        sf::Packet statePacket;
        statePacket << PacketType::UpdateState;
        local_player->serialize(statePacket);
        net.send_to_all(statePacket);

        if (remote_player->position.x != 1700.0f) {
            currentState = GameState::Gaming;
            clock.restart();
        }
    }
    else if (currentState == GameState::Gaming) {
      local_player->set_input(
          sf::Keyboard::isKeyPressed(sf::Keyboard::W),
          sf::Keyboard::isKeyPressed(sf::Keyboard::A),
          sf::Keyboard::isKeyPressed(sf::Keyboard::S),
          sf::Keyboard::isKeyPressed(sf::Keyboard::D)
      );
      local_player->set_mouse(sf::Mouse::getPosition(renderer.window()));
      
      float dt = clock.restart().asSeconds();
      net.update(map);
      
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
    } else if (currentState == GameState::NetworkMode || currentState == GameState::JoinInput || currentState == GameState::WaitingForOpponent) {
      renderer.window().draw(menuBg);
      renderer.window().draw(uiText);
    } else if (currentState == GameState::Gaming) {
      map.render(renderer);
    }
    renderer.endframe();
  }
  return 0;
}
