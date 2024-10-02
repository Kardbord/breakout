#include <breakout/view/GameView.hpp>
#include <iostream>

namespace breakout::view {

auto GameView::render_main_menu(model::GameStateMainMenu const&) -> void {
  std::cout << "Rendering main menu!\n";
}

auto GameView::render_pause_menu(model::GameStatePauseMenu const&)-> void {
  std::cout << "Rendering pause menu!\n";
}

auto GameView::render_game_starting(model::GameStateStarting const&) -> void {

  std::cout << "Rendering game starting!\n";
}

auto GameView::render_game_active(model::GameStateActive const&) -> void {
  std::cout << "Rendering active game!\n";
}

} // namespace breakout::view
