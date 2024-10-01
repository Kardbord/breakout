#ifndef BREAKOUT_GAMESTATE_HPP
#define BREAKOUT_GAMESTATE_HPP

namespace breakout::model {

class GameState {
public:
  GameState() = default;
  ~GameState() = default;

  GameState(const GameState&) = delete;
  GameState& operator=(const GameState&) = delete;
  GameState(GameState&&) = delete;
  GameState& operator=(GameState&&) = delete;

protected:
};

} // namespace breakout::model

#endif
