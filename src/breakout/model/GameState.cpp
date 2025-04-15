#include <breakout/model/GameState.hpp>
#include <chrono>
#include <ratio>
#include <thread>

namespace breakout::model {

GameStateActive::GameStateActive(GameStateActive&& other): GameStateBase{std::move(other)} {
  m_ball_engine_sentinel = other.m_ball_engine_sentinel.load();
  other.stop_ball_engine();
  m_board = std::move(other.m_board);
  if (m_ball_engine_sentinel) {
    start_ball_engine();
  }
}

auto GameStateActive::operator=(GameStateActive&& other) -> GameStateActive& {
  if (this != &other) {
    GameStateBase::operator=(std::move(other));
    stop_ball_engine();
    m_ball_engine_sentinel = other.m_ball_engine_sentinel.load();
    other.stop_ball_engine();
    m_board = std::move(other.m_board);
    if (m_ball_engine_sentinel) {
      start_ball_engine();
    }
  }
  return *this;
}

GameStateActive::~GameStateActive() {
  stop_ball_engine();
}

auto GameStateBase::set_last_event(ftxui::Event e) -> void {
  m_last_event = e;
}

auto GameStateBase::get_last_event() const -> ftxui::Event {
  return m_last_event;
}

auto GameStateActive::for_each_game_board_cell(GameBoard::CellFunctor const &f) const -> void {
  m_board.for_each_cell(f);
}

auto GameStateActive::shift_paddle_left() -> void {
  m_board.shift_paddle_left();
}

auto GameStateActive::shift_paddle_right() -> void {
  m_board.shift_paddle_right();
}

auto GameStateActive::start_ball_engine() -> void {
  if (m_ball_engine_thread.has_value()) {
    // The ball engine thread is already started.
    return;
  }

  m_ball_engine_sentinel = true;
  m_ball_engine_thread = std::thread{[this]() -> void {
    ball_engine_loop();
  }};
}

auto GameStateActive::stop_ball_engine() -> void {
  if (!m_ball_engine_thread.has_value()) {
    // The ball engine thread is already stopped.
    return;
  }

  m_ball_engine_sentinel = false;
  if (m_ball_engine_thread.value().joinable()) {
    m_ball_engine_thread.value().join();
  }
  m_ball_engine_thread.reset();
}

auto GameStateActive::ball_engine_loop() -> void {
  static constexpr std::chrono::duration<double, std::ratio<1>> k_update_interval{1.0 / k_ball_engine_update_rate_hz};
  std::chrono::steady_clock::time_point frame_start;
  std::chrono::steady_clock::time_point frame_end;
  std::chrono::steady_clock::duration frame_duration;

  while (m_ball_engine_sentinel) {
    frame_start = std::chrono::steady_clock::now();
    m_board.shift_ball();
    frame_end = std::chrono::steady_clock::now();
    frame_duration = frame_start - frame_end;
    if (frame_duration < k_update_interval) {
      std::this_thread::sleep_for(k_update_interval - frame_duration);
    }
  }
}

} // namespace breakout::model
