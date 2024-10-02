#include <breakout/view/GameView.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>

namespace breakout::view {

auto GameView::render_main_menu(model::GameStateMainMenu const&) -> void {
  using namespace ftxui;

  auto play_button = Button("  Play  ", []() -> void {}, ButtonOption::Simple());
  auto quit_button = Button("  Quit  ", []() -> void {}, ButtonOption::Simple());

  auto component = Renderer([&]() -> Element {
    return vbox({
      filler(),
      hbox({
        filler(),
        vbox({
          text(R"(                                                )"),
          text(R"(    ____                 _               _      )"),
          text(R"(   | __ ) _ __ ___  __ _| | _____  _   _| |_    )"),
          text(R"(   |  _ \| '__/ _ \/ _` | |/ / _ \| | | | __|   )"),
          text(R"(   | |_) | | |  __/ (_| |   < (_) | |_| | |_    )"),
          text(R"(   |____/|_|  \___|\__,_|_|\_\___/ \__,_|\__|   )"),
          text(R"(                                                )"),
          text(R"(                                                )"),
        }) | borderRounded,
        filler(),
      }),
      filler(),
      hbox({
        filler(),
        play_button->Render(),
        filler(),
      }),
      filler(),
      hbox({
        filler(),
        quit_button->Render(),
        filler(),
      }),
      filler(),
    });
  });

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(component);
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
