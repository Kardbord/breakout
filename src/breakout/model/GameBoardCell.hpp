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

  enum Property : uint32_t {
    IS_BRICK_START = 1 << 0,
    IS_BRICK_END   = 1 << 1,
    IS_ROW_START   = 1 << 2,
    IS_ROW_END     = 1 << 3,
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

  // Returns true if a cell possesses all of the provided properties
  auto has_property(std::initializer_list<Property> const &properties) const -> bool;

private:

  CellType m_type;

  uint32_t m_properties{0};
};

} // namespace breakout::model

#endif
