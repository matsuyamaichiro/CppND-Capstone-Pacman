#ifndef MAZE_H
#define MAZE_H

#include <vector>

class Maze {
    public:
        enum class PosType { kWall, kBlank, kGate, kFood, kPowFood};
        enum class Direction {kUp, kDown, kLeft, kRight};
        void generateMaze();
        int getW() const;
        int getH() const;
        int getPacmanSpawnX() const;
        int getPacmanSpawnY() const;
        PosType getPosType(int x, int y) const;
        bool isAvailable(int x, int y, Direction d) const;

    private:
        int _w;
        int _h;
        int _pacman_spawn_x;
        int _pacman_spawn_y;
        std::vector<std::vector<PosType>> _maze;
};

#endif