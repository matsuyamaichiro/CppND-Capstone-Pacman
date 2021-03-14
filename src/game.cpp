#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::string filename) : engine(dev()), _maze(filename), _pacman(Snake::Color::kYellow), _monster(Snake::Color::kRed) {
  _pacman.SetPos(_maze.getPacmanSpawnX(), _maze.getPacmanSpawnY());
  _monster.SetPos(_maze.getMonsterSpawnX(), _maze.getMonsterSpawnY());
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, _pacman);
    Update();
    renderer.RenderMaze(_maze);
    renderer.RenderSnake(_pacman);
    renderer.RenderSnake(_monster);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::Update() {
  if (!_pacman.alive) return;
  if (_maze.GetFoodNum() == 0) {
    _maze.GrowWall();
    return;
  }

  _pacman.Update(_maze);
  _monster.SetDirection(_maze, _pacman.GetX(), _pacman.GetY());
  _monster.Update(_maze);

  int new_x = static_cast<int>(_pacman.GetX() + 0.5);
  int new_y = static_cast<int>(_pacman.GetY() + 0.5);

  // Check if there's food over here
  //
  // sample case figure (Food has not been eaten)
  // (#: Wall, o: Packman, x: Origin of Pacman, $: Food, X: Origin of Food)
  //   0        1       2       
  // 4 #########        ########
  //   #########        ########
  // 5 X       xoooooo         
  //      $$   oooooooo       
  //      $$   oooooooo       
  //            oooooo        
  // 6 #########################
  //   #########################
  // (_x = 0.825, new_x = 1)
  // (_y = 5.000, new_y = 5)
  //
  switch (_maze.getPosType(new_x, new_y)) {
    case Maze::PosType::kFood:
      _maze.clearFood(new_x, new_y);
      score++;
      break;
    case Maze::PosType::kPowFood:
      _maze.clearFood(new_x, new_y);
      score++;
  }
}

int Game::GetScore() const { return score; }

int Game::GetGridW() const {
  return _maze.getW();
}

int Game::GetGridH() const {
  return _maze.getH();
}