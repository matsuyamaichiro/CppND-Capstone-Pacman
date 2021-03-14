#include <gtest/gtest.h>

#include "../src/maze.h"
#include "../src/pacman.h"
#include "../src/monster.h"
#include "../src/game.h"

class MazeTest : public ::testing::Test {
    protected:
    virtual void SetUp() {
        maze = new Maze("../src/maze.csv");
    };
    virtual void TearDown() {
        delete maze;
    };
    Maze* maze;
};

TEST_F(MazeTest, Size) {
    EXPECT_EQ(maze->getH(), 30);
    EXPECT_EQ(maze->getW(), 28);
}

TEST_F(MazeTest, MazeSetting) {
    EXPECT_EQ(maze->GetFoodNum(), 240);
    EXPECT_EQ(maze->getPacmanSpawnX(), 14);
    EXPECT_EQ(maze->getPacmanSpawnY(), 23);
    EXPECT_EQ(maze->getPosType(14, 23), Maze::PosType::kBlank);
}

TEST_F(MazeTest, PosTypeAndAvailable) {
    // test around top-right corner
    EXPECT_EQ(maze->getPosType(26, 0), Maze::PosType::kWall);
    EXPECT_EQ(maze->getPosType(26, 1), Maze::PosType::kFood);
    EXPECT_EQ(maze->getPosType(26, 2), Maze::PosType::kFood);
    EXPECT_EQ(maze->getPosType(26, 3), Maze::PosType::kPowFood);
    EXPECT_EQ(maze->getPosType(25, 1), Maze::PosType::kFood);
    EXPECT_EQ(maze->getPosType(27, 1), Maze::PosType::kWall);
    EXPECT_FALSE(maze->isAvailable(26, 1, Maze::Direction::kUp));
    EXPECT_TRUE(maze->isAvailable(26, 1, Maze::Direction::kDown));
    EXPECT_TRUE(maze->isAvailable(26, 1, Maze::Direction::kLeft));
    EXPECT_FALSE(maze->isAvailable(26, 1, Maze::Direction::kRight));
    // test around monster gate
    EXPECT_EQ(maze->getPosType(14, 11), Maze::PosType::kBlank);
    EXPECT_EQ(maze->getPosType(14, 12), Maze::PosType::kGate);
    EXPECT_FALSE(maze->isAvailable(14, 11, Maze::Direction::kUp));
    EXPECT_FALSE(maze->isAvailable(14, 11, Maze::Direction::kDown));
    EXPECT_TRUE(maze->isAvailable(14, 11, Maze::Direction::kLeft));
    EXPECT_TRUE(maze->isAvailable(14, 11, Maze::Direction::kRight));
    // test around warp zone
    EXPECT_EQ(maze->getPosType(0, 14), Maze::PosType::kBlank);
    EXPECT_FALSE(maze->isAvailable(0, 14, Maze::Direction::kUp));
    EXPECT_FALSE(maze->isAvailable(0, 14, Maze::Direction::kDown));
    EXPECT_TRUE(maze->isAvailable(0, 14, Maze::Direction::kLeft));
    EXPECT_TRUE(maze->isAvailable(0, 14, Maze::Direction::kRight));
    EXPECT_EQ(maze->getPosType(27, 14), Maze::PosType::kBlank);
    EXPECT_FALSE(maze->isAvailable(27, 14, Maze::Direction::kUp));
    EXPECT_FALSE(maze->isAvailable(27, 14, Maze::Direction::kDown));
    EXPECT_TRUE(maze->isAvailable(27, 14, Maze::Direction::kLeft));
    EXPECT_TRUE(maze->isAvailable(27, 14, Maze::Direction::kRight));
}

