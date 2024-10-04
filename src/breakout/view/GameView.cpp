#include <breakout/utils/Visitor.hpp>
#include <breakout/view/GameView.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

namespace breakout::view {

GameView::GameView(std::weak_ptr<const model::GameState> const p_state, EventHandler const& event_handler) : mp_state{std::move(p_state)}, m_event_handler{event_handler} {
  m_visitor = {
    [this](model::GameStateMainMenu const& state)  -> ftxui::Component { return build_main_menu(state); },
    [this](model::GameStatePauseMenu const& state) -> ftxui::Component { return build_pause_menu(state); },
    [this](model::GameStateStarting const& state)  -> ftxui::Component { return build_game_starting(state); },
    [this](model::GameStateActive const& state)    -> ftxui::Component { return build_game_active(state); },
  };
}

auto GameView::render() -> void {
  ftxui::Component renderer;

  { // Scope for shared_ptr lock.
    auto p_state = mp_state.lock();
    if (p_state == nullptr) {
      throw std::bad_weak_ptr{};
    }
    renderer = std::visit(m_visitor, *p_state);
  }

  renderer | ftxui::CatchEvent([&](ftxui::Event event) -> bool { return m_event_handler(event); });
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  exit_main_loop();
  m_exit_closure = screen.ExitLoopClosure();
  screen.Loop(renderer);
}

auto GameView::exit_main_loop() -> void {
  m_exit_closure.value_or([]() -> void {})();
}

auto GameView::build_main_menu(model::GameStateMainMenu const&) -> ftxui::Component {
  using namespace ftxui;

  auto p_play_button = Button("  Play  ", [this]() -> void { (void)m_event_handler(Event::MainMenuPlayButton); }, ButtonOption::Simple());
  auto p_quit_button = Button("  Quit  ", [this]() -> void { (void)m_event_handler(Event::QuitButton); }, ButtonOption::Simple());

  auto p_container = Container::Vertical({
    p_play_button,
    p_quit_button,
  });

  // Need to capture these by value so that the references don't go out of scope.
  return Renderer(p_container, [p_container, p_play_button, p_quit_button]() -> Element {
    FlexboxConfig config;
    config.direction = FlexboxConfig::Direction::Column;
    config.wrap = FlexboxConfig::Wrap::NoWrap;
    config.justify_content = FlexboxConfig::JustifyContent::Center;
    config.align_items = FlexboxConfig::AlignItems::Center;
    config.align_content = FlexboxConfig::AlignContent::Center;
    config.SetGap(0, 1);

    return flexbox({
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
      p_play_button->Render(),
      p_quit_button->Render(),
    }, config) | border;
  });
}

auto GameView::build_pause_menu(model::GameStatePauseMenu const&) -> ftxui::Component {
  using ftxui::Renderer;
  using ftxui::Element;
  return Renderer([]() -> Element {
    return ftxui::text("Pause menu placeholder");
  });
}

auto GameView::build_game_starting(model::GameStateStarting const&) -> ftxui::Component {
  using namespace::ftxui;

  return Renderer([] {
    auto brick = []() -> Element {
      return text("████") | color(Color::Blue) | border | size(WIDTH, EQUAL, 5) | size(HEIGHT, EQUAL, 1);
    };

    auto paddle = text("██████████") | color(Color::Red) | size(WIDTH, EQUAL, 10) | size(HEIGHT, EQUAL, 1);

    auto ball = text("●") | color(Color::White) | size(WIDTH, EQUAL, 1) | size(HEIGHT, EQUAL, 1);

    auto bricks = vbox({
      hbox({brick(), brick(), brick(), brick(), brick(), brick()}),
      hbox({brick(), brick(), brick(), brick(), brick(), brick()}),
      hbox({brick(), brick(), brick(), brick(), brick(), brick()}),
    }) | vcenter | hcenter;

    auto controls_overlay = vbox({
      text("Controls:") | bold | underlined | color(Color::Yellow),
      text("  ESC - Pause") | color(Color::White),
      text("  Left/Right Arrow or A/D or J/L - Move Paddle") | color(Color::White),
    }) | border | size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 5) | hcenter | vcenter;
    return vbox({
      filler(),
      controls_overlay,
      filler(),
      bricks,
      filler(),
      ball | vcenter | hcenter,
      filler(),
      paddle | hcenter,
    });
  });
}

auto GameView::build_game_active(model::GameStateActive const&) -> ftxui::Component {
  using ftxui::Renderer;
  using ftxui::Element;
  return Renderer([]() -> Element {
    return ftxui::text("Active game placeholder");
  });
}

} // namespace breakout::view
