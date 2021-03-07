#include <gtest/gtest.h>

#include "../src/maze.h"

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
}