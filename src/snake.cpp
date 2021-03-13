#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update(const Maze &maze) {
  if (Move(_direction, maze)) {
    _hold_direction = _direction;
  } else {
    Move(_hold_direction, maze);
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

void Snake::SetDirection(Snake::Direction d) {
  _direction = d;
}

bool Snake::Move(Direction d, const Maze &maze) {
  bool wasMoved = false;
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
        _y -= _speed;
        wasMoved = true;
      }
      break;
    case Direction::kDown:
      if ((maze.isAvailable(x1, y0, Maze::Direction::kDown)) && (maze.isAvailable(x0, y0, Maze::Direction::kDown))) {
        _y += _speed;
        wasMoved = true;
      }
      break;
    case Direction::kLeft:
      if ((maze.isAvailable(x1, y1, Maze::Direction::kLeft)) && (maze.isAvailable(x1, y0, Maze::Direction::kLeft))) {
        _x -= _speed;
        wasMoved = true;
      }
      break;
    case Direction::kRight:
      if ((maze.isAvailable(x0, y1, Maze::Direction::kRight)) && (maze.isAvailable(x0, y0, Maze::Direction::kRight))) {
        _x += _speed;
        wasMoved = true;
      }
      break;
  }
  if (wasMoved == true) {
    // Wrap the Snake around to the beginning if going off of the screen.
    _x = fmod(_x + grid_width, grid_width);
    _y = fmod(_y + grid_height, grid_height);
  }
  return wasMoved;
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(_x) && y == static_cast<int>(_y)) {
    return true;
  }
  return false;
}