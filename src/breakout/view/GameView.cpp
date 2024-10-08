#include <breakout/utils/Visitor.hpp>
#include <breakout/view/GameView.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <memory>
#include <stdexcept>
#include <string>

namespace breakout::view {

// These are the default dimensions of most terminal emulators.
constexpr int MIN_TERM_WIDTH  = 80;
constexpr int MIN_TERM_HEIGHT = 24;

// The program may not render correctly if dimensions
// are calculated to be below these thresholds.
constexpr uint32_t MIN_BRICK_WIDTH   = 2;
constexpr uint32_t MIN_BRICK_HEIGHT  = 2;
constexpr uint32_t MIN_PADDLE_WIDTH  = 2;
constexpr uint32_t MIN_PADDLE_HEIGHT = MIN_BRICK_HEIGHT;

constexpr uint32_t BRICKS_PER_ROW = 14;
constexpr uint32_t ROWS_OF_BRICKS = 8;

auto get_term_width() -> uint32_t {
  const auto width = ftxui::Terminal::Size().dimx;
  if (width < MIN_TERM_WIDTH) {
    throw std::runtime_error{"Terminal size must be at least " + std::to_string(MIN_TERM_WIDTH) + "x" + std::to_string(MIN_TERM_HEIGHT)};
  }
  return width;
}

auto get_term_height() -> uint32_t {
  const auto height = ftxui::Terminal::Size().dimy;
  if (height < MIN_TERM_HEIGHT) {
    throw std::runtime_error{"Terminal size must be at least " + std::to_string(MIN_TERM_WIDTH) + "x" + std::to_string(MIN_TERM_HEIGHT)};
  }
  return height;
}

auto get_brick_width() -> uint32_t {
  const auto width = get_term_width() / BRICKS_PER_ROW;
  if (width < MIN_BRICK_WIDTH) {
    throw std::runtime_error{"Calculated brick width is smaller than minimum required: " + std::to_string(width) + " < " + std::to_string(MIN_BRICK_WIDTH)};
  }
  return width;
}

auto get_brick_height() -> uint32_t {
  // Bricks should take up about 25% of the screen, so each brick
  // should take up (25% of screen) / (rows of bricks).
  const auto height = get_term_height() / 4 / ROWS_OF_BRICKS;
  if (height < MIN_BRICK_HEIGHT) {
    throw std::runtime_error{"Calculated brick height is smaller than minimum required: " + std::to_string(height) + " < " + std::to_string(MIN_BRICK_HEIGHT)};
  }
  return height;
}

auto get_paddle_width() -> uint32_t {
  // Paddle should be about 20% of the screen
  const auto width = get_term_width() / 5;
  if (width < MIN_PADDLE_WIDTH) {
    throw std::runtime_error{"Calculated paddle width is smaller than minimum required: " + std::to_string(width) + " < " + std::to_string(MIN_PADDLE_WIDTH)};
  }
  return width;
}

auto get_paddle_height() -> uint32_t {
  uint32_t height;
  try {
    height = get_brick_height();
  } catch (std::runtime_error const &e) {
    throw std::runtime_error{"Paddle height is smaller than minimum allowed brick size -> " + std::string{e.what()}};
  }

  if (height < MIN_PADDLE_HEIGHT) {
    throw std::runtime_error{"Calculated paddle height is smaller than minimum required: " + std::to_string(height) + " < " + std::to_string(MIN_PADDLE_HEIGHT)};
  }
  return height;
}

auto get_num_empty_rows() -> uint32_t {
  return get_term_height() - (ROWS_OF_BRICKS * get_brick_height()) - get_paddle_height();
}

auto get_canvas_width() -> uint32_t {
  return get_brick_width() * BRICKS_PER_ROW;
}

auto get_canvas_height() -> uint32_t {
  return  (get_brick_height() * ROWS_OF_BRICKS) + get_paddle_height() + get_num_empty_rows();
}


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
        text(R"(    ____                 _               _      )") | color(Color::Red),
        text(R"(   | __ ) _ __ ___  __ _| | _____  _   _| |_    )") | color(Color::Red),
        text(R"(   |  _ \| '__/ _ \/ _` | |/ / _ \| | | | __|   )") | color(Color::DarkOrange),
        text(R"(   | |_) | | |  __/ (_| |   < (_) | |_| | |_    )") | color(Color::Green),
        text(R"(   |____/|_|  \___|\__,_|_|\_\___/ \__,_|\__|   )") | color(Color::Yellow),
        text(R"(                                                )") | color(Color::Yellow),
        text(R"(                                                )"),
      }) | borderRounded | color(Color::Cyan),
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

  return Renderer([]() -> Element {
    const auto brick_width   = get_brick_width();
    const auto brick_height  = get_brick_height();
    const auto paddle_width  = get_paddle_width();
    const auto paddle_height = get_paddle_height();
    const auto canvas_width  = get_canvas_width();
    const auto canvas_height = get_canvas_height();

    auto draw_brick_row = [=](Canvas& canvas, int start_x, int start_y, Color color) -> void {
      for (uint32_t i = 0; i < canvas_width; ++i) {
        int x_offset = i * brick_width;
        for (uint32_t y = 0; y < brick_height; ++y) {
          for (uint32_t x = 0; x < brick_width; ++x) {
            canvas.DrawBlock(start_x + x_offset + x, start_y + y, true, color);
          }
        }
      }
    };

    auto draw_paddle = [=](Canvas& canvas, int start_x, int start_y, Color color) -> void {
      for (uint32_t y = 0; y < paddle_height; ++y) {
        for (uint32_t x = 0; x < paddle_width; ++x) {
          canvas.DrawBlock(start_x + x, start_y + y, true, color);
        }
      }
    };

    Canvas canvas(canvas_width, canvas_height);
    draw_brick_row(canvas, 0, 0,                Color::Red);
    draw_brick_row(canvas, 0, brick_height,     Color::Red);
    draw_brick_row(canvas, 0, brick_height * 2, Color::DarkOrange);
    draw_brick_row(canvas, 0, brick_height * 3, Color::DarkOrange);
    draw_brick_row(canvas, 0, brick_height * 4, Color::Green);
    draw_brick_row(canvas, 0, brick_height * 5, Color::Green);
    draw_brick_row(canvas, 0, brick_height * 6, Color::Yellow);
    draw_brick_row(canvas, 0, brick_height * 7, Color::Yellow);

    int paddle_x_position = (canvas_width / 2) - (paddle_width / 2);
    draw_paddle(canvas, paddle_x_position, canvas_height - paddle_height - 1, Color::White);
    return hbox({
      filler(),
      vbox({
        filler(),
        ftxui::canvas(canvas) | border,
        filler(),
      }),
      filler(),
    }) | border;
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
