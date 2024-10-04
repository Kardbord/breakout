#include <breakout/utils/Visitor.hpp>
#include <breakout/view/GameView.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

namespace breakout::view {

GameView::GameView(std::weak_ptr<const model::GameState> const p_state, EventHandler const& event_handler) : mp_state{std::move(p_state)} {
  m_visitor = {
    [this](model::GameStateMainMenu const& state)  -> ftxui::Element { return render_main_menu(state); },
    [this](model::GameStatePauseMenu const& state) -> ftxui::Element { return render_pause_menu(state); },
    [this](model::GameStateStarting const& state)  -> ftxui::Element { return render_game_starting(state); },
    [this](model::GameStateActive const& state)    -> ftxui::Element { return render_game_active(state); },
  };

  m_event_handler = event_handler;
  update_renderer(ftxui::Container::Vertical({}));
}

auto GameView::main_loop() -> void {
  m_screen.Loop(mp_renderer);
}

auto GameView::exit_main_loop() -> void {
  m_screen.Exit();
}

auto GameView::update_renderer(ftxui::Component p_container) -> void {
  using ftxui::Renderer;
  using ftxui::CatchEvent;
  using ftxui::Element;

  auto p_tmp_renderer = ftxui::Renderer(p_container, [&]() -> ftxui::Element {
    auto p_state = mp_state.lock();
    if (p_state == nullptr) {
      throw std::bad_weak_ptr{};
    }
    return std::visit(m_visitor, *p_state);
  });

  auto p_new_renderer = ftxui::CatchEvent(p_tmp_renderer, [&](ftxui::Event event) -> bool {
    return m_event_handler(event);
  });

  mp_renderer.swap(p_new_renderer);
}

auto GameView::render_main_menu(model::GameStateMainMenu const&) -> ftxui::Element {
  using namespace ftxui;

  auto p_play_button = Button("  Play  ", [this]() -> void { (void)m_event_handler(Event::MainMenuPlayButton); }, ButtonOption::Simple());
  auto p_quit_button = Button("  Quit  ", [this]() -> void { (void)m_event_handler(Event::QuitButton); }, ButtonOption::Simple());

  auto p_container = Container::Vertical({
    p_play_button,
    p_quit_button,
  });

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
    p_container->Render(),
  }, config) | border;

}

auto GameView::render_pause_menu(model::GameStatePauseMenu const&) -> ftxui::Element {
  return ftxui::text("Pause menu placeholder");
}

auto GameView::render_game_starting(model::GameStateStarting const&) -> ftxui::Element {
  return ftxui::text("Game starting placeholder");
}

auto GameView::render_game_active(model::GameStateActive const&) -> ftxui::Element {
  return ftxui::text("Active game placeholder\n");
}


} // namespace breakout::view
