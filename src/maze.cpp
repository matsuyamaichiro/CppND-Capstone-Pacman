#include "maze.h"
#include <iostream>

constexpr int w = 28;
constexpr int h = 30;
constexpr char init_maze[h][w] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3,3,3,3,3,3,3,3,3,3,3,3,0,
    0,3,0,0,0,0,3,0,0,0,0,0,3,0,0,3,0,0,0,0,0,3,0,0,0,0,3,0,
    0,4,0,0,0,0,3,0,0,0,0,0,3,0,0,3,0,0,0,0,0,3,0,0,0,0,4,0,
    0,3,0,0,0,0,3,0,0,0,0,0,3,0,0,3,0,0,0,0,0,3,0,0,0,0,3,0,
    0,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3,3,3,3,3,3,3,3,3,3,3,3,0,
    0,3,0,0,0,0,3,0,0,3,0,0,0,0,0,0,0,0,3,0,0,3,0,0,0,0,3,0,
    0,3,0,0,0,0,3,0,0,3,0,0,0,0,0,0,0,0,3,0,0,3,0,0,0,0,3,0,
    0,3,3,3,3,3,3,0,0,3,3,3,3,0,0,3,3,3,3,0,0,3,3,3,3,3,3,0,
    0,0,0,0,0,0,3,0,0,0,0,0,1,0,0,1,0,0,0,0,0,3,0,0,0,0,0,0,
    0,0,0,0,0,0,3,0,0,0,0,0,1,0,0,1,0,0,0,0,0,3,0,0,0,0,0,0,
    0,0,0,0,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,0,0,0,0,
    0,0,0,0,0,0,3,0,0,1,0,0,0,2,2,0,0,0,1,0,0,3,0,0,0,0,0,0,
    0,0,0,0,0,0,3,0,0,1,0,1,1,1,1,1,1,0,1,0,0,3,0,0,0,0,0,0,
    1,1,1,1,1,1,3,1,1,1,0,1,1,1,1,1,1,0,1,1,1,3,1,1,1,1,1,1,
    0,0,0,0,0,0,3,0,0,1,0,1,1,1,1,1,1,0,1,0,0,3,0,0,0,0,0,0,
    0,0,0,0,0,0,3,0,0,1,0,0,0,0,0,0,0,0,1,0,0,3,0,0,0,0,0,0,
    0,0,0,0,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,0,0,0,0,
    0,0,0,0,0,0,3,0,0,1,0,0,0,0,0,0,0,0,1,0,0,3,0,0,0,0,0,0,
    0,0,0,0,0,0,3,0,0,1,0,0,0,0,0,0,0,0,1,0,0,3,0,0,0,0,0,0,
    0,3,3,3,3,3,3,3,3,3,3,3,3,0,0,3,3,3,3,3,3,3,3,3,3,3,3,0,
    0,3,0,0,0,0,3,0,0,0,0,0,3,0,0,3,0,0,0,0,0,3,0,0,0,0,3,0,
    0,3,0,0,0,0,3,0,0,0,0,0,3,0,0,3,0,0,0,0,0,3,0,0,0,0,3,0,
    0,4,3,3,0,0,3,3,3,3,3,3,3,1,5,3,3,3,3,3,3,3,0,0,3,3,4,0,
    0,0,0,3,0,0,3,0,0,3,0,0,0,0,0,0,0,0,3,0,0,3,0,0,3,0,0,0,
    0,0,0,3,0,0,3,0,0,3,0,0,0,0,0,0,0,0,3,0,0,3,0,0,3,0,0,0,
    0,3,3,3,3,3,3,0,0,3,3,3,3,0,0,3,3,3,3,0,0,3,3,3,3,3,3,0,
    0,3,0,0,0,0,0,0,0,0,0,0,3,0,0,3,0,0,0,0,0,0,0,0,0,0,3,0,
    0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

constexpr Maze::PosType type_encode[] = {
    Maze::PosType::kWall,   // 0
    Maze::PosType::kBlank,  // 1
    Maze::PosType::kGate,   // 2
    Maze::PosType::kFood,   // 3
    Maze::PosType::kPowFood // 4
};

constexpr char pacman_spawn_pos_code = 5;

void Maze::generateMaze() {
    _w = w;
    _h = h;
    _maze.resize(h);
    for (int j = 0; j < h; j++) {
        _maze[j].resize(w);
    }
    for (int j = 0; j < h; j ++) {
        for (int i = 0; i < w; i++) {
            int k = init_maze[j][i];
            if (k == pacman_spawn_pos_code) {
                _pacman_spawn_x = i;
                _pacman_spawn_y = j; 
                _maze[j][i] = Maze::PosType::kBlank;
            } else if (k < sizeof(type_encode)/sizeof(Maze::PosType)) {
                _maze[j][i] = type_encode[k];
            } else {
                _maze[j][i] = Maze::PosType::kWall;
            }
            std::cout << k;
        }
        std::cout << std::endl;
    }
}

int Maze::getW() const {
    return _w;
}

int Maze::getH() const {
    return _h;
}

int Maze::getPacmanSpawnX() const {
    return _pacman_spawn_x;
}

int Maze::getPacmanSpawnY() const {
    return _pacman_spawn_y;
}

Maze::PosType Maze::getPosType(int x, int y) const {
    return _maze[y][x];
}

bool Maze::isAvailable(int x, int y, Direction d) const {
    PosType dest_type = PosType::kWall;
    switch (d) {
        case Direction::kDown:
            if (y < _h - 1) {
                dest_type = _maze[y + 1][x];
            } else if (y == _h - 1) {
                dest_type = _maze[0][x];
            }
            break;
        case Direction::kUp:
            if (y > 0) {
                dest_type = _maze[y - 1][x];
            } else if (y == 0) {
                dest_type = _maze[_h - 1][x];
            }
            break;
        case Direction::kLeft:
            if (x > 0) {
                dest_type = _maze[y][x - 1];
            } else if (x == 0) {
                dest_type = _maze[y][_w - 1];
            }
            break;
        case Direction::kRight:
            if (x < _w - 1) {
                dest_type = _maze[y][x + 1];
            } else if (x == _w - 1) {
                dest_type = _maze[y][0];
            }
    }
    switch (dest_type) {
        case PosType::kWall:
        case PosType::kGate:
            return false;
    }
    return true;
}