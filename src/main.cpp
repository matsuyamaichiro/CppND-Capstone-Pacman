#include <iostream>
#include <exception>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argc, char *argv[]) {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr int kGridSize{20};

  std::string filename;
  if (argc >= 2) {
    filename = std::move(std::string(argv[1]));
  } else {
    filename = "../src/maze.csv";
    std::cerr << "trying to open " << filename << std::endl;
  }
  try {
    Game game(filename);
    std::size_t grid_width = game.GetGridW();
    std::size_t grid_height = game.GetGridH();
    std::size_t screen_width = grid_width * kGridSize;
    std::size_t screen_height = grid_height * kGridSize;
    Renderer renderer(screen_width, screen_height, grid_width, grid_height);
    Controller controller;
    game.Run(controller, renderer, kMsPerFrame);
    std::cout << "Game has terminated successfully!\n";
    std::cout << "Score: " << game.GetScore() << "\n";
  } catch (std::invalid_argument e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 0;
  } catch (...) {
    std::cerr << "Unknown error" << std::endl;
  }
  return 0;
}