#include "maze.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

constexpr Maze::PosType kPosTypeEncode[] = {
    Maze::PosType::kWall,   // 0
    Maze::PosType::kBlank,  // 1
    Maze::PosType::kGate,   // 2
    Maze::PosType::kFood,   // 3
    Maze::PosType::kPowFood // 4
};
constexpr int kPosTypeSize{sizeof(kPosTypeEncode) / sizeof(Maze::PosType)};
constexpr int kPosTypeOffset{0};

constexpr Maze::SpawnType kSpawnTypeEncode[] = {
    Maze::SpawnType::kPacman, // 5
    Maze::SpawnType::kBlinky, // 6
    Maze::SpawnType::kPinky,  // 7
    Maze::SpawnType::kInky,   // 8
    Maze::SpawnType::kClyde   // 9
};
constexpr int kSpawnTypeSize{sizeof(kSpawnTypeEncode) / sizeof(Maze::PosType)};
constexpr int kSpawnTypeOffset{kPosTypeSize};

std::vector<std::string> Maze::Split(std::string& line, char delimiter) {
    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;
    while (std::getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

std::vector<std::unique_ptr<int[]>> Maze::ReadTable(std::string filename) {
    std::vector<std::unique_ptr<int[]>> table;
    std::ifstream ifs(filename);
    std::string line;
    if (!ifs.good()) {
        throw std::invalid_argument("Maze csv file cound not open.");
    }
    int j = 0;
    for (j = 0; std::getline(ifs, line) && (j < _h); j++) {
        table.push_back(std::make_unique<int[]>(_w));
        std::vector<std::string> fields = Split(line, ',');
        int i;
        for (i = 0; (i < fields.size()) && (i < _w); i++) {
            table[j][i] = stoi(fields.at(i));
        }
        for (; i < _w; i++) { // to fill no data field
            table[j][i] = 0;
        }
    }
    for (; j < _h; j++) { // to fill no data line
        table.push_back(std::make_unique<int[]>(_w));
        for (int i = 0; i < _w; i++) {
            table[j][i] = 0;
        }
    }
    return table;
}

void Maze::SetMaze(std::vector<std::unique_ptr<int[]>> table) {
    _maze.resize(_h);
    for (int j = 0; j < _h; j++) {
        _maze[j].resize(_w);
    }
    for (int j = 0; j < _h; j ++) {
        for (int i = 0; i < _w; i++) {
            int k = table[j][i];
            if ((k >= kSpawnTypeOffset) && (k < kSpawnTypeOffset + kSpawnTypeSize)) {
                switch (kSpawnTypeEncode[k - kSpawnTypeOffset]) {
                    case SpawnType::kPacman:
                        _pacman_spawn_x = i;
                        _pacman_spawn_y = j;
                        break;
                    default: // monsters
                        _monsters_spawn_x.push_back(i);
                        _monsters_spawn_y.push_back(j);
                        _monsters_spawn_type.push_back(kSpawnTypeEncode[k - kSpawnTypeOffset]);
                }
                _maze[j][i] = Maze::PosType::kBlank;
            } else if ((k >= kPosTypeOffset) && (k < kPosTypeOffset + kPosTypeSize)) {
                switch (kPosTypeEncode[k - kPosTypeOffset]) {
                    case Maze::PosType::kFood:
                    case Maze::PosType::kPowFood:
                        _food_num++;
                }
                _maze[j][i] = kPosTypeEncode[k - kPosTypeOffset];
            } else {
                _maze[j][i] = Maze::PosType::kWall;
            }
        }
    }
}

Maze::Maze(std::string filename) {
    std::vector<std::unique_ptr<int[]>> table = ReadTable(filename);
    SetMaze(std::move(table));
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

int Maze::GetMonstersNum() const {
    return _monsters_spawn_type.size();
}

int Maze::GetMonsterSpawnX(int index) const {
    return _monsters_spawn_x[index];
}

int Maze::GetMonsterSpawnY(int index) const {
    return _monsters_spawn_y[index];
}

Maze::SpawnType Maze::GetMonsterSpawnType(int index) const {
    return _monsters_spawn_type[index];
}

Maze::PosType Maze::getPosType(int x, int y) const {
    if (x < 0 || x >= _w || y < 0 || y >= _h) {
        return Maze::PosType::kWall;
    }
    return _maze[y][x];
}

bool Maze::isAvailable(int x, int y, Direction d) const {
    PosType dest_type = PosType::kWall;
    if (x < 0 || x >= _w || y < 0 || y >= _h) {
        return false;
    }
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

void Maze::clearFood(int x, int y) {
    if (x < 0 || x >= _w || y < 0 || y >= _h) {
        return;
    }
    switch (getPosType(x, y)) {
        case Maze::PosType::kFood:
        case Maze::PosType::kPowFood:
            _maze[y][x] = Maze::PosType::kBlank;
            if (_food_num > 0) {
                _food_num--;
            }
    }
}

int Maze::GetFoodNum() const {
    return _food_num;
}

void Maze::GrowWall() {
    _growing = true;
}

bool Maze::IsGrowing() const {
    return _growing;
}