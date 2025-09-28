#include <breakout/model/GameBoardCell.hpp>

namespace breakout::model {

GameBoardCell::GameBoardCell(CellType const ct): m_type(ct) {}

auto GameBoardCell::set_cell_type(CellType const ct) -> void {
  m_type = ct;
}

auto GameBoardCell::get_cell_type() const -> CellType {
  return m_type;
}

auto GameBoardCell::get_cell_value() const -> uint32_t {
  // Point values from https://en.wikipedia.org/wiki/Breakout_(video_game)
  switch (m_type) {
    case BRICK_RED:    return 7;
    case BRICK_ORANGE: return 5;
    case BRICK_GREEN:  return 3;
    case BRICK_YELLOW: return 1;
    default:           return 0;
  }
}

} // namespace breakout::model
