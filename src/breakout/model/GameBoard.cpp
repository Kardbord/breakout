#include <breakout/model/GameBoardCell.hpp>
#include <breakout/model/GameBoard.hpp>
#include <stdexcept>

namespace breakout::model {

GameBoard::GameBoard() {
  static_assert(BRICK_WIDTH > 1, "BRICK_WIDTH must be greater than 1 to prevent division by 0.");
  static_assert(BOARD_WIDTH > 1, "BOARD_WIDTH must be greater than 1 to prevent division by 0.");
  static_assert(PADDLE_WIDTH <= BOARD_WIDTH, "PADDLE_WIDTH must be less than or equal to BOARD_WIDTH");
  reset_board();
}

auto GameBoard::reset_board() -> void {
  for (uint32_t i = 0; i < m_board.size(); i += BOARD_WIDTH) {
    if (!is_row_start(i)) {
      throw std::logic_error{"A bug in the program caused the ROW_START property to be set incorrectly. This should be reported."};
    }
    m_board.at(i).add_properties({GameBoardCell::Property::ROW_START});
  }

  for (uint32_t i = BOARD_WIDTH - 1; i < m_board.size(); i += BOARD_WIDTH) {
    if (!is_row_end(i)) {
      throw std::logic_error{"A bug in the program caused the ROW_END property to be set incorrectly. This should be reported."};
    }
    m_board.at(i).add_properties({GameBoardCell::Property::ROW_END});
  }

  for (uint32_t i = 0; i < BOARD_WIDTH; ++i) {
    if (!is_col_start(i)) {
      throw std::logic_error("A bug in the program caused the COL_START property to be set incorrectly. This should be reported.");
    }
    m_board.at(i).add_properties({GameBoardCell::Property::COL_START});
  }

  for (uint32_t i = static_cast<uint32_t>(m_board.size()) - 1; i >= m_board.size() - BOARD_WIDTH; --i) {
    if (!is_col_end(i)) {
      throw std::logic_error("A bug in the program caused the COL_END property to be set incorrectly. This should be reported.");
    }
    m_board.at(i).add_properties({GameBoardCell::Property::COL_END});
  }

  reset_ball();
  reset_paddle();
  reset_bricks();
}

auto GameBoard::reset_bricks() -> void {
  for (auto i = BRICK_START_IDX; i <= BRICK_END_IDX; ++i) {
    if (is_brick_start(i)) {
      m_board.at(i).add_properties({GameBoardCell::Property::BRICK_START});
    } else if (is_brick_end(i)) {
      m_board.at(i).add_properties({GameBoardCell::Property::BRICK_END});
    }

    if (i < BOARD_WIDTH * BRICK_HEIGHT * 2) {
      // First 2 rows of bricks are red.
      m_board.at(i).set_cell_type(GameBoardCell::CellType::BRICK_RED);
    } else if (i < BOARD_WIDTH * BRICK_HEIGHT * 4) {
      // Second 2 rows of bricks are orange
      m_board.at(i).set_cell_type(GameBoardCell::CellType::BRICK_ORANGE);
    } else if (i < BOARD_WIDTH * BRICK_HEIGHT * 6) {
      // Third 2 rows of bricks are green
      m_board.at(i).set_cell_type(GameBoardCell::CellType::BRICK_GREEN);
    } else if (i < BOARD_WIDTH * BRICK_HEIGHT * 8) {
      // Fourth 2 rows of bricks are yellow
      m_board.at(i).set_cell_type(GameBoardCell::CellType::BRICK_YELLOW);
    } else {
      throw std::out_of_range("A bug in the program caused us to go out of range while resetting bricks. This should be reported.");
    }
  }
}

auto GameBoard::coords_to_idx(uint32_t const x, uint32_t const y) -> uint32_t {
  if (x >= BOARD_WIDTH) {
    throw std::out_of_range("X coordinate out of bounds");
  }
  if (y >= BOARD_HEIGHT) {
    throw std::out_of_range("Y coordinate out of bounds");
  }
  return y * BOARD_WIDTH + x;
}

auto GameBoard::idx_to_coords(uint32_t const idx) const -> std::tuple<uint32_t, uint32_t> {
  if (idx >= BOARD_WIDTH * BOARD_HEIGHT) {
    throw std::out_of_range("idx out of bounds");
  }
  uint32_t y = idx / BOARD_WIDTH;
  uint32_t x = idx % BOARD_WIDTH;
  return {x, y};
}

auto GameBoard::move_ball(uint32_t const ball_start_x, uint32_t const ball_start_y) -> void {
  move_ball(coords_to_idx(ball_start_x, ball_start_y));
}

auto GameBoard::move_ball(uint32_t const ball_start_idx) -> void {
  auto set_ball_cells = [this](GameBoardCell::CellType const ct) -> void {
    auto [start_x, start_y] = idx_to_coords(m_ball_start_idx);
    for (uint32_t j = 0; j < BALL_HEIGHT; ++j) {
      for (uint32_t i = 0; i < BALL_WIDTH; ++i) {
        uint32_t const x = start_x + i;
        uint32_t const y = start_y + j;

        if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT) {
          throw std::out_of_range("Ball exceeds board boundaries.");
        }

        m_board.at(coords_to_idx(x, y)).set_cell_type(ct);
      }
    }
  };

