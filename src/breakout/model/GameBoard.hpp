#ifndef BREAKOUT_GAMEBOARD_HPP
#define BREAKOUT_GAMEBOARD_HPP

#include <breakout/model/GameBoardCell.hpp>
#include <cinttypes>
#include <array>
#include <functional>

namespace breakout::model {

class GameBoard {
public:

  static inline constexpr uint32_t BRICKS_PER_ROW = 14;
  static inline constexpr uint32_t ROWS_OF_BRICKS = 8;

  // Amount of space taken on the board by a brick.
  static inline constexpr uint32_t BRICK_WIDTH  = 16;
  static inline constexpr uint32_t BRICK_HEIGHT = 2;

  // Amount of space taken on the board by the paddle
  static inline constexpr uint32_t PADDLE_WIDTH  = BRICK_WIDTH * 2;
  static inline constexpr uint32_t PADDLE_HEIGHT = 1;

  // Dimensions of the ball on the board
  static inline constexpr uint32_t BALL_WIDTH  = 2;
  static inline constexpr uint32_t BALL_HEIGHT = 1;

  // The number of empty game board rows between the bricks and the paddle,
  // including the row on which the paddle is drawn.
  static inline constexpr uint32_t EMPTY_ROWS = ROWS_OF_BRICKS * BRICK_HEIGHT * 4;

  // Total game board dimensions.
  static inline constexpr uint32_t BOARD_WIDTH  = BRICK_WIDTH * BRICKS_PER_ROW;
  static inline constexpr uint32_t BOARD_HEIGHT = EMPTY_ROWS + (ROWS_OF_BRICKS * BRICK_HEIGHT);

  GameBoard();
  ~GameBoard() = default;

  GameBoard(const GameBoard&) = default;
  GameBoard& operator=(const GameBoard&) = default;
  GameBoard(GameBoard&&) = default;
  GameBoard& operator=(GameBoard&&) = default;

  auto reset_board() -> void;

  // Is this cell the beginning of a brick?
  static auto is_brick_start(uint32_t idx) -> bool;

  // Is this cell the end of a brick?
  static auto is_brick_end(uint32_t idx) -> bool;

  // Is this cell the start of a row?
  static auto is_row_start(uint32_t idx) -> bool;

  // Is this cell the end of a row?
  static auto is_row_end(uint32_t idx) -> bool;

  using CellFunctor = std::function<void(GameBoardCell const&)>;
  auto for_each_cell(CellFunctor const &f) const -> void;

  auto shift_paddle_left(uint32_t shift = BOARD_WIDTH / 25) -> void;
  auto shift_paddle_right(uint32_t shift = BOARD_WIDTH / 25) -> void;

  enum class BallTrajectory {
    UpLeft,
    UpRight,
    DownLeft,
    DownRight
  };

  auto shift_ball(uint32_t shift = 1) -> void;

private:

  static inline constexpr uint32_t BRICK_START_IDX = 0;
  static inline constexpr uint32_t BRICK_END_IDX   = (BOARD_WIDTH * ROWS_OF_BRICKS * BRICK_HEIGHT) - 1;

  // Convert x/y coordinates to m_board index.
  auto coords_to_idx(uint32_t x, uint32_t y) const -> uint32_t;

  // Convert m_board index to x/y coordinates.
  auto idx_to_coords(uint32_t idx) const -> std::tuple<uint32_t, uint32_t>;

  auto move_ball(uint32_t ball_start_x, uint32_t ball_start_y) -> void;
  auto move_ball(uint32_t ball_start_idx) -> void;
  auto move_paddle(uint32_t paddle_start_x, uint32_t paddle_start_y) -> void;
  auto move_paddle(uint32_t paddle_start_idx) -> void;

  auto reset_bricks() -> void;
  auto reset_ball() -> void;
  auto reset_paddle() -> void;

  std::array<GameBoardCell, BOARD_WIDTH * BOARD_HEIGHT> m_board;

  // Current position of the first cell of the paddle
  uint32_t m_paddle_start_idx{0};

  BallTrajectory m_ball_trajectory{BallTrajectory::DownRight};

  // Current position of the first cell of the ball
  uint32_t m_ball_start_idx{0};

};

} // namespace breakout::model

#endif
