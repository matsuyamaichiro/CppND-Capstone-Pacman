#include <gtest/gtest.h>

#include "../src/maze.h"
#include "../src/snake.h"

TEST(Maze, Whole) {
    Maze maze;
    // based on current maze
    maze.generateMaze();
    EXPECT_EQ(maze.getH(), 30);
    EXPECT_EQ(maze.getW(), 28);
    // test pacman spawn position
    EXPECT_EQ(maze.getPacmanSpawnX(), 14);
    EXPECT_EQ(maze.getPacmanSpawnY(), 23);
    EXPECT_EQ(maze.getPosType(14, 23), Maze::PosType::kBlank);
    // test around top-right corner
    EXPECT_EQ(maze.getPosType(26, 0), Maze::PosType::kWall);
    EXPECT_EQ(maze.getPosType(26, 1), Maze::PosType::kFood);
    EXPECT_EQ(maze.getPosType(26, 2), Maze::PosType::kFood);
    EXPECT_EQ(maze.getPosType(26, 3), Maze::PosType::kPowFood);
    EXPECT_EQ(maze.getPosType(25, 1), Maze::PosType::kFood);
    EXPECT_EQ(maze.getPosType(27, 1), Maze::PosType::kWall);
    EXPECT_FALSE(maze.isAvailable(26, 1, Maze::Direction::kUp));
    EXPECT_TRUE(maze.isAvailable(26, 1, Maze::Direction::kDown));
    EXPECT_TRUE(maze.isAvailable(26, 1, Maze::Direction::kLeft));
    EXPECT_FALSE(maze.isAvailable(26, 1, Maze::Direction::kRight));
    // test around monster gate
    EXPECT_EQ(maze.getPosType(14, 11), Maze::PosType::kBlank);
    EXPECT_EQ(maze.getPosType(14, 12), Maze::PosType::kGate);
    EXPECT_FALSE(maze.isAvailable(14, 11, Maze::Direction::kUp));
    EXPECT_FALSE(maze.isAvailable(14, 11, Maze::Direction::kDown));
    EXPECT_TRUE(maze.isAvailable(14, 11, Maze::Direction::kLeft));
    EXPECT_TRUE(maze.isAvailable(14, 11, Maze::Direction::kRight));
    // test around warp zone
    EXPECT_EQ(maze.getPosType(0, 14), Maze::PosType::kBlank);
    EXPECT_FALSE(maze.isAvailable(0, 14, Maze::Direction::kUp));
    EXPECT_FALSE(maze.isAvailable(0, 14, Maze::Direction::kDown));
    EXPECT_TRUE(maze.isAvailable(0, 14, Maze::Direction::kLeft));
    EXPECT_TRUE(maze.isAvailable(0, 14, Maze::Direction::kRight));
    EXPECT_EQ(maze.getPosType(27, 14), Maze::PosType::kBlank);
    EXPECT_FALSE(maze.isAvailable(27, 14, Maze::Direction::kUp));
    EXPECT_FALSE(maze.isAvailable(27, 14, Maze::Direction::kDown));
    EXPECT_TRUE(maze.isAvailable(27, 14, Maze::Direction::kLeft));
    EXPECT_TRUE(maze.isAvailable(27, 14, Maze::Direction::kRight));
    // test clear food
    EXPECT_EQ(maze.getPosType(26, 1), Maze::PosType::kFood);
    maze.clearFood(26, 1);
    EXPECT_EQ(maze.getPosType(26, 1), Maze::PosType::kBlank);
    EXPECT_EQ(maze.getPosType(26, 3), Maze::PosType::kPowFood);
    maze.clearFood(26, 3);
    EXPECT_EQ(maze.getPosType(26, 3), Maze::PosType::kBlank);
    EXPECT_EQ(maze.getPosType(26, 0), Maze::PosType::kWall);
    maze.clearFood(26, 0);
    EXPECT_EQ(maze.getPosType(26, 0), Maze::PosType::kWall);
    EXPECT_EQ(maze.getPosType(14, 11), Maze::PosType::kBlank);
    maze.clearFood(14, 11);
    EXPECT_EQ(maze.getPosType(14, 11), Maze::PosType::kBlank);
    EXPECT_EQ(maze.getPosType(14, 12), Maze::PosType::kGate);
    maze.clearFood(14, 12);
    EXPECT_EQ(maze.getPosType(14, 12), Maze::PosType::kGate);
}

