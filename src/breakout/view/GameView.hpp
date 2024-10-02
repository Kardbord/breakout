#ifndef BREAKOUT_GAMEVIEW_HPP
#define BREAKOUT_GAMEVIEW_HPP

#include <breakout/model/GameState.hpp>

namespace breakout::view {

class GameView {
public:
  GameView() = default;
  ~GameView() = default;

  GameView(const GameView&) = delete;
  GameView& operator=(const GameView&) = delete;
  GameView(GameView&&) = delete;
  GameView& operator=(GameView&&) = delete;

  auto render_main_menu(model::GameStateMainMenu const&)     -> void;
  auto render_pause_menu(model::GameStatePauseMenu const&)   -> void;
  auto render_game_starting(model::GameStateStarting const&) -> void;
  auto render_game_active(model::GameStateActive const&)     -> void;
};

} // namespace breakout::view

#endif

