#ifndef GAME_H
#define GAME_H

#include <random>
#include "controller.h"
#include "renderer.h"
#include "pacman.h"
#include "monster.h"
#include "maze.h"
#include <string>
#include <memory>

class Game {
 public:
  Game(std::string filename);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetGridW() const;
  int GetGridH() const;

 private:
  std::shared_ptr<Maze> _maze;
  std::shared_ptr<Pacman> _pacman;
  std::vector<std::shared_ptr<Monster>> _monsters;
  std::random_device dev;
  std::mt19937 engine;

  int score{0};

  void Update();
};

#endif