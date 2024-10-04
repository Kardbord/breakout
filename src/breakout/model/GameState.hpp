#ifndef BREAKOUT_GAMESTATE_HPP
#define BREAKOUT_GAMESTATE_HPP

#include <ftxui/component/event.hpp>
#include <variant>

namespace breakout::model {

class GameStateBase {
public:
  GameStateBase() = default;
  ~GameStateBase() = default;

  GameStateBase(const GameStateBase&) = default;
  GameStateBase& operator=(const GameStateBase&) = default;
  GameStateBase(GameStateBase&&) = default;
  GameStateBase& operator=(GameStateBase&&) = default;

  auto set_last_event(ftxui::Event e) -> void;
  auto get_last_event() const -> ftxui::Event;

protected:
  // Stores the most recent event passed to the model.
  ftxui::Event m_last_event;
};


class GameStateMainMenu : public GameStateBase {
public:
  GameStateMainMenu() = default;
  ~GameStateMainMenu() = default;

  GameStateMainMenu(const GameStateMainMenu&) = default;
  GameStateMainMenu& operator=(const GameStateMainMenu&) = default;
  GameStateMainMenu(GameStateMainMenu&&) = default;
  GameStateMainMenu& operator=(GameStateMainMenu&&) = default;

protected:
};

class GameStatePauseMenu : public GameStateBase {
public:
  GameStatePauseMenu() = default;
  ~GameStatePauseMenu() = default;

  GameStatePauseMenu(const GameStatePauseMenu&) = default;
  GameStatePauseMenu& operator=(const GameStatePauseMenu&) = default;
  GameStatePauseMenu(GameStatePauseMenu&&) = default;
  GameStatePauseMenu& operator=(GameStatePauseMenu&&) = default;

protected:
};

class GameStateStarting : public GameStateBase {
public:
  GameStateStarting() = default;
  ~GameStateStarting() = default;

  GameStateStarting(const GameStateStarting&) = default;
  GameStateStarting& operator=(const GameStateStarting&) = default;
  GameStateStarting(GameStateStarting&&) = default;
  GameStateStarting& operator=(GameStateStarting&&) = default;

protected:
};

class GameStateActive : public GameStateBase {
public:
  GameStateActive() = default;
  ~GameStateActive() = default;

  GameStateActive(const GameStateActive&) = default;
  GameStateActive& operator=(const GameStateActive&) = default;
  GameStateActive(GameStateActive&&) = default;
  GameStateActive& operator=(GameStateActive&&) = default;

protected:
};

// The ordering of template types is important here, as the
// first alternative is initialized by default.
using GameState = std::variant<model::GameStateMainMenu,
/**/                           model::GameStatePauseMenu,
/**/                           model::GameStateStarting,
/**/                           model::GameStateActive>;

} // namespace breakout::model

#endif
