#include "breakout/model/GameState.hpp"
#include "breakout/view/GameView.hpp"
#include <breakout/controller/GameController.hpp>
#include <ftxui/component/event.hpp>

namespace breakout::controller {

auto GameController::run() -> void {
  m_view.main_loop();
}

GameController::GameController() : mp_state{std::make_shared<model::GameState>()},
  m_view{mp_state, [this](ftxui::Event e) -> bool { return handle_event(e); }} {
  m_visitor = {
    [this](model::GameStateMainMenu const& state)  -> bool { return handle_main_menu_events(state); },
    [this](model::GameStatePauseMenu const& state) -> bool { return handle_pause_menu_events(state); },
    [this](model::GameStateStarting const& state)  -> bool { return handle_game_starting_events(state); },
    [this](model::GameStateActive const& state)    -> bool { return handle_game_active_events(state); },
  };
}

auto GameController::handle_event(ftxui::Event e) -> bool {
  std::visit(utils::Visitor{
    [e](model::GameStateMainMenu &state)  -> void { state.set_last_event(e); },
    [e](model::GameStatePauseMenu &state) -> void { state.set_last_event(e); },
    [e](model::GameStateStarting &state)  -> void { state.set_last_event(e); },
    [e](model::GameStateActive &state)    -> void { state.set_last_event(e); },
  }, *mp_state);

  return std::visit(m_visitor, *mp_state);
}

auto GameController::handle_main_menu_events(model::GameStateMainMenu const& state) -> bool {
  auto const last_event = state.get_last_event();
  if (last_event == view::Event::QuitButton) {
    m_view.exit_main_loop();
  } else if (last_event == view::Event::MainMenuPlayButton) {
    *mp_state = model::GameStateStarting{};
  } else {
    return false;
  }

  return true;
}

auto GameController::handle_pause_menu_events(model::GameStatePauseMenu const&) -> bool {

  return true;
}

auto GameController::handle_game_starting_events(model::GameStateStarting const&) -> bool {

  return true;
}

auto GameController::handle_game_active_events(model::GameStateActive const&) -> bool {

  return true;
}


} // namespace breakout::controller