  set_ball_cells(GameBoardCell::CellType::EMPTY);
  m_ball_start_idx = ball_start_idx;
  check_ball_collisions();
  set_ball_cells(GameBoardCell::CellType::BALL);
}

auto GameBoard::check_ball_collisions() -> void {
  auto check_cell_collisions = [this](uint32_t const x, uint32_t const y) -> void {
    uint32_t const x_offset = (m_ball_trajectory == BallTrajectory::DownLeft || m_ball_trajectory == BallTrajectory::UpLeft) ? -1 : 1;
    uint32_t const y_offset = (m_ball_trajectory == BallTrajectory::UpLeft || m_ball_trajectory == BallTrajectory::UpRight) ? -1 : 1;

    auto const& cell = m_board.at(coords_to_idx(x + x_offset, y + y_offset));

    switch (cell.get_cell_type()) {
      case GameBoardCell::EMPTY: {
        // Check for collision with walls
        if (cell.has_any_properties({GameBoardCell::ROW_END})) {
          m_ball_trajectory = (m_ball_trajectory == BallTrajectory::DownRight) ? BallTrajectory::DownLeft : BallTrajectory::UpLeft;
        } else if (cell.has_any_properties({GameBoardCell::ROW_START})) {
          m_ball_trajectory = (m_ball_trajectory == BallTrajectory::DownLeft) ? BallTrajectory::DownRight : BallTrajectory::UpRight;
        }

        // Check for collision with ceiling and floor
        if (cell.has_any_properties({GameBoardCell::COL_START})) {
          m_ball_trajectory = (m_ball_trajectory == BallTrajectory::UpRight) ? BallTrajectory::DownRight : BallTrajectory::DownLeft;
        } else if (cell.has_any_properties({GameBoardCell::COL_END})) {
          reset_ball();
          reset_paddle();
        }

        break;
      }
      case GameBoardCell::PADDLE: {
        m_ball_trajectory = (m_ball_trajectory == BallTrajectory::DownRight) ? BallTrajectory::UpRight : BallTrajectory::UpLeft;
        break;
      }
      case GameBoardCell::BRICK_RED: [[fallthrough]];
      case GameBoardCell::BRICK_GREEN: [[fallthrough]];
      case GameBoardCell::BRICK_YELLOW: [[fallthrough]];
      case GameBoardCell::BRICK_ORANGE: {
        remove_brick(x + x_offset, y + y_offset);
        m_ball_trajectory = (m_ball_trajectory == BallTrajectory::UpRight) ? BallTrajectory::DownRight : BallTrajectory::DownLeft;
        break;
      }
      case GameBoardCell::BALL: [[fallthrough]];
      default: break;
    }
  };

  auto [start_x, start_y] = idx_to_coords(m_ball_start_idx);
  for (uint32_t j = 0; j < BALL_HEIGHT; ++j) {
    for (uint32_t i = 0; i < BALL_WIDTH; ++i) {
      uint32_t const x = start_x + i;
      uint32_t const y = start_y + j;
      check_cell_collisions(x, y);
    }
  }
}

auto GameBoard::remove_brick(uint32_t const x, uint32_t const y) -> void {
  if (coords_to_idx(x, y) > m_board.size()) {
    throw std::out_of_range{"remove_brick: x and y are out of range"};
  }

  uint32_t const brick_start_x = (x / BRICK_WIDTH) * BRICK_WIDTH;
  uint32_t const brick_start_y = (y / BRICK_HEIGHT) * BRICK_HEIGHT;

  for (uint32_t dx = 0; dx < BRICK_WIDTH; ++dx) {
    for (uint32_t dy = 0; dy < BRICK_HEIGHT; ++dy) {
      uint32_t const idx = coords_to_idx(brick_start_x + dx, brick_start_y + dy);
      m_board.at(idx).set_cell_type(GameBoardCell::CellType::EMPTY);
    }
  }

  // TODO: Increment game score
}

auto GameBoard::move_paddle(uint32_t const paddle_start_x, uint32_t const paddle_start_y) -> void {
  move_paddle(coords_to_idx(paddle_start_x, paddle_start_y));
}

