#ifndef BREAKOUT_GAMEVIEW_HPP
#define BREAKOUT_GAMEVIEW_HPP

#include <breakout/model/GameState.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/task.hpp>
#include <functional>

namespace breakout::view {

class GameView {
public:
  GameView() = default;
  ~GameView() = default;

  GameView(const GameView&) = delete;
  GameView& operator=(const GameView&) = delete;
  GameView(GameView&&) = delete;
  GameView& operator=(GameView&&) = delete;

  auto exit_main_loop() -> void;

  auto update_screen_renderer(ftxui::Component) -> void;

  auto render_pause_menu(model::GameStatePauseMenu const&)   -> void;
  auto render_game_starting(model::GameStateStarting const&) -> void;
  auto render_game_active(model::GameStateActive const&)     -> void;

  using EventHandler = std::function<void()>;

  class MainMenuHandlers {
  public:
    MainMenuHandlers() = delete;
    MainMenuHandlers(EventHandler const &handle_play, EventHandler const &handle_quit) :
      m_play_handler{handle_play}, m_quit_handler{handle_quit} {}

    ~MainMenuHandlers() = default;
    MainMenuHandlers(const MainMenuHandlers&) = default;
    MainMenuHandlers& operator=(const MainMenuHandlers&) = default;
    MainMenuHandlers(MainMenuHandlers&&) = default;
    MainMenuHandlers& operator=(MainMenuHandlers&&) = default;

    inline auto handle_play_button() const -> void { m_play_handler(); }
    inline auto handle_quit_button() const -> void { m_quit_handler(); }

  private:
    EventHandler m_play_handler;
    EventHandler m_quit_handler;
  };

  auto render_main_menu(model::GameStateMainMenu const&, MainMenuHandlers const&) -> void;

private:
  ftxui::ScreenInteractive m_screen{ftxui::ScreenInteractive::Fullscreen()};
};

} // namespace breakout::view

#endif

