#ifndef PACMAN_H
#define PACMAN_H

#include "snake.h"

class Pacman : public Snake {
    public:
        void SetDirection(Snake::Direction d);
    private:
};

#endif