auto GameBoard::move_paddle(uint32_t const paddle_start_idx) -> void {
  auto set_paddle_cells = [this](GameBoardCell::CellType const ct) -> void {
    auto [start_x, start_y] = idx_to_coords(m_paddle_start_idx);
    for (uint32_t j = 0; j < PADDLE_HEIGHT; ++j) {
      for (uint32_t i = 0; i < PADDLE_WIDTH; ++i) {
        uint32_t const x = start_x + i;
        uint32_t const y = start_y + j;
        m_board.at(coords_to_idx(x, y)).set_cell_type(ct);
      }
    }
  };

  const uint32_t board_size = static_cast<uint32_t>(m_board.size());
  uint32_t corrected_idx = paddle_start_idx;
  if (paddle_start_idx < board_size - (BOARD_WIDTH * PADDLE_HEIGHT)) {
    corrected_idx = board_size - (BOARD_WIDTH * PADDLE_HEIGHT);
  } else if (paddle_start_idx > board_size - (BOARD_WIDTH * PADDLE_HEIGHT) + BOARD_WIDTH - PADDLE_WIDTH) {
    corrected_idx = board_size - (BOARD_WIDTH * PADDLE_HEIGHT) + BOARD_WIDTH - PADDLE_WIDTH;
  }

  set_paddle_cells(GameBoardCell::CellType::EMPTY);
  m_paddle_start_idx = corrected_idx;
  set_paddle_cells(GameBoardCell::CellType::PADDLE);
}

auto GameBoard::reset_ball() -> void {
  uint32_t ball_x = (BOARD_WIDTH - BALL_WIDTH) / 2;
  uint32_t ball_y = (BOARD_HEIGHT - BALL_HEIGHT) / 2;
  move_ball(ball_x, ball_y);
}

auto GameBoard::reset_paddle() -> void {
  uint32_t paddle_x = (BOARD_WIDTH - PADDLE_WIDTH) / 2;
  uint32_t paddle_y = BOARD_HEIGHT - PADDLE_HEIGHT;
  move_paddle(paddle_x, paddle_y);
}

auto GameBoard::is_brick_start(uint32_t const x, uint32_t const y) -> bool {
  return is_brick_start(coords_to_idx(x, y));
}

auto GameBoard::is_brick_start(uint32_t const idx) -> bool {
  return idx >= BRICK_START_IDX && idx <= BRICK_END_IDX && idx % BRICK_WIDTH == 0;
}

auto GameBoard::is_brick_end(uint32_t const x, uint32_t const y) -> bool {
  return is_brick_end(coords_to_idx(x, y));
}

auto GameBoard::is_brick_end(uint32_t const idx) -> bool {
  return idx >= BRICK_START_IDX && idx <= BRICK_END_IDX && (idx % BRICK_WIDTH) == (BRICK_WIDTH - 1);
}

// Is this cell the start of a row?
auto GameBoard::is_row_start(uint32_t idx) -> bool {
  return idx % (BOARD_WIDTH) == 0;
}

// Is this cell the end of a row?
auto GameBoard::is_row_end(uint32_t idx) -> bool {
  return (idx % BOARD_WIDTH) == (BOARD_WIDTH - 1);
}

// Is this cell the start of a column?
auto GameBoard::is_col_start(uint32_t idx) -> bool {
  return (idx / BOARD_WIDTH) == 0;
}

// Is this cell the end of a column?
auto GameBoard::is_col_end(uint32_t idx) -> bool {
  return (idx / BOARD_WIDTH) == (BOARD_HEIGHT - 1);
}

auto GameBoard::for_each_cell(CellFunctor const &f) const -> void {
  for (auto const &cell: m_board){
    f(cell);
  }
}

auto GameBoard::shift_paddle_left(uint32_t const shift) -> void {
  move_paddle(m_paddle_start_idx - shift);
}

auto GameBoard::shift_paddle_right(uint32_t const shift) -> void {
  move_paddle(m_paddle_start_idx + shift);
}

auto GameBoard::shift_ball(uint32_t const shift) -> void {
  auto const [ball_x, ball_y] = idx_to_coords(m_ball_start_idx);
  switch (m_ball_trajectory) {
    case BallTrajectory::DownRight: {
      move_ball(ball_x + shift, ball_y + shift);
      break;
    }
    case BallTrajectory::DownLeft: {
      move_ball(ball_x - shift, ball_y + shift);
      break;
    }
    case BallTrajectory::UpRight: {
      move_ball(ball_x + shift, ball_y - shift);
      break;
    }
    case BallTrajectory::UpLeft: {
      move_ball(ball_x - shift, ball_y - shift);
      break;
    }
    default: {
      throw std::logic_error("A bug in the game logic caused an error moving the ball. This should be reported.");
    }
  }
}

} // namespace breakout::model
