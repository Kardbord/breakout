#include <breakout/model/GameState.hpp>

namespace breakout::model {

auto GameStateBase::set_last_event(ftxui::Event e) -> void {
  m_last_event = e;
}

auto GameStateBase::get_last_event() const -> ftxui::Event {
  return m_last_event;
}

} // namespace breakout::model
