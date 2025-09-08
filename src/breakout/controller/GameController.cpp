#include <breakout/utils/Logger.hpp>
#include <breakout/model/GameState.hpp>
#include <breakout/view/GameView.hpp>
#include <breakout/controller/GameController.hpp>
#include <ftxui/component/event.hpp>
#include <set>

namespace breakout::controller {

auto GameController::run() -> void {
  m_view.render();
}

GameController::GameController() : mp_state{std::make_shared<model::GameState>()},
  m_view{mp_state, [this](ftxui::Event e) -> bool { return handle_event(e); }} {
  m_visitor = {
    [this](model::GameStateMainMenu& state)  -> bool { return handle_main_menu_events(state); },
    [this](model::GameStatePauseMenu& state) -> bool { return handle_pause_menu_events(state); },
    [this](model::GameStateHelpMenu& state)  -> bool { return handle_help_menu_events(state); },
    [this](model::GameStateActive& state)    -> bool { return handle_game_active_events(state); },
  };
}

auto GameController::handle_event(ftxui::Event e) -> bool {
  LOG << "Handling event " << e.DebugString();
  std::visit(utils::Visitor{
    [e](model::GameStateMainMenu &state)  -> void { state.set_last_event(e); },
    [e](model::GameStatePauseMenu &state) -> void { state.set_last_event(e); },
    [e](model::GameStateHelpMenu &state)  -> void { state.set_last_event(e); },
    [e](model::GameStateActive &state)    -> void { state.set_last_event(e); },
  }, *mp_state);

  return std::visit(m_visitor, *mp_state);
}

auto GameController::handle_main_menu_events(model::GameStateMainMenu& state) -> bool {
  auto const last_event = state.get_last_event();
  if (last_event == view::Event::QuitButton) {
    m_view.exit_main_loop();
  } else if (last_event == view::Event::MainMenuPlayButton) {
    *mp_state = model::GameStateActive{};
    // TODO: Need a way to ensure that the view is updated along with the state as the ball moves
    m_view.render();
  } else if (last_event == view::Event::HelpButton) {
    *mp_state = model::GameStateHelpMenu{};
    m_view.render();
  } else {
    return false;
  }

  return true;
}

auto GameController::handle_pause_menu_events(model::GameStatePauseMenu&) -> bool {
  return false;
}

auto GameController::handle_help_menu_events(model::GameStateHelpMenu& state) -> bool {
  auto const last_event = state.get_last_event();
  if (last_event == view::Event::MainMenuButton) {
    *mp_state = model::GameStateMainMenu{};
    m_view.render();
  } else {
    return false;
  }

  return true;
}

auto GameController::handle_game_active_events(model::GameStateActive& state) -> bool {
  state.start_ball_engine();

  static const std::set<ftxui::Event> paddle_left_evts{
    ftxui::Event::Character("a"),
    ftxui::Event::ArrowLeft,
    ftxui::Event::Character("h"),
  };

  static const std::set<ftxui::Event> paddle_right_evts{
    ftxui::Event::Character("d"),
    ftxui::Event::ArrowRight,
    ftxui::Event::Character("l"),
  };

  auto const last_event = state.get_last_event();

  if (paddle_left_evts.count(last_event) > 0) {
    state.shift_paddle_left();
  } else if (paddle_right_evts.count(last_event) > 0) {
    state.shift_paddle_right();
  } else {
    return false;
  }

  return true;
}


} // namespace breakout::controller
