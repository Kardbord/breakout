cmake_minimum_required(VERSION 3.28)
project(breakout)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# -------------------------------
# Source files
# -------------------------------
include_directories(src)
set("${CMAKE_PROJECT_NAME}_SRC"
    src/main.cpp
    src/breakout/model/GameState.cpp
    src/breakout/model/GameBoard.cpp
    src/breakout/model/GameBoardCell.cpp
    src/breakout/view/GameView.cpp
    src/breakout/controller/GameController.cpp)

# -------------------------------
# Add executable
# -------------------------------
add_executable(${CMAKE_PROJECT_NAME} ${${CMAKE_PROJECT_NAME}_SRC})

# -------------------------------
# Fetch FTXUI
# -------------------------------
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
set(FTXUI_ENABLE_INSTALL OFF CACHE BOOL "Disable install target for FTXUI library")
set(FTXUI_QUIET ON CACHE BOOL "Quiet FTXUI library")

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG 1d40687a403b7babdafdd577d10e001840b0f124
)
FetchContent_MakeAvailable(ftxui)

find_package(Threads REQUIRED)
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE Threads::Threads
)

# -------------------------------
# Compiler-specific options
# -------------------------------
if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC /W4 /WX)
elseif(EMSCRIPTEN)
    message(STATUS "Building for Emscripten/WebAssembly")

    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE "-pthread")
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
        -sWASM=1
        -sASSERTIONS=2
        -sALLOW_MEMORY_GROWTH=1
        -sEXIT_RUNTIME=0
        -sUSE_PTHREADS=1
        -sASYNCIFY=1
        -pthread
        -sPROXY_TO_PTHREAD=1
        -sSTACK_SIZE=1MB
        -sEXPORTED_FUNCTIONS=[_main,_ftxui_on_resize,ccall,cwrap]
    )
    target_link_options(component PUBLIC "SHELL: -s ALLOW_MEMORY_GROWTH=1")
    foreach(file
        "index.css"
        "index.html"
        "index.mjs"
        "sw.js")
        configure_file(${file} ${file})
    endforeach()
else()
    target_compile_options(
        ${CMAKE_PROJECT_NAME}
        PUBLIC -Wall
               -Wextra
               -Wpedantic
               -Werror
               $<$<CONFIG:RELEASE>:-O3>
               $<$<CONFIG:RELEASE>:-ffast-math>
               $<$<CONFIG:DEBUG>:-O0>
               $<$<CONFIG:DEBUG>:-ggdb3>
    )
endif()

