#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include "maze.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update(const Maze &maze);

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kLeft;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;

 private:
  void UpdateHead(const Maze &maze);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif