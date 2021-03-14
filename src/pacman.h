#ifndef PACMAN_H
#define PACMAN_H

#include "snake.h"

class Pacman : public Snake {
    public:
        Pacman(Snake::Color c) : Snake(c) {};
        void SetDirection(Snake::Direction d);
    private:
};

#endif