#ifndef SNAKE_H
#define SNAKE_H

#include "maze.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };
  enum class Color {kYellow, kRed, kOrange, kPink, kCyan, kWhite};
  Snake(Color c) : _color(c) {};
  void Update(const Maze &maze);
  void SetPos(int x, int y);
  float GetX() const;
  float GetY() const;
  Color GetColor() const;
  bool alive{true};
  static constexpr float speed{(float)1/4}; // should be 1/(2^n)
 protected:
  bool IsAvailable(Direction d, const Maze &maze);
  Direction _direction{Direction::kLeft};
  float _x{0};
  float _y{0};
 private:
  void ForceMove(Direction d, int w, int h);
  Direction _hold_direction{Direction::kLeft};
  Color _color{Color::kYellow};
};

#endif