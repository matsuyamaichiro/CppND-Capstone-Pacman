#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <memory>

class Maze {
    public:
        enum class PosType { kWall, kBlank, kGate, kFood, kPowFood};
        enum class Direction {kUp, kDown, kLeft, kRight};
        Maze(std::string filename);
        int getW() const;
        int getH() const;
        int getPacmanSpawnX() const;
        int getPacmanSpawnY() const;
        PosType getPosType(int x, int y) const;
        bool isAvailable(int x, int y, Direction d) const;
        void clearFood(int x, int y);
        int GetFoodNum() const;
        void GrowWall();
        bool IsGrowing() const;
    protected:
        std::vector<std::string> Split(std::string& line, char delimiter);
        std::vector<std::unique_ptr<int[]>> ReadTable(std::string filename);
        void SetMaze(std::vector<std::unique_ptr<int[]>> table);
    private:
        int _w{28};
        int _h{30};
        int _pacman_spawn_x;
        int _pacman_spawn_y;
        std::vector<std::vector<PosType>> _maze;
        int _food_num{0};
        bool _growing{false};
};

#endif