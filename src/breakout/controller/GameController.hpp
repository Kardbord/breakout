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

  using VisitMainMenu  = std::function<bool(model::GameStateMainMenu&)>;
  using VisitHelpMenu  = std::function<bool(model::GameStateHelpMenu&)>;
  using VisitActive    = std::function<bool(model::GameStateActive&)>;

  utils::Visitor<VisitMainMenu,
  /**/    VisitHelpMenu,
  /**/    VisitActive> m_visitor;

  auto handle_event(ftxui::Event) -> bool;

  auto handle_main_menu_events(model::GameStateMainMenu&)   -> bool;
  auto handle_help_menu_events(model::GameStateHelpMenu&)   -> bool;
  auto handle_game_active_events(model::GameStateActive&)   -> bool;
};

} // namespace breakout::controller

#endif

