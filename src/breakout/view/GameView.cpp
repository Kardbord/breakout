#include <breakout/model/GameBoard.hpp>
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
#include <stdexcept>
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
    text("See this help menu with Esc. ⚠️ You won't be able to go back to your game!"),
    text(""),
    text("Quit the game at any time with Ctrl+c"),
  });
}

GameView::GameView(std::weak_ptr<const model::GameState> const p_state, controller::EventHandler const& event_handler):
  mp_state{std::move(p_state)}, m_event_handler{event_handler}
{
  m_visitor = {
    [this](model::GameStateMainMenu const& state)  -> ftxui::Component { return build_main_menu(state); },
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

  renderer |= ftxui::CatchEvent([&](ftxui::Event event) -> bool { return m_event_handler(event); });
  exit_main_loop();
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  m_exit_closure = screen.ExitLoopClosure();
  m_refresh_closure = [&screen]() -> void {
    screen.PostEvent(Event::RefreshRequested);
  };
  screen.Loop(renderer);
}

auto GameView::refresh() -> void {
  m_refresh_closure.value_or([]() -> void {})();
}

auto GameView::exit_main_loop() -> void {
  m_exit_closure.value_or([]() -> void {})();
}

auto GameView::build_main_menu(model::GameStateMainMenu const&) const -> ftxui::Component {
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

auto GameView::build_game_active(model::GameStateActive const &state) const -> ftxui::Component {
  using namespace::ftxui;
  using GB  = ::breakout::model::GameBoard;
  using GBC = ::breakout::model::GameBoardCell;


  // TODO: show scoreboard
  return Renderer([&state]() -> Element {
    Canvas canvas(GB::BOARD_WIDTH, GB::BOARD_HEIGHT);

    { // Scope for indices
      int x_idx = 0;
      int y_idx = 0;
      state.for_each_game_board_cell([&x_idx, &y_idx, &canvas](GBC const &cell) -> void {
        if (!model::GameBoard::is_brick_end(x_idx, y_idx)) {
          switch (cell.get_cell_type()) {
            case GBC::CellType::EMPTY:
              break;
            case GBC::CellType::BRICK_RED:
              canvas.DrawBlock(x_idx, y_idx, true, Color::Red);
              break;
            case GBC::CellType::BRICK_ORANGE:
              canvas.DrawBlock(x_idx, y_idx, true, Color::DarkOrange);
              break;
            case GBC::CellType::BRICK_GREEN:
              canvas.DrawBlock(x_idx, y_idx, true, Color::Green);
              break;
            case GBC::CellType::BRICK_YELLOW:
              canvas.DrawBlock(x_idx, y_idx, true, Color::Yellow);
              break;
            case GBC::CellType::BALL:
              canvas.DrawBlock(x_idx, y_idx, true, Color::White);
              break;
            case GBC::CellType::PADDLE:
              canvas.DrawBlock(x_idx, y_idx, true, Color::White);
              break;
            default:
              throw std::logic_error{"A bug in the program caused us to encounter an unkown cell type. This should be reported."};
          }
        }

        if (model::GameBoard::is_row_end(x_idx, y_idx)) {
          x_idx = 0;
          ++y_idx;
        } else {
          ++x_idx;
        }
      });
    }

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

auto GameView::build_help_menu(model::GameStateHelpMenu const&) const -> ftxui::Component {
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
