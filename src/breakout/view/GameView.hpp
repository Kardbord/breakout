#ifndef BREAKOUT_GAMEVIEW_HPP
#define BREAKOUT_GAMEVIEW_HPP

#include <breakout/model/GameState.hpp>
#include <breakout/utils/Visitor.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/task.hpp>
#include <ftxui/dom/node.hpp>
#include <functional>

namespace breakout::view {

using EventHandler = std::function<bool(ftxui::Event)>;

namespace Event {

const ftxui::Event QuitButton         = ftxui::Event::Special("QuitButton");
const ftxui::Event MainMenuPlayButton = ftxui::Event::Special("MainMenuPlayButton");

}

class GameView {
public:
  GameView() = delete;
  GameView(model::GameState const&, EventHandler const&);
  ~GameView() = default;

  GameView(const GameView&) = delete;
  GameView& operator=(const GameView&) = delete;
  GameView(GameView&&) = delete;
  GameView& operator=(GameView&&) = delete;

  auto main_loop() -> void;
  auto exit_main_loop() -> void;

  auto render_main_menu(model::GameStateMainMenu const&)     -> ftxui::Element;
  auto render_pause_menu(model::GameStatePauseMenu const&)   -> ftxui::Element;
  auto render_game_starting(model::GameStateStarting const&) -> ftxui::Element;
  auto render_game_active(model::GameStateActive const&)     -> ftxui::Element;

private:
  ftxui::ScreenInteractive m_screen{ftxui::ScreenInteractive::Fullscreen()};

  using VisitMainMenu  = std::function<ftxui::Element(model::GameStateMainMenu const&)>;
  using VisitPauseMenu = std::function<ftxui::Element(model::GameStatePauseMenu const&)>;
  using VisitStarting  = std::function<ftxui::Element(model::GameStateStarting const&)>;
  using VisitActive    = std::function<ftxui::Element(model::GameStateActive const&)>;

  utils::Visitor<VisitMainMenu,
  /**/    VisitPauseMenu,
  /**/    VisitStarting,
  /**/    VisitActive> m_visitor;

  ftxui::Component mp_container{nullptr};

  EventHandler m_event_handler;

  ftxui::Component mp_renderer{nullptr};


};

} // namespace breakout::view

#endif

