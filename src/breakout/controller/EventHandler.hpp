#ifndef BREAKOUT_EVENTHANDLER_HPP
#define BREAKOUT_EVENTHANDLER_HPP

#include <ftxui/component/event.hpp>
#include <functional>

namespace breakout::controller {

  // The view and model use this to pass events to the controller
  using EventHandler = std::function<bool(ftxui::Event)>;

} // namespace breakout::controller

#endif // BREAKOUT_EVENTHANDLER_HPP
