#ifndef SNAKE_H
#define SNAKE_H

#include "maze.h"
#include <mutex>

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };
  enum class Color {kYellow, kRed, kOrange, kPink, kCyan, kWhite, kGray, kBlue};
  Snake(Color c) : _color(c) {};
  void Update(const Maze &maze);
  void SetPos(int x, int y);
  void GetPos(float &x, float &y);
  Color GetColor() const;
  static constexpr float speed{(float)1/4}; // should be 1/(2^n)
 protected:
  bool IsAvailable(Direction d, const Maze &maze);
  Direction _direction{Direction::kLeft}; // thread safe
  float _x{0}; // nonatomic position param
  float _y{0}; // nonatomic position param
  Color _color{Color::kYellow}; // thread safe
  bool _alive{true};
  std::mutex _mutex;
 private:
  void ForceMove(Direction d, int w, int h);
  Direction _hold_direction{Direction::kLeft};
};

#endif