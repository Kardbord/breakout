#ifndef BREAKOUT_GAMEBOARD_CELL_HPP
#define BREAKOUT_GAMEBOARD_CELL_HPP

#include <cinttypes>

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

private:

  CellType m_type;
};

} // namespace breakout::model

#endif
