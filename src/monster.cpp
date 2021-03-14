#include "monster.h"
#include <cmath>

#if 0
std::string GetStr(Snake::Direction d) {
    switch (d) {
        case Snake::Direction::kUp:
            return "kUp";
        case Snake::Direction::kDown:
            return "kDown";
        case Snake::Direction::kLeft:
            return "kLeft";
        case Snake::Direction::kRight:
            return "kRight";
    }
}
#endif

void Monster::SetDirection(const Maze &maze, int pacman_x, int pacman_y) {
    Direction front;
    Direction left_hand;
    Direction right_hand;
    bool target_in_front;
    bool target_on_left_hand;
    bool far_to_left_right;
    switch (_direction) {
        case Direction::kUp:
            front = Direction::kUp;
            left_hand = Direction::kLeft;
            right_hand = Direction::kRight;
            target_in_front = (pacman_x == _x);
            target_on_left_hand = (pacman_x < _x);
            far_to_left_right = (std::abs(pacman_x - _x) > std::abs(pacman_y) - _y);
        break;
        case Direction::kDown:
            front = Direction::kDown;
            left_hand = Direction::kRight;
            right_hand = Direction::kLeft;
            target_in_front = (pacman_x == _x);
            target_on_left_hand = (pacman_x > _x);
            far_to_left_right = (std::abs(pacman_x - _x) > std::abs(pacman_y) - _y);
        break;
        case Direction::kLeft:
            front = Direction::kLeft;
            left_hand = Direction::kDown;
            right_hand = Direction::kUp;
            target_in_front = (pacman_y == _y);
            target_on_left_hand = (pacman_y > _y);
            far_to_left_right = (std::abs(pacman_y - _y) > std::abs(pacman_x) - _x);
        break;
        case Direction::kRight:
            front = Direction::kRight;
            left_hand = Direction::kUp;
            right_hand = Direction::kDown;
            target_in_front = (pacman_y == _y);
            target_on_left_hand = (pacman_y < _y);
            far_to_left_right = (std::abs(pacman_y - _y) > std::abs(pacman_x) - _x);
    }
    if (!IsAvailable(front, maze)) {
        if (IsAvailable(left_hand, maze) && IsAvailable(right_hand, maze)) {
            if (target_on_left_hand) {
                _direction = left_hand;
            } else {
                _direction = right_hand;
            }
        } else if (IsAvailable(left_hand, maze)) {
            _direction = left_hand;
        } else {
            _direction = right_hand;
        }
    } else {
        if (IsAvailable(left_hand, maze) && !(target_in_front) && (target_on_left_hand) && (far_to_left_right)) {
            _direction = left_hand;
        } else if (IsAvailable(right_hand, maze) && !(target_in_front) && !(target_on_left_hand) && (far_to_left_right)) {
            _direction = right_hand;
        }
    }
}

Snake::Color Monster::GetColorFromSpawnType(Maze::SpawnType type) {
    switch (type) {
        case Maze::SpawnType::kBlinky:
            return Snake::Color::kRed;
        case Maze::SpawnType::kPinky:
            return Snake::Color::kPink;
        case Maze::SpawnType::kInky:
            return Snake::Color::kCyan;
        case Maze::SpawnType::kClyde:
            return Snake::Color::kOrange;
    }
    return Snake::Color::kWhite;
}