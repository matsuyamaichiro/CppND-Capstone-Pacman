#ifndef MONSTER_H
#define MONSTER_H

#include "snake.h"

class Monster : public Snake {
    public:
    Monster(Snake::Color c) : Snake(c) {};
    using Snake::SetDirection;
    void SetDirection(const Maze &maze, int pacman_x, int pacman_y);
    static Snake::Color GetColorFromSpawnType(Maze::SpawnType type);

    private:
};

#endif