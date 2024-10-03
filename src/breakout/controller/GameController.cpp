#include <breakout/controller/GameController.hpp>
#include <iostream>

namespace breakout::controller {

auto GameController::run() -> void {
  std::visit(m_visitor, m_model);
}

GameController::GameController() {
  m_visitor = {
    [this](model::GameStateMainMenu const& state)  -> void { enter_main_menu(state);     },
    [this](model::GameStatePauseMenu const& state) -> void { enter_pause_menu(state);    },
    [this](model::GameStateStarting const& state)  -> void { enter_game_starting(state); },
    [this](model::GameStateActive const& state)    -> void { enter_game_active(state);   },
  };
}

GameController::~GameController() {
  // If I've done my job right elsewhere, this is redundant.
  // But it doesn't hurt anything and it ensures the terminal
  // state is restored.
  m_view.exit_main_loop();
}

auto GameController::enter_main_menu(model::GameStateMainMenu const& state) -> void {
  m_view.render_main_menu(state, {
    []() -> void {
      std::cout << "Handled play button!\n";
    }, 
    [this]() -> void {
      m_view.exit_main_loop();
    }
  });
}

auto GameController::enter_pause_menu(model::GameStatePauseMenu const& state) -> void {
  m_view.render_pause_menu(state);
}

auto GameController::enter_game_starting(model::GameStateStarting const& state) -> void {
  m_view.render_game_starting(state);
}

auto GameController::enter_game_active(model::GameStateActive const& state) -> void {
  m_view.render_game_active(state);
}

} // namespace breakout::controller
