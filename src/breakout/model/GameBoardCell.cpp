#include <breakout/model/GameBoardCell.hpp>
#include <initializer_list>

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

auto GameBoardCell::add_properties(std::initializer_list<Property> const &properties) -> void {
  for (auto const &p: properties) {
    m_properties |= p;
  }
}

auto GameBoardCell::has_all_properties(std::initializer_list<Property> const &properties) const -> bool {
  for (auto const &p: properties) {
    if ((m_properties & p) == 0) return false;
  }
  return true;
}

auto GameBoardCell::has_any_properties(std::initializer_list<Property> const &properties) const -> bool {
  for (auto const &p: properties) {
    if ((m_properties & p) == 1) return true;
  }
  return false;
}

} // namespace breakout::model
