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
        _x(grid_width / 2),
        _y(grid_height / 2) {}

  void Update(const Maze &maze);
  void SetPos(int x, int y);
  float GetX() const;
  float GetY() const;
  void SetDirection(Snake::Direction d);
  void GrowBody();
  bool SnakeCell(int x, int y);
  int size{1};
  bool alive{true};

 private:
  bool Move(Direction d, const Maze &maze);
  Direction _direction{Direction::kLeft};
  Direction _hold_direction{Direction::kLeft};

  bool growing{false};
  int grid_width;
  int grid_height;
  float _speed{(float)1/16}; // should be 1/(2^n)
  float _x;
  float _y;
};

#endif