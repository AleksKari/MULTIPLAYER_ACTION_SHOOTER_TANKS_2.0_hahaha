#include <Game.hpp>
#include <collision/Collision.hpp>
#include <tile/EmptyTile.hpp>
#include <iostream>
#include <cctype>

Game::Game(Renderer& renderer) : renderer_(renderer), map_(59, 33) {
    menuBgTex_.loadFromFile("textures/menu_bg.png");
    skin1Tex_.loadFromFile("textures/player.png");
    skin2Tex_.loadFromFile("textures/pink_player.png");
    menuBg_.setTexture(menuBgTex_);
    preview1_.setTexture(skin1Tex_); preview1_.setScale(4.5f, 4.5f);
    preview2_.setTexture(skin2Tex_); preview2_.setScale(4.5f, 4.5f);
    font_.loadFromFile("textures/font.ttf");
    uiText_.setFont(font_);
    menu_music_.openFromFile("sounds/menu_pcm.wav");
    menu_music_.setLoop(true);
    engine_music_.openFromFile("sounds/engine.wav");
    engine_music_.setLoop(true);
    shot_buf_.loadFromFile("sounds/shot_sound.wav");
    win_buf_.loadFromFile("sounds/win_sound.mp3");
    shot_sound_.setBuffer(shot_buf_); win_sound_.setBuffer(win_buf_);
    menu_music_.play();
    RelayoutUI();
}

std::string Game::GenerateLobbyCode() {
    std::string code = "";
    for (int i = 0; i < 4; ++i) code += static_cast<char>('A' + rand() % 26);
    return code;
}

Vec2 Game::TileToWorld(int tx, int ty) { return Vec2(tx * TILESIZE, ty * TILESIZE); }

bool Game::ValidSpawn(int tx, int ty) {
    const Vec2 pos = TileToWorld(tx, ty);
    return map_.IsEmpty(pos.cord_x + 2.0, pos.cord_y + 2.0) && 
           map_.IsEmpty(pos.cord_x + TILESIZE - 2.0, pos.cord_y + TILESIZE - 2.0);
}

Vec2 Game::PickSpawn(const std::vector<std::pair<int, int>>& candidates, const Vec2& fallback) {
    for (const auto& [x, y] : candidates) if (ValidSpawn(x, y)) return TileToWorld(x, y);
    return fallback;
}

void Game::CreatePlayers(bool host_role) {
    const std::vector<std::pair<int, int>> host_spawns = {{2, 2}, {2, 30}, {8, 16}, {12, 16}};
    const std::vector<std::pair<int, int>> guest_spawns = {{56, 30}, {56, 2}, {50, 16}, {46, 16}};
    host_authority_ = host_role;
    
    local_player_ = new Player(host_role ? PickSpawn(host_spawns, {200, 540}) : PickSpawn(guest_spawns, {1700, 540}), 100, 300.0f, std::make_unique<Gun>());
    local_player_->network_id = host_role ? 1 : 2;
    local_player_->is_local = true;
    local_player_->LoadSkin((selected_ == 1) ? "textures/player.png" : "textures/pink_player.png");
    map_.SpawnEntity(local_player_);

    remote_player_ = new Player(host_role ? PickSpawn(guest_spawns, {1700, 540}) : PickSpawn(host_spawns, {200, 540}), 100, 300.0f, std::make_unique<Gun>());
    remote_player_->network_id = host_role ? 2 : 1;
    remote_player_->LoadSkin((selected_ == 1) ? "textures/pink_player.png" : "textures/player.png");
    map_.SpawnEntity(remote_player_);
}

void Game::ReturnToMenu() {
    map_.ResetWorld();
    local_player_ = nullptr; remote_player_ = nullptr;
    host_authority_ = false; inputCode_.clear(); myRoomCode_.clear();
    net_.disconnect(); engine_music_.stop(); win_sound_.stop();
    menu_music_.play(); currentState_ = GameState::Menu;
}

void Game::RelayoutUI() {
    const sf::Vector2u size = renderer_.Window().getSize();
    ui_view_ = sf::View(sf::FloatRect(0.0f, 0.0f, (float)size.x, (float)size.y));
    menuBg_.setScale((float)size.x / menuBgTex_.getSize().x, (float)size.y / menuBgTex_.getSize().y);
    preview1_.setPosition(size.x * 0.30f, size.y * 0.34f);
    preview2_.setPosition(size.x * 0.58f, size.y * 0.34f);
    uiText_.setCharacterSize(std::max(28u, size.y / 28u));
    uiText_.setPosition(size.x * 0.10f, size.y * 0.62f);
}

