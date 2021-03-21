#include "monster.h"

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
    //
    // in_front o <-@
    //
    bool target_in_front;
    //
    // front <-@ ----------
    //        o on_left_hand
    bool target_on_left_hand;
    //           / (right 135 deg)       \ (right 45 deg)    
    //          /                         \  o on_right_back 
    // front <-@                   front <-@                 
    //        /  o on_left_back             \                
    //       / (left 45 deg)                 \ (left 135 deg)
    bool target_on_left_back;
    bool target_on_right_back;
    std::lock_guard<std::mutex> lock(_mutex);
    switch (_direction) {
        case Direction::kUp:
            front = Direction::kUp;
            left_hand = Direction::kLeft;
            right_hand = Direction::kRight;
            target_in_front = (pacman_x == _x) && (pacman_y < _y);
            target_on_left_hand = (pacman_x < _x);
            target_on_left_back = (pacman_x - _x) < (pacman_y - _y);
            target_on_right_back = (pacman_x - _x) > -(pacman_y - _y);
        break;
        case Direction::kDown:
            front = Direction::kDown;
            left_hand = Direction::kRight;
            right_hand = Direction::kLeft;
            target_in_front = (pacman_x == _x) && (pacman_y > _y);
            target_on_left_hand = (pacman_x > _x);
            target_on_left_back = (pacman_x - _x) > (pacman_y - _y);
            target_on_right_back = (pacman_x - _x) < -(pacman_y - _y);
        break;
        case Direction::kLeft:
            front = Direction::kLeft;
            left_hand = Direction::kDown;
            right_hand = Direction::kUp;
            target_in_front = (pacman_y == _y) && (pacman_x < _x);
            target_on_left_hand = (pacman_y > _y);
            target_on_left_back = (pacman_x - _x) > -(pacman_y - _y);
            target_on_right_back = (pacman_x - _x) > (pacman_y - _y);
        break;
        case Direction::kRight:
            front = Direction::kRight;
            left_hand = Direction::kUp;
            right_hand = Direction::kDown;
            target_in_front = (pacman_y == _y) && (pacman_x > _x);
            target_on_left_hand = (pacman_y < _y);
            target_on_left_back = (pacman_x - _x) < -(pacman_y - _y);
            target_on_right_back = (pacman_x - _x) < (pacman_y - _y);
    }
    lock.~lock_guard();
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
        if (IsAvailable(left_hand, maze) && !(target_in_front) && (target_on_left_hand) && (target_on_left_back)) {
            _direction = left_hand;
        } else if (IsAvailable(right_hand, maze) && !(target_in_front) && !(target_on_left_hand) && (target_on_right_back)) {
            _direction = right_hand;
        } else if (IsAvailable(left_hand, maze) && (target_on_left_back)) {
            _direction = left_hand;
        } else if (IsAvailable(right_hand, maze) && (target_on_right_back)) {
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