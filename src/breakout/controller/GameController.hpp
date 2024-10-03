#ifndef BREAKOUT_CONTROLLER_HPP
#define BREAKOUT_CONTROLLER_HPP

#include <functional>
#include <variant>
#include <breakout/model/GameState.hpp>
#include <breakout/view/GameView.hpp>

namespace breakout::controller {

// A helper class for visiting variants. This allows for
// defining a set of overloaded callables for a variant
// type, as opposed to a single callable capable of handling
// any variant type.
// See https://en.cppreference.com/w/cpp/utility/variant/visit
template<typename... Ts>
struct Visitor : Ts... { using Ts::operator()...; };
// An deduction guide for the Visitor class. This allows the
// compiler to deduce template arguments rather than requiring
// them to explicitly defined.
template<typename... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

class GameController {
public:
  // Primary entrypoint for the Breakout game.
  auto run() -> void;

  GameController();
  ~GameController();

  GameController(const GameController&) = delete;
  GameController& operator=(const GameController&) = delete;
  GameController(GameController&&) = delete;
  GameController& operator=(GameController&&) = delete;

private:

  // The current game state. The ordering of template types is
  // important here, as the first alternative is initialized
  // by default.
  std::variant<model::GameStateMainMenu,
  /**/         model::GameStatePauseMenu,
  /**/         model::GameStateStarting,
  /**/         model::GameStateActive> m_model;

  view::GameView m_view;

  using VisitMainMenu  = std::function<void(model::GameStateMainMenu const&)>;
  using VisitPauseMenu = std::function<void(model::GameStatePauseMenu const&)>;
  using VisitStarting  = std::function<void(model::GameStateStarting const&)>;
  using VisitActive    = std::function<void(model::GameStateActive const&)>;
  Visitor<VisitMainMenu,
  /**/    VisitPauseMenu,
  /**/    VisitStarting,
  /**/    VisitActive> m_visitor;

  auto enter_main_menu(model::GameStateMainMenu const&)     -> void;
  auto enter_pause_menu(model::GameStatePauseMenu const&)   -> void;
  auto enter_game_starting(model::GameStateStarting const&) -> void;
  auto enter_game_active(model::GameStateActive const&)     -> void;

};

} // namespace breakout::controller

#endif

