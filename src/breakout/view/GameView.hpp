#ifndef BREAKOUT_GAMEVIEW_HPP
#define BREAKOUT_GAMEVIEW_HPP

#include <breakout/model/GameState.hpp>
#include <breakout/utils/Visitor.hpp>
#include <breakout/controller/EventHandler.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/task.hpp>
#include <ftxui/dom/node.hpp>
#include <functional>
#include <memory>
#include <optional>

namespace breakout::view {

namespace Event {

static inline const ftxui::Event QuitButton         = ftxui::Event::Special("QuitButton");
static inline const ftxui::Event MainMenuPlayButton = ftxui::Event::Special("MainMenuPlayButton");
static inline const ftxui::Event HelpButton         = ftxui::Event::Special("GoToHelpButton");
static inline const ftxui::Event MainMenuButton     = ftxui::Event::Special("GoToMainMenuButton");
static inline const ftxui::Event RefreshRequested   = ftxui::Event::Special("RefreshRequested");

} // namespace Event

class GameView {
public:
  GameView() = delete;
  GameView(std::weak_ptr<const model::GameState>, controller::EventHandler const&);
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

  // Request that the current rendering be refreshed.
  auto refresh() -> void;

private:

  // A Closure that can be used to exit the current main screen loop.
  std::optional<ftxui::Closure> m_exit_closure{std::nullopt};
  std::optional<ftxui::Closure> m_refresh_closure{std::nullopt};

  // A non-owning, read-only reference to the game state.
  const std::weak_ptr<const model::GameState> mp_state;

  using VisitMainMenu  = std::function<ftxui::Component(model::GameStateMainMenu const&)>;
  using VisitHelpMenu  = std::function<ftxui::Component(model::GameStateHelpMenu const&)>;
  using VisitActive    = std::function<ftxui::Component(model::GameStateActive const&)>;

  utils::Visitor<VisitMainMenu,
  /**/    VisitHelpMenu,
  /**/    VisitActive> m_visitor;

  controller::EventHandler m_event_handler;

  auto build_main_menu(model::GameStateMainMenu const&)   const -> ftxui::Component;
  auto build_help_menu(model::GameStateHelpMenu const&)   const -> ftxui::Component;
  auto build_game_active(model::GameStateActive const&)   const -> ftxui::Component;

};

} // namespace breakout::view

#endif

