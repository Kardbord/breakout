#ifndef BREAKOUT_GAMESTATE_HPP
#define BREAKOUT_GAMESTATE_HPP

#include <atomic>
#include <breakout/controller/EventHandler.hpp>
#include <breakout/model/GameBoard.hpp>
#include <ftxui/component/event.hpp>
#include <optional>
#include <thread>
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

namespace Event {

static inline const ftxui::Event BallMoved = ftxui::Event::Special("BallMoved");

} // namespace Event

class GameStateActive : public GameStateBase {
public:

  constexpr inline static uint32_t k_ball_engine_update_rate_hz = 32;

  GameStateActive() = delete;
  GameStateActive(controller::EventHandler const &h);
  ~GameStateActive();

  GameStateActive(const GameStateActive&) = delete;
  GameStateActive& operator=(const GameStateActive&) = delete;
  GameStateActive(GameStateActive&&);
  GameStateActive& operator=(GameStateActive&&);

  auto for_each_game_board_cell(GameBoard::CellFunctor const &f) const -> void;

  auto shift_paddle_left() -> void;
  auto shift_paddle_right() -> void;

  auto start_ball_engine() -> void;
  auto stop_ball_engine() -> void;

protected:

  auto ball_engine_loop() -> void;

  GameBoard m_board;

  std::atomic_bool m_ball_engine_sentinel;
  std::optional<std::thread> m_ball_engine_thread;

  controller::EventHandler m_event_handler;
};

// The ordering of template types is important here, as the
// first alternative is initialized by default.
using GameState = std::variant<model::GameStateMainMenu,
/**/                           model::GameStatePauseMenu,
/**/                           model::GameStateHelpMenu,
/**/                           model::GameStateActive>;

} // namespace breakout::model

#endif
