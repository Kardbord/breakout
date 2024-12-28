#include <breakout/model/GameState.hpp>

namespace breakout::model {

auto GameStateBase::set_last_event(ftxui::Event e) -> void {
  m_last_event = e;
}

auto GameStateBase::get_last_event() const -> ftxui::Event {
  return m_last_event;
}


auto GameStateActive::for_each_game_board_cell(GameBoard::CellFunctor const &f) const -> void {
  m_board.for_each_cell(f);
}

} // namespace breakout::model
