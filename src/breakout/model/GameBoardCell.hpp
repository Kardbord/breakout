#ifndef BREAKOUT_GAMEBOARD_CELL_HPP
#define BREAKOUT_GAMEBOARD_CELL_HPP

#include <cinttypes>
#include <initializer_list>

namespace breakout::model {

class GameBoardCell {
public:
  enum CellType {
    EMPTY,
    BRICK_RED,
    BRICK_ORANGE,
    BRICK_GREEN,
    BRICK_YELLOW,
    BALL,
    PADDLE,
  };

  // TODO: Remove these, they are not necessary
  enum Property : uint32_t {
    BRICK_START = 1 << 0,
    BRICK_END   = 1 << 1,
    ROW_START   = 1 << 2,
    ROW_END     = 1 << 3,
    COL_START   = 1 << 4,
    COL_END     = 1 << 5,
  };

  GameBoardCell() = default;
  ~GameBoardCell() = default;

  GameBoardCell(const GameBoardCell&) = default;
  GameBoardCell& operator=(const GameBoardCell&) = default;
  GameBoardCell(GameBoardCell&&) = default;
  GameBoardCell& operator=(GameBoardCell&&) = default;

  GameBoardCell(CellType);

  auto set_cell_type(CellType) -> void;
  auto get_cell_type() const -> CellType;

  auto get_cell_value() const -> uint32_t;

  // Add the provided properties to the cell
  auto add_properties(std::initializer_list<Property> const &properties) -> void;

  // Returns true if a cell possesses all of the provided properties
  auto has_all_properties(std::initializer_list<Property> const &properties) const -> bool;

  // Returns true if a cell possesses any of the provided properties
  auto has_any_properties(std::initializer_list<Property> const &properties) const -> bool;

  // Returns the integer representation of all properties applicable to the cell.
  auto cell_properties_value() const -> uint32_t;
private:

  CellType m_type{EMPTY};

  uint32_t m_properties{0};
};

} // namespace breakout::model

#endif
