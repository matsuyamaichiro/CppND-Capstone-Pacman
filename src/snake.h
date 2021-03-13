#ifndef SNAKE_H
#define SNAKE_H

#include "maze.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };
  void Update(const Maze &maze);
  void SetPos(int x, int y);
  float GetX() const;
  float GetY() const;
  void SetDirection(Snake::Direction d);
  void GrowBody();
  bool alive{true};

 private:
  bool Move(Direction d, const Maze &maze);
  Direction _direction{Direction::kLeft};
  Direction _hold_direction{Direction::kLeft};
  float _speed{(float)1/16}; // should be 1/(2^n)
  float _x{0};
  float _y{0};
};

#endif