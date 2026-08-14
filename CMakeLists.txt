cmake_minimum_required(VERSION 3.26)
file(READ "VERSION" PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
project(breakout VERSION ${PROJECT_VERSION} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
# Dependencies
# -------------------------------
set(FTXUI_ENABLE_INSTALL OFF CACHE BOOL "Disable install target for FTXUI library")
set(FTXUI_QUIET ON CACHE BOOL "Quiet FTXUI library")
add_subdirectory(external)


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

# -------------------------------
# Install
# -------------------------------
if(NOT EMSCRIPTEN)
    include(GNUInstallDirs)
    install(TARGETS ${CMAKE_PROJECT_NAME}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES LICENSE
            DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif()

