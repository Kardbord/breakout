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

auto GameController::enter_main_menu(model::GameStateMainMenu const&) -> void {
  std::cout << "Entering main menu!\n";
}

auto GameController::enter_pause_menu(model::GameStatePauseMenu const&) -> void {
  std::cout << "Entering pause menu!\n";
}

auto GameController::enter_game_starting(model::GameStateStarting const&) -> void {
  std::cout << "Entering starting state!\n";
}

auto GameController::enter_game_active(model::GameStateActive const&) -> void {
  std::cout << "Entering active state!\n";
}

} // namespace breakout::controller
