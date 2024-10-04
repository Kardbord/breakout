#ifndef BREAKOUT_CONTROLLER_HPP
#define BREAKOUT_CONTROLLER_HPP

#include <breakout/model/GameState.hpp>
#include <breakout/view/GameView.hpp>
#include <breakout/utils/Visitor.hpp>
#include <ftxui/component/event.hpp>

namespace breakout::controller {

class GameController {
public:
  // Primary entrypoint for the Breakout game.
  auto run() -> void;

  GameController();
  ~GameController() = default;

  GameController(const GameController&) = delete;
  GameController& operator=(const GameController&) = delete;
  GameController(GameController&&) = delete;
  GameController& operator=(GameController&&) = delete;

private:

  std::shared_ptr<model::GameState> mp_state{nullptr};

  view::GameView m_view;

  using VisitMainMenu  = std::function<bool(model::GameStateMainMenu const&)>;
  using VisitPauseMenu = std::function<bool(model::GameStatePauseMenu const&)>;
  using VisitStarting  = std::function<bool(model::GameStateStarting const&)>;
  using VisitActive    = std::function<bool(model::GameStateActive const&)>;

  utils::Visitor<VisitMainMenu,
  /**/    VisitPauseMenu,
  /**/    VisitStarting,
  /**/    VisitActive> m_visitor;

  auto handle_event(ftxui::Event) -> bool;

  auto handle_main_menu_events(model::GameStateMainMenu const&)     -> bool;
  auto handle_pause_menu_events(model::GameStatePauseMenu const&)   -> bool;
  auto handle_game_starting_events(model::GameStateStarting const&) -> bool;
  auto handle_game_active_events(model::GameStateActive const&)     -> bool;
};

} // namespace breakout::controller

#endif

