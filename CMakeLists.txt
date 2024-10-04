cmake_minimum_required(VERSION 3.28)
project(breakout)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(src)
set("${CMAKE_PROJECT_NAME}_SRC"
  src/main.cpp
  src/breakout/model/GameState.cpp
  src/breakout/view/GameView.cpp
  src/breakout/controller/GameController.cpp
)
add_executable(${CMAKE_PROJECT_NAME} ${${CMAKE_PROJECT_NAME}_SRC})

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
set(FTXUI_ENABLE_INSTALL OFF CACHE BOOL "Disable install target for FTXUI library")
set(FTXUI_QUIET ON CACHE BOOL "Quiet FTXUI library")
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG 1d40687a403b7babdafdd577d10e001840b0f124
)
FetchContent_MakeAvailable(ftxui)
 
target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

if(MSVC)
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC /W4 /WX)
else()
  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    -Wall -Wextra -Wpedantic -Werror
    $<$<CONFIG:RELEASE>:-Ofast>
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:DEBUG>:-ggdb3>
  )
endif()

