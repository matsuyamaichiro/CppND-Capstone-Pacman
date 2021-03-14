#ifndef MONSTER_H
#define MONSTER_H

#include "snake.h"

class Monster : public Snake {
    public:
    Monster(Snake::Color c) : Snake(c) {};
    void SetDirection(const Maze &maze, int pacman_x, int pacman_y);

    private:
};

#endif