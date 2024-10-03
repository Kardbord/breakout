#ifndef BREAKOUT_GAMESTATE_HPP
#define BREAKOUT_GAMESTATE_HPP

namespace breakout::model {

class GameStateMainMenu {
public:
  GameStateMainMenu() = default;
  ~GameStateMainMenu() = default;

  GameStateMainMenu(const GameStateMainMenu&) = default;
  GameStateMainMenu& operator=(const GameStateMainMenu&) = default;
  GameStateMainMenu(GameStateMainMenu&&) = default;
  GameStateMainMenu& operator=(GameStateMainMenu&&) = default;

protected:
};

class GameStatePauseMenu {
public:
  GameStatePauseMenu() = default;
  ~GameStatePauseMenu() = default;

  GameStatePauseMenu(const GameStatePauseMenu&) = default;
  GameStatePauseMenu& operator=(const GameStatePauseMenu&) = default;
  GameStatePauseMenu(GameStatePauseMenu&&) = default;
  GameStatePauseMenu& operator=(GameStatePauseMenu&&) = default;

protected:
};

class GameStateStarting {
public:
  GameStateStarting() = default;
  ~GameStateStarting() = default;

  GameStateStarting(const GameStateStarting&) = default;
  GameStateStarting& operator=(const GameStateStarting&) = default;
  GameStateStarting(GameStateStarting&&) = default;
  GameStateStarting& operator=(GameStateStarting&&) = default;

protected:
};

class GameStateActive {
public:
  GameStateActive() = default;
  ~GameStateActive() = default;

  GameStateActive(const GameStateActive&) = default;
  GameStateActive& operator=(const GameStateActive&) = default;
  GameStateActive(GameStateActive&&) = default;
  GameStateActive& operator=(GameStateActive&&) = default;

protected:
};

} // namespace breakout::model

#endif
