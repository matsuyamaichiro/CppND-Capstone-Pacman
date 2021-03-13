#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "pacman.h"

class Controller {
 public:
  void HandleInput(bool &running, Pacman &pacman) const;

 private:
  void ChangeDirection(Pacman &pacman, Snake::Direction input) const;
};

#endif