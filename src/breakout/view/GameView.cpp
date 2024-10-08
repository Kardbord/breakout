#include <breakout/utils/Visitor.hpp>
#include <breakout/model/GameState.hpp>
#include <breakout/view/GameView.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <memory>
#include <string>

namespace breakout::view {

auto get_term_width() -> int {
  return ftxui::Terminal::Size().dimx;
}

auto get_term_height() -> int {
  return ftxui::Terminal::Size().dimy;
}

auto get_title_art() -> ftxui::Element {
  using namespace ftxui;
  return vbox({
    text(R"(                                                )"),
    text(R"(    ____                 _               _      )") | color(Color::Red),
    text(R"(   | __ ) _ __ ___  __ _| | _____  _   _| |_    )") | color(Color::Red),
    text(R"(   |  _ \| '__/ _ \/ _` | |/ / _ \| | | | __|   )") | color(Color::DarkOrange),
    text(R"(   | |_) | | |  __/ (_| |   < (_) | |_| | |_    )") | color(Color::Green),
    text(R"(   |____/|_|  \___|\__,_|_|\_\___/ \__,_|\__|   )") | color(Color::Yellow),
    text(R"(                                                )") | color(Color::Yellow),
    text(R"(                                                )"),
  }) | borderRounded | color(Color::Cyan);
}

// Verify that we have space to display the requested element.
// If yes, return that element.
// If no, return a simple element telling the user to resize their terminal.
auto verify_screen_requirement(ftxui::Element p_element) -> ftxui::Element {
  p_element->ComputeRequirement();
  auto requirement = p_element->requirement();

  if (get_term_width() < requirement.min_x || get_term_height() < requirement.min_y) {
    return ftxui::paragraph("Your terminal is too small to properly render the game. Please resize it. :)");
  }

  return p_element;
}

auto get_help_text() -> ftxui::Element {
  using namespace ftxui;
  return vbox({
    text("Controls") | center,
    text(""),
    text("Move the paddle with a/d, h/l, or left/right"),
    text(""),
    text("Pause the game with Esc"),
    text(""),
    text("Quit the game at any time with Ctrl+c"),
  });
}

GameView::GameView(std::weak_ptr<const model::GameState> const p_state, EventHandler const& event_handler) : mp_state{std::move(p_state)}, m_event_handler{event_handler} {
  m_visitor = {
    [this](model::GameStateMainMenu const& state)  -> ftxui::Component { return build_main_menu(state); },
    [this](model::GameStatePauseMenu const& state) -> ftxui::Component { return build_pause_menu(state); },
    [this](model::GameStateHelpMenu const& state)  -> ftxui::Component { return build_help_menu(state); },
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
  exit_main_loop();
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  m_exit_closure = screen.ExitLoopClosure();
  screen.Loop(renderer);
}

auto GameView::exit_main_loop() -> void {
  m_exit_closure.value_or([]() -> void {})();
}

auto GameView::build_main_menu(model::GameStateMainMenu const&) -> ftxui::Component {
  using namespace ftxui;

  auto p_play_button = Button("  Play  ", [this]() -> void { (void)m_event_handler(Event::MainMenuPlayButton); }, ButtonOption::Simple());
  auto p_help_button = Button("  Help  ", [this]() -> void { (void)m_event_handler(Event::HelpButton); }, ButtonOption::Simple());
  auto p_quit_button = Button("  Quit  ", [this]() -> void { (void)m_event_handler(Event::QuitButton); }, ButtonOption::Simple());

  auto p_container = Container::Vertical({
    p_play_button,
    p_help_button,
    p_quit_button,
  });

  // Need to capture these by value so that the references don't go out of scope.
  return Renderer(p_container, [=]() -> Element {
    FlexboxConfig config;
    config.direction = FlexboxConfig::Direction::Column;
    config.wrap = FlexboxConfig::Wrap::NoWrap;
    config.justify_content = FlexboxConfig::JustifyContent::Center;
    config.align_items = FlexboxConfig::AlignItems::Center;
    config.align_content = FlexboxConfig::AlignContent::Center;
    config.SetGap(0, 1);

    auto p_element = flexbox({
      get_title_art(),
      p_play_button->Render(),
      p_help_button->Render(),
      p_quit_button->Render(),
    }, config) | border;
    return verify_screen_requirement(p_element);
  });
}

auto GameView::build_pause_menu(model::GameStatePauseMenu const&) -> ftxui::Component {
  using ftxui::Renderer;
  using ftxui::Element;
  return Renderer([]() -> Element {
    return verify_screen_requirement(ftxui::text("Pause menu placeholder"));
  });
}

auto GameView::build_game_active(model::GameStateActive const&) -> ftxui::Component {
  using namespace::ftxui;
  using GSA = ::breakout::model::GameStateActive;


  return Renderer([]() -> Element {

    auto draw_brick_row = [=](Canvas& canvas, int start_x, int start_y, Color c) -> void {
      for (uint32_t i = 0; i < GSA::GAME_BOARD_WIDTH; ++i) {
        uint32_t x_offset = i * GSA::BRICK_WIDTH;
        for (uint32_t y = 0; y < GSA::BRICK_HEIGHT; ++y) {
          for (uint32_t x = 0; x < GSA::BRICK_WIDTH; ++x) {
            bool is_block_end = (x == 0 || x == GSA::BRICK_WIDTH - 1);
            canvas.DrawBlock(start_x + x_offset + x, start_y + y, true, [is_block_end, c](Pixel &p) -> void {
              if (is_block_end) {
                p.foreground_color = Color::Black;
              } else {
                p.foreground_color = c;
              }
              p.dim = is_block_end;
            });
          }
        }
      }
    };

    auto draw_ball = [=](Canvas& canvas, int center_x, int center_y, Color color) {
      canvas.DrawBlockCircleFilled(center_x, center_y, GSA::BALL_RADIUS, color);
    };

    auto draw_paddle = [=](Canvas& canvas, int start_x, int start_y, Color color) -> void {
      for (uint32_t y = 0; y < GSA::PADDLE_HEIGHT; ++y) {
        for (uint32_t x = 0; x < GSA::PADDLE_WIDTH; ++x) {
          canvas.DrawBlock(start_x + x, start_y + y, true, color);
        }
      }
    };

    Canvas canvas(GSA::GAME_BOARD_WIDTH, GSA::GAME_BOARD_HEIGHT);
    draw_brick_row(canvas, 0, 0,                Color::Red);
    draw_brick_row(canvas, 0, GSA::BRICK_HEIGHT,     Color::Red);
    draw_brick_row(canvas, 0, GSA::BRICK_HEIGHT * 2, Color::DarkOrange);
    draw_brick_row(canvas, 0, GSA::BRICK_HEIGHT * 3, Color::DarkOrange);
    draw_brick_row(canvas, 0, GSA::BRICK_HEIGHT * 4, Color::Green);
    draw_brick_row(canvas, 0, GSA::BRICK_HEIGHT * 5, Color::Green);
    draw_brick_row(canvas, 0, GSA::BRICK_HEIGHT * 6, Color::Yellow);
    draw_brick_row(canvas, 0, GSA::BRICK_HEIGHT * 7, Color::Yellow);


    const auto center_x = GSA::GAME_BOARD_WIDTH / 2;
    const auto center_y = GSA::GAME_BOARD_HEIGHT / 2;
    draw_ball(canvas, center_x, center_y, Color::White);

    int paddle_x_position = (GSA::GAME_BOARD_WIDTH / 2) - (GSA::PADDLE_WIDTH / 2);
    draw_paddle(canvas, paddle_x_position, GSA::GAME_BOARD_HEIGHT - GSA::PADDLE_HEIGHT - 1, Color::White);

    FlexboxConfig config;
    config.direction = FlexboxConfig::Direction::Column;
    config.wrap = FlexboxConfig::Wrap::NoWrap;
    config.justify_content = FlexboxConfig::JustifyContent::Center;
    config.align_items = FlexboxConfig::AlignItems::Center;
    config.align_content = FlexboxConfig::AlignContent::Center;
    config.SetGap(0, 1);

    auto p_element = flexbox({
      get_title_art(),
      ftxui::canvas(canvas) | border | center,
    }, config) | border;
    return verify_screen_requirement(p_element);
  });
}

auto GameView::build_help_menu(model::GameStateHelpMenu const&) -> ftxui::Component {
  using namespace ftxui;

  auto p_main_menu_button = Button("  Main Menu  ", [this]() -> void { (void)m_event_handler(Event::MainMenuButton); }, ButtonOption::Simple());

  auto p_container = Container::Vertical({
    p_main_menu_button,
  });

  return Renderer(p_container, [=]() -> Element {
    FlexboxConfig config;
    config.direction = FlexboxConfig::Direction::Column;
    config.wrap = FlexboxConfig::Wrap::NoWrap;
    config.justify_content = FlexboxConfig::JustifyContent::Center;
    config.align_items = FlexboxConfig::AlignItems::Center;
    config.align_content = FlexboxConfig::AlignContent::Center;
    config.SetGap(0, 1);

    auto p_element = flexbox({
      get_title_art(),
      p_main_menu_button->Render(),
      get_help_text(),
    }, config) | border;
    return verify_screen_requirement(p_element);
  });
}

} // namespace breakout::view
