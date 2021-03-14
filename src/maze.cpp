#include "maze.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

constexpr Maze::PosType type_encode[] = {
    Maze::PosType::kWall,   // 0
    Maze::PosType::kBlank,  // 1
    Maze::PosType::kGate,   // 2
    Maze::PosType::kFood,   // 3
    Maze::PosType::kPowFood // 4
};

constexpr char pacman_spawn_pos_code = 5;
constexpr char monster_spawn_pos_code = 6;

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
            if (k == pacman_spawn_pos_code) {
                _pacman_spawn_x = i;
                _pacman_spawn_y = j; 
                _maze[j][i] = Maze::PosType::kBlank;
            } else if (k == monster_spawn_pos_code) {
                _monster_spawn_x = i;
                _monster_spawn_y = j; 
                _maze[j][i] = Maze::PosType::kBlank;
            } else if (k < sizeof(type_encode)/sizeof(Maze::PosType)) {
                _maze[j][i] = type_encode[k];
                switch (_maze[j][i]) {
                    case Maze::PosType::kFood:
                    case Maze::PosType::kPowFood:
                        _food_num++;
                }
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

int Maze::getMonsterSpawnX() const {
    return _monster_spawn_x;
}

int Maze::getMonsterSpawnY() const {
    return _monster_spawn_y;
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