#include "pacman.h"

void Pacman::SetDirection(Snake::Direction d) {
  _direction = d;
}

void Pacman::Eaten() {
  _alive = false;
  _color = Snake::Color::kGray;
}

bool Pacman::IsAlive() const {
  return _alive;
}