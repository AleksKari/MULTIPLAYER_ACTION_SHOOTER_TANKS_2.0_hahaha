#include <Player.hpp>
#include <Projectile.hpp>
#include <algorithm>
#include <cctype>
#include <collision/Collision.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map/Map.hpp>
#include <network/NetworkManager.hpp>
#include <network/Protocol.hpp>
#include <render/Render.hpp>
#include <tile/EmptyTile.hpp>
#include <tuple>
#include <utility>
#include <vector>
#include <weapon/Gun.hpp>

enum class GameState {
  Menu,
  NetworkMode,
  JoinInput,
  WaitingForOpponent,
  Gaming,
  GameOver
};

std::string generateLobbyCode() {
  std::string code = "";
  for (int i = 0; i < 4; ++i) code += static_cast<char>('A' + rand() % 26);
  return code;
}

sf::FloatRect makeLetterboxViewport(float window_width, float window_height,
                                    float view_width, float view_height) {
  const float window_ratio = window_width / window_height;
  const float view_ratio = view_width / view_height;

  if (window_ratio > view_ratio) {
    const float width = view_ratio / window_ratio;
    return sf::FloatRect((1.0f - width) * 0.5f, 0.0f, width, 1.0f);
  }

  const float height = window_ratio / view_ratio;
  return sf::FloatRect(0.0f, (1.0f - height) * 0.5f, 1.0f, height);
}

