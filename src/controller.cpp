#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "pacman.h"

void Controller::ChangeDirection(Pacman &pacman, Snake::Direction input) const {
  pacman.SetDirection(input);
  return;
}

void Controller::HandleInput(bool &running, Pacman &pacman) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(pacman, Snake::Direction::kUp);
          break;
        case SDLK_DOWN:
          ChangeDirection(pacman, Snake::Direction::kDown);
          break;
        case SDLK_LEFT:
          ChangeDirection(pacman, Snake::Direction::kLeft);
          break;
        case SDLK_RIGHT:
          ChangeDirection(pacman, Snake::Direction::kRight);
          break;
      }
    }
  }
}