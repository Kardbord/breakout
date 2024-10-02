#include <breakout/controller/GameController.hpp>
#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
  breakout::controller::GameController controller;

  try {
    controller.run();
  } catch (std::exception const &e) {
    std::cerr << "Error! Encounterted exception: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