int main() {
  srand(time(NULL));

  const std::string SERVER_IP = "185.79.139.24";
  const unsigned short SERVER_PORT = 35678;
  const int map_width_tiles = 59;
  const int map_height_tiles = 33;
  const float map_width_px = static_cast<float>(map_width_tiles * TILESIZE);
  const float map_height_px = static_cast<float>(map_height_tiles * TILESIZE);

  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  unsigned int window_width =
      std::min(desktop.width, static_cast<unsigned int>(map_width_px));
  unsigned int window_height =
      std::min(desktop.height, static_cast<unsigned int>(map_height_px));

  Renderer renderer(window_width, window_height, 32);
  renderer.window().setFramerateLimit(120);
  sf::Clock clock;

  sf::Texture menuBgTex, skin1Tex, skin2Tex;
  menuBgTex.loadFromFile("textures/menu_bg.png");
  skin1Tex.loadFromFile("textures/player.png");
  skin2Tex.loadFromFile("textures/pink_player.png");

  sf::Sprite menuBg(menuBgTex);

  sf::Sprite preview1(skin1Tex);
  sf::Sprite preview2(skin2Tex);
  preview1.setScale(4.5f, 4.5f);
  preview2.setScale(4.5f, 4.5f);

  sf::Font font;
  if (!font.loadFromFile("textures/font.ttf")) {
    std::cerr << "Failed to load font.ttf!" << std::endl;
  }
  sf::Text uiText("", font, window_height / 20);
  uiText.setFillColor(sf::Color::White);
  uiText.setPosition(window_width * 0.14f, window_height * 0.62f);

  sf::View game_view(sf::FloatRect(0.0f, 0.0f, map_width_px, map_height_px));
  game_view.setViewport(makeLetterboxViewport(static_cast<float>(window_width),
                                              static_cast<float>(window_height),
                                              map_width_px, map_height_px));
  sf::View ui_view(sf::FloatRect(0.0f, 0.0f, static_cast<float>(window_width),
                                 static_cast<float>(window_height)));

  auto relayout_ui = [&]() {
    const sf::Vector2u size = renderer.window().getSize();
    ui_view = sf::View(sf::FloatRect(0.0f, 0.0f, static_cast<float>(size.x),
                                     static_cast<float>(size.y)));
    game_view.setViewport(makeLetterboxViewport(static_cast<float>(size.x),
                                                static_cast<float>(size.y),
                                                map_width_px, map_height_px));
    menuBg.setScale(static_cast<float>(size.x) / menuBgTex.getSize().x,
                    static_cast<float>(size.y) / menuBgTex.getSize().y);
    preview1.setPosition(size.x * 0.30f, size.y * 0.34f);
    preview2.setPosition(size.x * 0.58f, size.y * 0.34f);
    uiText.setCharacterSize(std::max(28u, size.y / 28u));
    uiText.setPosition(size.x * 0.10f, size.y * 0.62f);
  };
  relayout_ui();

  int selected = 1;
  bool host_authority = false;
  GameState currentState = GameState::Menu;
  std::string inputCode = "";
  std::string myRoomCode = "";
  std::string game_over_text = "";

  Map map(map_width_tiles, map_height_tiles);
  Player* local_player = nullptr;
  Player* remote_player = nullptr;

  NetworkManager net;

  auto tile_to_world = [](int tx, int ty) {
    return Vec2(tx * TILESIZE, ty * TILESIZE);
  };

  auto valid_spawn = [&](int tx, int ty) {
    const Vec2 pos = tile_to_world(tx, ty);
    const double x0 = pos.x + 2.0;
    const double y0 = pos.y + 2.0;
    const double x1 = pos.x + TILESIZE - 2.0;
    const double y1 = pos.y + TILESIZE - 2.0;
    return map.is_empty(x0, y0) && map.is_empty(x1, y1) &&
           !map.is_slow(x0, y0) && !map.is_damage(x0, y0);
  };

  auto pick_spawn = [&](const std::vector<std::pair<int, int>>& candidates,
                        const Vec2& fallback) {
    for (const auto& [x, y] : candidates) {
      if (valid_spawn(x, y)) return tile_to_world(x, y);
    }
    return fallback;
  };

  auto create_players = [&](bool host_role) {
    const std::vector<std::pair<int, int>> host_spawns = {
        {2, 2}, {2, 30}, {8, 16}, {12, 16}};
    const std::vector<std::pair<int, int>> guest_spawns = {
        {56, 30}, {56, 2}, {50, 16}, {46, 16}};
    const Vec2 host_spawn = pick_spawn(host_spawns, Vec2(200, 540));
    const Vec2 guest_spawn = pick_spawn(guest_spawns, Vec2(1700, 540));

    host_authority = host_role;
    if (host_role) {
      local_player =
          new Player(host_spawn, 100, 300.0f, std::make_unique<Gun>());
      local_player->network_id = 1;
      local_player->is_local = true;
      local_player->loadSkin((selected == 1) ? "textures/player.png"
                                             : "textures/pink_player.png");
      map.spawn_entity(local_player);

      remote_player =
          new Player(guest_spawn, 100, 300.0f, std::make_unique<Gun>());
      remote_player->network_id = 2;
      remote_player->is_local = false;
      remote_player->loadSkin((selected == 1) ? "textures/pink_player.png"
                                              : "textures/player.png");
      map.spawn_entity(remote_player);
    } else {
      local_player =
          new Player(guest_spawn, 100, 300.0f, std::make_unique<Gun>());
      local_player->network_id = 2;
      local_player->is_local = true;
      local_player->loadSkin((selected == 1) ? "textures/player.png"
                                             : "textures/pink_player.png");
      map.spawn_entity(local_player);

      remote_player =
          new Player(host_spawn, 100, 300.0f, std::make_unique<Gun>());
      remote_player->network_id = 1;
      remote_player->is_local = false;
      remote_player->loadSkin((selected == 1) ? "textures/pink_player.png"
                                              : "textures/player.png");
      map.spawn_entity(remote_player);
    }
  };

  while (renderer.window().isOpen()) {
    sf::Event event;
    while (renderer.window().pollEvent(event)) {
      if (event.type == sf::Event::Closed) renderer.window().close();
      if (event.type == sf::Event::Resized) {
        relayout_ui();
      }

      if (currentState == GameState::Menu) {
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Left ||
              event.key.code == sf::Keyboard::A)
            selected = 1;
          if (event.key.code == sf::Keyboard::Right ||
              event.key.code == sf::Keyboard::D)
            selected = 2;
          if (event.key.code == sf::Keyboard::Enter ||
              event.key.code == sf::Keyboard::Space) {
            currentState = GameState::NetworkMode;
            uiText.setString("Choose mode:\n[1] Create lobby\n[2] Join lobby");
          }
        }
      } else if (currentState == GameState::NetworkMode) {
        if (event.type == sf::Event::KeyPressed) {
          const bool create_pressed = event.key.code == sf::Keyboard::H ||
                                      event.key.code == sf::Keyboard::Num1 ||
                                      event.key.code == sf::Keyboard::Numpad1;
          const bool join_pressed = event.key.code == sf::Keyboard::J ||
                                    event.key.code == sf::Keyboard::Num2 ||
                                    event.key.code == sf::Keyboard::Numpad2;

          if (create_pressed) {
            if (net.try_join_lobby(SERVER_IP, SERVER_PORT)) {
              myRoomCode = generateLobbyCode();
              int assigned_id = net.join_lobby(myRoomCode, true);
              if (assigned_id == 1) {
                create_players(true);
                net.reset_opponent_joined();
                currentState = GameState::WaitingForOpponent;
              } else {
                uiText.setString("Failed to create lobby. Try again.");
              }
            } else {
              uiText.setString("Server Offline! Could not connect.");
            }
          }
          if (join_pressed) {
            currentState = GameState::JoinInput;
            uiText.setString("Enter 4-letter Room Code: ");
          }
          if (event.key.code == sf::Keyboard::Escape) {
            currentState = GameState::Menu;
          }
        }
      } else if (currentState == GameState::JoinInput) {
        if (event.type == sf::Event::TextEntered) {
          if (event.text.unicode == '\b' && !inputCode.empty()) {
            inputCode.pop_back();
          } else if (event.text.unicode < 128 &&
                     std::isalpha(
                         static_cast<unsigned char>(event.text.unicode)) &&
                     inputCode.length() < 4) {
            inputCode += static_cast<char>(
                std::toupper(static_cast<unsigned char>(event.text.unicode)));
          }
          uiText.setString("Enter 4-letter Room Code: " + inputCode);
        }
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Enter && inputCode.length() == 4) {
          if (net.try_join_lobby(SERVER_IP, SERVER_PORT)) {
            int assigned_id = net.join_lobby(inputCode, false);
            if (assigned_id == 2) {
              create_players(false);
              currentState = GameState::Gaming;
              clock.restart();
            } else {
              inputCode.clear();
              uiText.setString("Lobby Not Found/Busy. Try again: ");
            }
          } else {
            inputCode.clear();
            uiText.setString("Connection Failed! Try again: ");
          }
        }
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
          inputCode.clear();
          currentState = GameState::NetworkMode;
          uiText.setString("Choose mode:\n[1] Create lobby\n[2] Join lobby");
        }
      } else if (currentState == GameState::Gaming) {
        if (event.type == sf::Event::MouseButtonPressed ||
            (event.type == sf::Event::KeyPressed &&
             event.key.code == sf::Keyboard::Space)) {
          if (!local_player->is_dead()) {
            if (host_authority) {
              local_player->attack(map);
            } else {
              sf::Packet shootPacket;
              shootPacket << PacketType::Shoot;
              net.send_to_all(shootPacket);
            }
          }
        }
      } else if (currentState == GameState::GameOver) {
        if (event.type == sf::Event::KeyPressed &&
            (event.key.code == sf::Keyboard::Enter ||
             event.key.code == sf::Keyboard::Escape)) {
          renderer.window().close();
        }
      }
    }

    if (currentState == GameState::WaitingForOpponent) {
      net.update(map, local_player, remote_player, true);
      uiText.setString("LOBBY CODE: " + myRoomCode +
                       "\n\nWaiting for second player...");
      if (net.connection_lost()) {
        game_over_text = "Connection lost.\nPress Enter/Esc to exit.";
        currentState = GameState::GameOver;
      }
      if (net.opponent_joined()) {
        currentState = GameState::Gaming;
        clock.restart();
      }
    } else if (currentState == GameState::Gaming) {
      const bool w = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
      const bool a = sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
      const bool s = sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
      const bool d = sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

      local_player->set_input(w, a, s, d);

      float dt = clock.restart().asSeconds();
      if (!host_authority) {
        sf::Packet inputPacket;
        inputPacket << PacketType::PlayerInput << w << a << s << d;
        net.send_to_all(inputPacket);
        // Client-side prediction: keeps local controls responsive between
        // snapshots.
        local_player->update(dt);
      }

      net.update(map, local_player, remote_player, host_authority);

      if (host_authority) {
        map.update(dt);
        Collision::resolve(map);

        auto generated_weapon = map.generate_weapon();
        if (generated_weapon.has_value()) {
          int x = 0, y = 0, type = 0;
          std::tie(x, y, type) = generated_weapon.value();
          sf::Packet spawnPacket;
          spawnPacket << PacketType::SpawnWeapon << x << y << type;
          net.send_to_all(spawnPacket);
        }

        std::vector<Vec2> removed_tiles = map.consume_removed_weapon_tiles();
        for (const Vec2& tile : removed_tiles) {
          sf::Packet removePacket;
          removePacket << PacketType::RemoveTile << static_cast<int>(tile.x)
                       << static_cast<int>(tile.y);
          net.send_to_all(removePacket);
        }

        sf::Packet statePacket;
        statePacket << PacketType::GameState;
        map.serialize_game_state(statePacket);
        net.send_to_all(statePacket);
      }

      if (net.connection_lost()) {
        game_over_text = "Opponent disconnected.\nPress Enter/Esc to exit.";
        currentState = GameState::GameOver;
      } else if (local_player->is_dead() || remote_player->is_dead()) {
        if (local_player->is_dead() && remote_player->is_dead()) {
          game_over_text = "Draw.\nPress Enter/Esc to exit.";
        } else if (local_player->is_dead()) {
          game_over_text = "You lose.\nPress Enter/Esc to exit.";
        } else {
          game_over_text = "You win!\nPress Enter/Esc to exit.";
        }
        currentState = GameState::GameOver;
      }
    } else {
      clock.restart();
    }

    renderer.beginframe();
    if (currentState == GameState::Menu) {
      renderer.window().setView(ui_view);
      preview1.setColor(selected == 1 ? sf::Color::White
                                      : sf::Color(100, 100, 100));
      preview2.setColor(selected == 2 ? sf::Color::White
                                      : sf::Color(100, 100, 100));
      uiText.setString("Choose tank skin: arrows/A-D\nPress Enter to continue");
      renderer.window().draw(menuBg);
      renderer.window().draw(preview1);
      renderer.window().draw(preview2);
      renderer.window().draw(uiText);
    } else if (currentState == GameState::NetworkMode ||
               currentState == GameState::JoinInput ||
               currentState == GameState::WaitingForOpponent) {
      renderer.window().setView(ui_view);
      renderer.window().draw(menuBg);
      renderer.window().draw(uiText);
    } else if (currentState == GameState::Gaming) {
      renderer.window().setView(game_view);
      map.render(renderer);
    } else if (currentState == GameState::GameOver) {
      renderer.window().setView(ui_view);
      uiText.setString(game_over_text);
      renderer.window().draw(menuBg);
      renderer.window().draw(uiText);
    }
    renderer.endframe();
  }
  return 0;
}
