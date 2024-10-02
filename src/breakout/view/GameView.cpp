#include <iostream>
#include <breakout/view/GameView.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>

namespace breakout::view {

auto GameView::render_main_menu(model::GameStateMainMenu const&) -> void {
  using ftxui::vbox;
  using ftxui::hbox;
  using ftxui::text;
  using ftxui::borderRounded;
  using ftxui::filler;
  using ftxui::Screen;
  using ftxui::Render;

  auto document = vbox({
    filler(),
    hbox(
      filler(),
      vbox(
        text(R"(                                                )"),
        text(R"(    ____                 _               _      )"),
        text(R"(   | __ ) _ __ ___  __ _| | _____  _   _| |_    )"),
        text(R"(   |  _ \| '__/ _ \/ _` | |/ / _ \| | | | __|   )"),
        text(R"(   | |_) | | |  __/ (_| |   < (_) | |_| | |_    )"),
        text(R"(   |____/|_|  \___|\__,_|_|\_\___/ \__,_|\__|   )"),
        text(R"(                                                )"),
        text(R"(                                                )")
      ) | borderRounded,
      filler()
    ),
    filler(),
    filler(),
  });

  auto screen = Screen::Create(ftxui::Dimension::Full());
  Render(screen, document);
  screen.Print();
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
