#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map/Map.hpp>
#include <network/NetworkManager.hpp>
#include <render/MenuRender.hpp>
#include <Player.hpp>
#include <weapon/Gun.hpp>

class Game {
public:
    Game(Renderer& renderer);
    void HandleEvents(sf::Event& event);
    void Update(float dt);
    void Draw();
    void SetRelayout();

private:
    Renderer& renderer_;
    Map map_;
    NetworkManager net_;
    GameState currentState_ = GameState::Menu;
    int selected_ = 1;
    bool host_authority_ = false;
    std::string inputCode_, myRoomCode_, game_over_text_;
    Player* local_player_ = nullptr;
    Player* remote_player_ = nullptr;
    sf::Clock clock_;
    sf::Music menu_music_, engine_music_;
    sf::Sound shot_sound_, win_sound_;
    sf::SoundBuffer shot_buf_, win_buf_;
    sf::Font font_;
    sf::Text uiText_;
    sf::Sprite menuBg_, preview1_, preview2_;
    sf::View game_view_, ui_view_;
    sf::Texture menuBgTex_, skin1Tex_, skin2Tex_;

    void RelayoutUI();
    void CreatePlayers(bool host_role);
    void ReturnToMenu();
    std::string GenerateLobbyCode();
    Vec2 TileToWorld(int tx, int ty);
    bool ValidSpawn(int tx, int ty);
    Vec2 PickSpawn(const std::vector<std::pair<int, int>>& candidates, const Vec2& fallback);
};
