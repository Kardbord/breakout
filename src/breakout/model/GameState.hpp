#ifndef BREAKOUT_GAMESTATE_HPP
#define BREAKOUT_GAMESTATE_HPP

namespace breakout::model {

class GameStateMainMenu {
public:
  GameStateMainMenu() = default;
  ~GameStateMainMenu() = default;

  GameStateMainMenu(const GameStateMainMenu&) = delete;
  GameStateMainMenu& operator=(const GameStateMainMenu&) = delete;
  GameStateMainMenu(GameStateMainMenu&&) = delete;
  GameStateMainMenu& operator=(GameStateMainMenu&&) = delete;

protected:
};

class GameStatePauseMenu {
public:
  GameStatePauseMenu() = default;
  ~GameStatePauseMenu() = default;

  GameStatePauseMenu(const GameStatePauseMenu&) = delete;
  GameStatePauseMenu& operator=(const GameStatePauseMenu&) = delete;
  GameStatePauseMenu(GameStatePauseMenu&&) = delete;
  GameStatePauseMenu& operator=(GameStatePauseMenu&&) = delete;

protected:
};

class GameStateStarting {
public:
  GameStateStarting() = default;
  ~GameStateStarting() = default;

  GameStateStarting(const GameStateStarting&) = delete;
  GameStateStarting& operator=(const GameStateStarting&) = delete;
  GameStateStarting(GameStateStarting&&) = delete;
  GameStateStarting& operator=(GameStateStarting&&) = delete;

protected:
};

class GameStateActive {
public:
  GameStateActive() = default;
  ~GameStateActive() = default;

  GameStateActive(const GameStateActive&) = delete;
  GameStateActive& operator=(const GameStateActive&) = delete;
  GameStateActive(GameStateActive&&) = delete;
  GameStateActive& operator=(GameStateActive&&) = delete;

protected:
};

} // namespace breakout::model

#endif
