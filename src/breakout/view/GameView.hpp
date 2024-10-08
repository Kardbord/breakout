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
#include <memory>
#include <optional>

namespace breakout::view {

using EventHandler = std::function<bool(ftxui::Event)>;

namespace Event {

const ftxui::Event QuitButton         = ftxui::Event::Special("QuitButton");
const ftxui::Event MainMenuPlayButton = ftxui::Event::Special("MainMenuPlayButton");
const ftxui::Event HelpButton     = ftxui::Event::Special("HelpButton");

}

class GameView {
public:
  GameView() = delete;
  GameView(std::weak_ptr<const model::GameState>, EventHandler const&);
  ~GameView() = default;

  GameView(const GameView&) = delete;
  GameView& operator=(const GameView&) = delete;
  GameView(GameView&&) = delete;
  GameView& operator=(GameView&&) = delete;

  // Renders a new screen based on the current game state
  // obtained from the observer provided during initialization.
  //
  // Exits the current screen's main loop (if there is one),
  // updates m_exit_closure, and enters the new screen's
  // main loop.
  //
  // ** ATTENTION ** This is a blocking call! The controller should
  // only call it after completing all other event logic.
  auto render() -> void;
  auto exit_main_loop() -> void;

private:

  // A Closure that can be used to exit the current main screen loop.
  std::optional<ftxui::Closure> m_exit_closure{std::nullopt};

  // A non-owning, read-only reference to the game state.
  const std::weak_ptr<const model::GameState> mp_state;

  using VisitMainMenu  = std::function<ftxui::Component(model::GameStateMainMenu const&)>;
  using VisitPauseMenu = std::function<ftxui::Component(model::GameStatePauseMenu const&)>;
  using VisitHelpMenu  = std::function<ftxui::Component(model::GameStateHelpMenu const&)>;
  using VisitActive    = std::function<ftxui::Component(model::GameStateActive const&)>;

  utils::Visitor<VisitMainMenu,
  /**/    VisitPauseMenu,
  /**/    VisitHelpMenu,
  /**/    VisitActive> m_visitor;

  EventHandler m_event_handler;

  auto build_main_menu(model::GameStateMainMenu const&)   -> ftxui::Component;
  auto build_pause_menu(model::GameStatePauseMenu const&) -> ftxui::Component;
  auto build_help_menu(model::GameStateHelpMenu const&)   -> ftxui::Component;
  auto build_game_active(model::GameStateActive const&)   -> ftxui::Component;

};

} // namespace breakout::view

#endif