void Game::HandleEvents(sf::Event& event) {
    if (currentState_ == GameState::Menu) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) selected_ = 1;
            if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) selected_ = 2;
            if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
                currentState_ = GameState::NetworkMode;
                uiText_.setString("Choose mode:\n[1] Create lobby\n[2] Join lobby");
            }
        }
    } else if (currentState_ == GameState::NetworkMode) {
        if (event.type == sf::Event::KeyPressed) {
            bool create_pressed = event.key.code == sf::Keyboard::H || event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1;
            bool join_pressed = event.key.code == sf::Keyboard::J || event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2;

            if (create_pressed) {
                const char* ip = std::getenv("SERVER_IP");
                const char* port_str = std::getenv("SERVER_PORT");
                if (net_.try_join_lobby(ip ? ip : "127.0.0.1", port_str ? std::stoi(port_str) : 5432)) {
                    myRoomCode_ = GenerateLobbyCode();
                    if (net_.join_lobby(myRoomCode_, true) == 1) {
                        CreatePlayers(true);
                        net_.reset_opponent_joined();
                        currentState_ = GameState::WaitingForOpponent;
                    }
                }
            }
            if (join_pressed) { currentState_ = GameState::JoinInput; uiText_.setString("Enter 4-letter Room Code: "); }
            if (event.key.code == sf::Keyboard::Escape) currentState_ = GameState::Menu;
        }
    } else if (currentState_ == GameState::JoinInput) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b' && !inputCode_.empty()) inputCode_.pop_back();
            else if (event.text.unicode < 128 && std::isalpha(event.text.unicode) && inputCode_.length() < 4) 
                inputCode_ += (char)std::toupper(event.text.unicode);
            uiText_.setString("Enter 4-letter Room Code: " + inputCode_);
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && inputCode_.length() == 4) {
            const char* ip = std::getenv("SERVER_IP");
            const char* port_str = std::getenv("SERVER_PORT");
            if (net_.try_join_lobby(ip ? ip : "127.0.0.1", port_str ? std::stoi(port_str) : 5432)) {
                if (net_.join_lobby(inputCode_, false) == 2) {
                    CreatePlayers(false);
                    sf::Packet p; p << PacketType::SkinSync << (sf::Int32)selected_;
                    net_.send_to_all(p);
                    menu_music_.stop(); engine_music_.play();
                    currentState_ = GameState::Gaming; clock_.restart();
                }
            }
        }
    } else if (currentState_ == GameState::Gaming) {
        if (event.type == sf::Event::MouseButtonPressed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)) {
            if (!local_player_->IsDead()) {
                if (host_authority_) {
                    size_t before = map_.projectiles_.size();
                    local_player_->Attack(map_);
                    if (map_.projectiles_.size() > before) shot_sound_.play();
                } else {
                    sf::Packet p; p << PacketType::Shoot; net_.send_to_all(p);
                    shot_sound_.play();
                }
            }
        }
    } else if (currentState_ == GameState::GameOver && event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Escape)) {
        ReturnToMenu();
    }
}

void Game::Update(float dt) {
    if (currentState_ == GameState::WaitingForOpponent) {
        net_.update(map_, local_player_, remote_player_, true);
        uiText_.setString("LOBBY CODE: " + myRoomCode_ + "\n\nWaiting for second player...");
        if (net_.connection_lost()) { game_over_text_ = "Connection lost."; currentState_ = GameState::GameOver; }
        if (net_.opponent_joined()) { currentState_ = GameState::Gaming; clock_.restart(); }
    } else if (currentState_ == GameState::Gaming) {
        bool w = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        bool a = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool s = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        bool d = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        local_player_->SetInput(w, a, s, d);
        if (!host_authority_) {
            sf::Packet p; p << PacketType::PlayerInput << w << a << s << d;
            net_.send_to_all(p);
            local_player_->Update(dt);
        }
        net_.update(map_, local_player_, remote_player_, host_authority_);
        if (host_authority_) {
            map_.Update(dt); Collision::Resolve(map_);
            
            auto weapon = map_.GenerateWeapon();
            if (weapon) {
                auto [x, y, type] = *weapon;
                sf::Packet p; p << PacketType::SpawnWeapon << x << y << type;
                net_.send_to_all(p);
            }
            for(auto& tile : map_.ConsumeRemovedWeaponTiles()) {
                sf::Packet p; p << PacketType::RemoveTile << (int)tile.cord_x << (int)tile.cord_y;
                net_.send_to_all(p);
            }
            sf::Packet state; state << PacketType::GameState; map_.SerializeGameState(state); net_.send_to_all(state);
        }
        if (net_.connection_lost()) { game_over_text_ = "Opponent disconnected."; currentState_ = GameState::GameOver; }
        else if (local_player_->IsDead() || remote_player_->IsDead()) {
            game_over_text_ = (local_player_->IsDead() && remote_player_->IsDead()) ? "Draw." : (local_player_->IsDead() ? "You lose." : "You win!");
            engine_music_.stop(); currentState_ = GameState::GameOver;
        }
    }
}

void Game::SetRelayout() { RelayoutUI(); }