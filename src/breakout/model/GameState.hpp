#ifndef BREAKOUT_GAMESTATE_HPP
#define BREAKOUT_GAMESTATE_HPP

#include <ftxui/component/event.hpp>
#include <variant>
#include <cinttypes>

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

class GameStateHelpMenu : public GameStateBase {
public:
  GameStateHelpMenu() = default;
  ~GameStateHelpMenu() = default;

  GameStateHelpMenu(const GameStateHelpMenu&) = default;
  GameStateHelpMenu& operator=(const GameStateHelpMenu&) = default;
  GameStateHelpMenu(GameStateHelpMenu&&) = default;
  GameStateHelpMenu& operator=(GameStateHelpMenu&&) = default;

protected:
};

class GameStateActive : public GameStateBase {
public:

  static inline constexpr uint32_t BRICKS_PER_ROW = 14;
  static inline constexpr uint32_t ROWS_OF_BRICKS = 8;

  // Amount of space taken on the board by a brick.
  static inline constexpr uint32_t BRICK_WIDTH  = 10;
  static inline constexpr uint32_t BRICK_HEIGHT = 2;

  // Amount of space taken on the board by the paddle
  static inline constexpr uint32_t PADDLE_WIDTH  = BRICK_WIDTH * 2;
  static inline constexpr uint32_t PADDLE_HEIGHT = 2;

  // Amount of white space between the bricks and the paddle
  static inline constexpr uint32_t EMPTY_ROWS = ROWS_OF_BRICKS * BRICK_HEIGHT * 3;

  // Total game board dimensions.
  static inline constexpr uint32_t GAME_BOARD_WIDTH  = BRICK_WIDTH * BRICKS_PER_ROW;
  static inline constexpr uint32_t GAME_BOARD_HEIGHT = EMPTY_ROWS + (ROWS_OF_BRICKS * BRICK_HEIGHT);

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
/**/                           model::GameStateHelpMenu,
/**/                           model::GameStateActive>;

} // namespace breakout::model

#endif