TEST_F(MazeTest, ClearFood) {
    // test clear food
    EXPECT_EQ(maze->getPosType(26, 1), Maze::PosType::kFood);
    maze->clearFood(26, 1);
    EXPECT_EQ(maze->GetFoodNum(), 239);
    EXPECT_EQ(maze->getPosType(26, 1), Maze::PosType::kBlank);
    EXPECT_EQ(maze->getPosType(26, 3), Maze::PosType::kPowFood);
    maze->clearFood(26, 3);
    EXPECT_EQ(maze->getPosType(26, 3), Maze::PosType::kBlank);
    EXPECT_EQ(maze->getPosType(26, 0), Maze::PosType::kWall);
    maze->clearFood(26, 0);
    EXPECT_EQ(maze->getPosType(26, 0), Maze::PosType::kWall);
    EXPECT_EQ(maze->getPosType(14, 11), Maze::PosType::kBlank);
    maze->clearFood(14, 11);
    EXPECT_EQ(maze->getPosType(14, 11), Maze::PosType::kBlank);
    EXPECT_EQ(maze->getPosType(14, 12), Maze::PosType::kGate);
    maze->clearFood(14, 12);
    EXPECT_EQ(maze->getPosType(14, 12), Maze::PosType::kGate);
    for (int j = 0; j < 30; j++) {
        for (int i = 0; i < 28; i++) {
            switch (maze->getPosType(i, j)) {
                case Maze::PosType::kFood:
                case Maze::PosType::kPowFood:
                    maze->clearFood(i, j);
            }
        }
    }
    EXPECT_EQ(maze->GetFoodNum(), 0);
}

TEST_F(MazeTest, Growing) {
    // test growing
    EXPECT_EQ(maze->IsGrowing(), false);
    maze->GrowWall();
    EXPECT_EQ(maze->IsGrowing(), true);
}

TEST_F(MazeTest, PacmanMoving) {
    Pacman pacman(Snake::Color::kYellow);
    // initial position
    //    14
    // 23  o
    pacman.SetPos(14, 23);
    pacman.SetDirection(Pacman::Direction::kLeft);
    EXPECT_FLOAT_EQ(pacman.GetX(), 14.0);
    EXPECT_FLOAT_EQ(pacman.GetY(), 23.0);
    // position after 1 tick
    //    13  14
    // 23     oo
    pacman.Update(*maze);
    EXPECT_FLOAT_EQ(pacman.GetX(), 14.0 - 1.0/16);
    EXPECT_FLOAT_EQ(pacman.GetY(), 23.0);
    // position after moving 1 grid
    //    13  14
    // 23  o<--o
    for (int i = 0; i < 15; i++) {
        pacman.Update(*maze);
    }
    EXPECT_FLOAT_EQ(pacman.GetX(), 13.0);
    EXPECT_FLOAT_EQ(pacman.GetY(), 23.0);
    // position after touching wall
    //     5 6      13
    // 22  0
    // 23  0 o<------o
    // 24  0
    for (int i = 0; i < 16 * 7; i++) {
        pacman.Update(*maze);
    }
    EXPECT_FLOAT_EQ(6.0, pacman.GetX());
    EXPECT_FLOAT_EQ(23.0, pacman.GetY());
    // position not moved
    //     5 6
    // 22  0
    // 23  0 o
    // 24  0
    for (int i = 0; i < 8; i++) {
        pacman.Update(*maze);
    }
    EXPECT_FLOAT_EQ(6.0, pacman.GetX());
    EXPECT_FLOAT_EQ(23.0, pacman.GetY());
    // position after intersection with holding direction
    //     6 7 8
    // 20  --->o
    // 21  | 0 0 
    // 22  | 0
    // 23  o 0
    pacman.SetDirection(Pacman::Direction::kUp);
    pacman.Update(*maze);
    pacman.SetDirection(Pacman::Direction::kRight);
    for (int i = 0; i < 15 + 16 * 4; i++) {
        pacman.Update(*maze);
    }
    EXPECT_FLOAT_EQ(8.0, pacman.GetX());
    EXPECT_FLOAT_EQ(20.0, pacman.GetY());
    // position after trying to enter gate
    //    13        18
    // 11  o-------->| 0
    // 12  2 2 0 0 0 | 0
    // 13          0 o 0
    pacman.SetPos(13, 11);
    pacman.SetDirection(Pacman::Direction::kRight);
    pacman.Update(*maze);
    pacman.SetDirection(Pacman::Direction::kDown);
    for (int i = 0; i < 15 + 16 * 6; i++) {
        pacman.Update(*maze);
    }
    EXPECT_FLOAT_EQ(18.0, pacman.GetX());
    EXPECT_FLOAT_EQ(13.0, pacman.GetY());
    // position after warping from right
    //       0       27
    // 14 -->o        o->
    pacman.SetPos(27, 14);
    pacman.SetDirection(Pacman::Direction::kRight);
    for (int i = 0; i < 16; i++) {
        pacman.Update(*maze);
    }
    EXPECT_FLOAT_EQ(0.0, pacman.GetX());
    EXPECT_FLOAT_EQ(14.0, pacman.GetY());
    // position after warping from left
    //       0       27
    // 14 <--o        o<-
    pacman.SetPos(0, 14);
    pacman.SetDirection(Pacman::Direction::kLeft);
    for (int i = 0; i < 16; i++) {
        pacman.Update(*maze);
    }
    EXPECT_FLOAT_EQ(27.0, pacman.GetX());
    EXPECT_FLOAT_EQ(14.0, pacman.GetY());
}

