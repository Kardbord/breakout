#ifndef PONG_GAMESTATE_HPP
#define PONG_GAMESTATE_HPP

namespace pong::model {

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

} // namespace pong::model

#endif
