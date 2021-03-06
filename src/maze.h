#ifndef MAZE_H
#define MAZE_H

#include <vector>

class Maze {
    public:
        enum class PosType { kWall, kBlank, kGate, kFood, kPowFood};
        void generateMaze();
        int getW() const;
        int getH() const;
        PosType getPosType(int x, int y) const;
    private:
        int _w;
        int _h;
        std::vector<std::vector<PosType>> _maze;
};

#endif