TEST_F(MazeTest, MonsterMoving) {
    Pacman pacman(Snake::Color::kYellow);
    // pacman initial position
    //    14
    // 23  o
    pacman.SetPos(14, 23);
    Monster monster(Snake::Color::kRed);
    // monster initial position
    //    13
    // 11  @
    monster.SetPos(13, 11);
    // position after moving 1 grid
    //    12  13
    // 11  @<--@
    for (int i = 0; i < 16; i++) {
        monster.SetDirection(*maze, pacman.GetX(), pacman.GetY());
        monster.Update(*maze);
    }
    EXPECT_EQ(12, monster.GetX());
    EXPECT_EQ(11, monster.GetY());
    // position after moving 5 grid
    //     8   9  10  11  12
    //     0   0   0   0   1
    // 11  0   |<----------@
    // 12  0   |
    // 13  0   @
    for (int i = 0; i < 16 * 5; i++) {
        monster.SetDirection(*maze, pacman.GetX(), pacman.GetY());
        monster.Update(*maze);
    }
    EXPECT_EQ(9, monster.GetX());
    EXPECT_EQ(13, monster.GetY());
    // position after moving 8 grid
    //     8   9  10  11  12
    // 13  0   @   0
    // 14  1   |   0
    // 15  0   |   0
    // 16  0   |   0
    // 17  0   |   1
    // 18  0   |   0
    // 19  0   |   0
    // 20  0   --->@
    // 21  0   0   0
    for (int i = 0; i < 16 * 8; i++) {
        monster.SetDirection(*maze, pacman.GetX(), pacman.GetY());
        monster.Update(*maze);
    }
    EXPECT_EQ(10, monster.GetX());
    EXPECT_EQ(20, monster.GetY());
    // moster position reset
    //     6
    // 18  @
    monster.SetPos(6, 18);
    EXPECT_EQ(6, monster.GetX());
    EXPECT_EQ(18, monster.GetY());
    // position after moving 3 grid
    //     5   6   7   8
    // 18  0   @   0   0
    // 19  0   |   0
    // 20  3   --->@
    // 21  0   3   0
    for (int i = 0; i < 16 * 3; i++) {
        monster.SetDirection(*maze, pacman.GetX(), pacman.GetY());
        monster.Update(*maze);
    }
    EXPECT_EQ(7, monster.GetX());
    EXPECT_EQ(20, monster.GetY());
}

TEST(Maze, ImperfectCsv) {
    Maze maze("../test/imperfect_maze.csv");
    // test around top-right corner
    EXPECT_EQ(maze.getPosType(26, 0), Maze::PosType::kWall);
    EXPECT_EQ(maze.getPosType(26, 1), Maze::PosType::kWall); // Filled
    EXPECT_EQ(maze.getPosType(26, 2), Maze::PosType::kFood);
    EXPECT_EQ(maze.getPosType(25, 1), Maze::PosType::kWall); // Filled
    EXPECT_EQ(maze.getPosType(27, 1), Maze::PosType::kWall);
    // test around bottom-left corner
    EXPECT_EQ(maze.getPosType(1, 28), Maze::PosType::kWall); // Filled
    EXPECT_EQ(maze.getPosType(1, 27), Maze::PosType::kFood);
    EXPECT_EQ(maze.getPosType(1, 29), Maze::PosType::kWall); // Filled
    EXPECT_EQ(maze.getPosType(0, 27), Maze::PosType::kWall); // Filled
    EXPECT_EQ(maze.getPosType(2, 27), Maze::PosType::kWall); // Filled
}

TEST(Maze, NoCsv) {
    ASSERT_THROW(Maze maze(""), std::invalid_argument);
}

TEST(Game, ReadMaze) {
    ASSERT_THROW(Game game(""), std::invalid_argument);
    ASSERT_NO_THROW(Game game("../src/maze.csv"));
}