TEST(Snake, Whole) {
    constexpr std::size_t kGridWidth{28};
    constexpr std::size_t kGridHeight{30};
    Maze maze;
    // based on current maze
    maze.generateMaze();
    Snake snake(kGridWidth, kGridHeight);
    // initial position
    //    14
    // 23  o
    snake.SetPos(14, 23);
    snake.direction = Snake::Direction::kLeft;
    EXPECT_FLOAT_EQ(snake.GetX(), 14.0);
    EXPECT_FLOAT_EQ(snake.GetY(), 23.0);
    // position after 1 tick
    //    13  14
    // 23     oo
    snake.Update(maze);
    EXPECT_FLOAT_EQ(snake.GetX(), 14.0 - 1.0/16);
    EXPECT_FLOAT_EQ(snake.GetY(), 23.0);
    // position after moving 1 grid
    //    13  14
    // 23  o<--o
    for (int i = 0; i < 15; i++) {
        snake.Update(maze);
    }
    EXPECT_FLOAT_EQ(snake.GetX(), 13.0);
    EXPECT_FLOAT_EQ(snake.GetY(), 23.0);
    // position after touching wall
    //     5 6      13
    // 22  0
    // 23  0 o<------o
    // 24  0
    for (int i = 0; i < 16 * 7; i++) {
        snake.Update(maze);
    }
    EXPECT_FLOAT_EQ(6.0, snake.GetX());
    EXPECT_FLOAT_EQ(23.0, snake.GetY());
    // position not moved
    //     5 6
    // 22  0
    // 23  0 o
    // 24  0
    for (int i = 0; i < 8; i++) {
        snake.Update(maze);
    }
    EXPECT_FLOAT_EQ(6.0, snake.GetX());
    EXPECT_FLOAT_EQ(23.0, snake.GetY());
    // position after intersection with holding direction
    //     6 7 8
    // 20  --->o
    // 21  | 0 0 
    // 22  | 0
    // 23  o 0
    snake.direction = Snake::Direction::kUp;
    snake.Update(maze);
    snake.direction = Snake::Direction::kRight;
    for (int i = 0; i < 15 + 16 * 4; i++) {
        snake.Update(maze);
    }
    EXPECT_FLOAT_EQ(8.0, snake.GetX());
    EXPECT_FLOAT_EQ(20.0, snake.GetY());
    // position after trying to enter gate
    //    13        18
    // 11  o-------->| 0
    // 12  2 2 0 0 0 | 0
    // 13          0 o 0
    snake.SetPos(13, 11);
    snake.direction = Snake::Direction::kRight;
    snake.Update(maze);
    snake.direction = Snake::Direction::kDown;
    for (int i = 0; i < 15 + 16 * 6; i++) {
        snake.Update(maze);
    }
    EXPECT_FLOAT_EQ(18.0, snake.GetX());
    EXPECT_FLOAT_EQ(13.0, snake.GetY());
    // position after warping from right
    //       0       27
    // 14 -->o        o->
    snake.SetPos(27, 14);
    snake.direction = Snake::Direction::kRight;
    for (int i = 0; i < 16; i++) {
        snake.Update(maze);
    }
    EXPECT_FLOAT_EQ(0.0, snake.GetX());
    EXPECT_FLOAT_EQ(14.0, snake.GetY());
    // position after warping from left
    //       0       27
    // 14 <--o        o<-
    snake.SetPos(0, 14);
    snake.direction = Snake::Direction::kLeft;
    for (int i = 0; i < 16; i++) {
        snake.Update(maze);
    }
    EXPECT_FLOAT_EQ(27.0, snake.GetX());
    EXPECT_FLOAT_EQ(14.0, snake.GetY());
}