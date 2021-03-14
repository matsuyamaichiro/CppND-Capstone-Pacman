#include "snake.h"
#include <cmath>

void Snake::Update(const Maze &maze) {
  if (IsAvailable(_direction, maze)) {
    ForceMove(_direction, maze.getW(), maze.getH());
    _hold_direction = _direction;
  } else {
    if (IsAvailable(_hold_direction, maze)) {
      ForceMove(_hold_direction, maze.getW(), maze.getH());
    }
  }
}

void Snake::SetPos(int x, int y) {
  _x = x;
  _y = y;
}

float Snake::GetX() const {
  return _x;
}

float Snake::GetY() const {
  return _y;
}

Snake::Color Snake::GetColor() const {
  return _color;
}

bool Snake::IsAvailable(Direction d, const Maze &maze) {
  //
  // sample case figure (can not move up)
  // (#: Wall, o: Packman, x: Origin of Pacman)
  //   0        1       2       
  // 4 #########        ########
  //   #########        ########
  // 5           xoooooo        
  //             oooooooo       
  //             oooooooo       
  //              oooooo        
  // 6 #########################
  //   #########################
  // (_speed = 0.125)
  // (_x = 1.125, x0 = 0, x1 = 1)
  // (_y = 5.000, y0 = 5, y1 = 5)
  // [0, 5] and [1, 5] will be checked
  //
  int x0 = static_cast<int>(_x);
  int x1 = static_cast<int>(_x - _speed + 1.0);
  int y0 = static_cast<int>(_y);
  int y1 = static_cast<int>(_y - _speed + 1.0);
  if (x1 >= maze.getW()) {
    x1 = 0;
  }
  if (y1 >= maze.getH()) {
    y1 = 0;
  }
  switch (d) {
    case Direction::kUp:
      if ((maze.isAvailable(x1, y1, Maze::Direction::kUp)) && (maze.isAvailable(x0, y1, Maze::Direction::kUp))) {
        return true;
      }
      break;
    case Direction::kDown:
      if ((maze.isAvailable(x1, y0, Maze::Direction::kDown)) && (maze.isAvailable(x0, y0, Maze::Direction::kDown))) {
        return true;
      }
      break;
    case Direction::kLeft:
      if ((maze.isAvailable(x1, y1, Maze::Direction::kLeft)) && (maze.isAvailable(x1, y0, Maze::Direction::kLeft))) {
        return true;
      }
      break;
    case Direction::kRight:
      if ((maze.isAvailable(x0, y1, Maze::Direction::kRight)) && (maze.isAvailable(x0, y0, Maze::Direction::kRight))) {
        return true;
      }
      break;
  }
  return false;
}

void Snake::ForceMove(Direction d, int w, int h) {
  switch (d) {
    case Direction::kUp:
      _y -= _speed;
      break;
    case Direction::kDown:
      _y += _speed;
      break;
    case Direction::kLeft:
      _x -= _speed;
      break;
    case Direction::kRight:
      _x += _speed;
      break;
  }
  // Wrap the Snake around to the beginning if going off of the screen.
  _x = fmod(_x + w, w);
  _y = fmod(_y + h, h);
}