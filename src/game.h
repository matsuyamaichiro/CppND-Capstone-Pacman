#ifndef GAME_H
#define GAME_H

#include <random>
#include "controller.h"
#include "renderer.h"
#include "pacman.h"
#include "monster.h"
#include "maze.h"
#include <string>

class Game {
 public:
  Game(std::string filename);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetGridW() const;
  int GetGridH() const;

 private:
  Maze _maze;
  Pacman _pacman;
  std::vector<Monster> _monsters;
  std::random_device dev;
  std::mt19937 engine;

  int score{0};

  void Update();
};

#endif