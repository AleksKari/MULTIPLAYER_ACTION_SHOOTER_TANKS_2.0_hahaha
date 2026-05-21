#pragma once

#include <algorithm>
#include <string>
#include <tuple>
#include <utility>

#include <SFML/Graphics.hpp>

#include <map/Map.hpp>
#include <render/Render.hpp>

enum class GameState {
  Menu,
  NetworkMode,
  JoinInput,
  WaitingForOpponent,
  Gaming,
  GameOver
};

namespace menurenderer {

inline void CenterTextOrigin(sf::Text& text) {
  sf::FloatRect bounds = text.getLocalBounds();
  text.setOrigin(bounds.left + bounds.width / 2.0f,
                 bounds.top + bounds.height / 2.0f);
}

inline void DrawCurrentState(Renderer& renderer, 
                             GameState game_state,
                             const sf::View& ui_view, 
                             const sf::View& game_view,
                             sf::Sprite& menu_bg, 
                             sf::Sprite& preview1,
                             sf::Sprite& preview2, 
                             sf::Text& ui_text,
                             int selected, 
                             Map& map,
                             const std::string& game_over_text) {
  renderer.BeginFrame();

  const sf::Vector2u window_size = renderer.Window().getSize();

  if (game_state == GameState::Menu) {
    renderer.Window().setView(ui_view);

    if (selected == 1) {
      preview1.setColor(sf::Color::White);
      preview1.setScale(5.3f, 5.3f);
      preview2.setColor(sf::Color(100, 100, 100));
      preview2.setScale(4.0f, 4.0f);
    } else {
      preview1.setColor(sf::Color(100, 100, 100));
      preview1.setScale(4.0f, 4.0f);
      preview2.setColor(sf::Color::White);
      preview2.setScale(5.3f, 5.3f);
    }

    ui_text.setString("Choose tank skin: arrows / A-D\nPress Enter to continue");
    ui_text.setOrigin(0.0f, 0.0f);
    ui_text.setPosition(window_size.x * 0.14f, window_size.y * 0.62f);

    renderer.Window().draw(menu_bg);
    renderer.Window().draw(preview1);
    renderer.Window().draw(preview2);
    renderer.Window().draw(ui_text);

  } else if (game_state == GameState::NetworkMode) {
    renderer.Window().setView(ui_view);

    ui_text.setString(
        "CHOOSE MULTIPLAYER MODE\n\n\n[1 / H] Create lobby\n\n[2 / J] "
        "Join lobby");
    ui_text.setOrigin(0.0f, 0.0f);
    ui_text.setPosition(window_size.x * 0.42f, window_size.y * 0.35f);

    renderer.Window().draw(menu_bg);
    renderer.Window().draw(ui_text);

  } else if (game_state == GameState::JoinInput ||
             game_state == GameState::WaitingForOpponent) {
    renderer.Window().setView(ui_view);

    ui_text.setOrigin(0.0f, 0.0f);
    ui_text.setPosition(window_size.x * 0.10f, window_size.y * 0.62f);

    renderer.Window().draw(menu_bg);
    renderer.Window().draw(ui_text);

  } else if (game_state == GameState::Gaming) {
    renderer.Window().setView(game_view);
    map.Render(renderer);

  } else if (game_state == GameState::GameOver) {
    renderer.Window().setView(ui_view);
    ui_text.setString(game_over_text);
    ui_text.setPosition(window_size.x / 2.0f, window_size.y / 2.0f);
    CenterTextOrigin(ui_text);

    renderer.Window().draw(menu_bg);
    renderer.Window().draw(ui_text);
  }

  renderer.EndFrame();
}

}
