#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::string filename) : engine(dev()) {
  _maze = std::make_shared<Maze>(filename);
  _pacman = std::make_shared<Pacman>(Snake::Color::kYellow);
  _pacman->SetPos(_maze->getPacmanSpawnX(), _maze->getPacmanSpawnY());
  for (int i = 0; i < _maze->GetMonstersNum(); i++) {
    _monsters.emplace_back(std::make_shared<Monster>(Monster::GetColorFromSpawnType(_maze->GetMonsterSpawnType(i))));
    _monsters[i]->SetPos(_maze->GetMonsterSpawnX(i), _maze->GetMonsterSpawnY(i));
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  renderer.SetMaze(_maze);
  renderer.AppendSnake(_pacman);
  for (int i = 0; i < _maze->GetMonstersNum(); i++) {
    renderer.AppendSnake(_monsters[i]);
  }
  renderer.RunRenderer(target_frame_duration);
  while (running) {
    frame_start = SDL_GetTicks();
    controller.HandleInput(running, *_pacman);
    Update();
    renderer.SetScore(score);
    frame_end = SDL_GetTicks();
    frame_duration = frame_end - frame_start;
    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  renderer.StopRenderer();
}

void Game::Update() {
  if (!_pacman->alive) return;
  if (_maze->GetFoodNum() == 0) {
    _maze->GrowWall();
    return;
  }
  _pacman->Update(*_maze);
  float x, y;
  _pacman->GetPos(x, y);
  for (int i = 0; i < _maze->GetMonstersNum(); i++) {
    _monsters[i]->SetDirection(*_maze, x, y);
    _monsters[i]->Update(*_maze);
  }
  int new_x = static_cast<int>(x + 0.5);
  int new_y = static_cast<int>(y + 0.5);

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
  switch (_maze->getPosType(new_x, new_y)) {
    case Maze::PosType::kFood:
      _maze->clearFood(new_x, new_y);
      score++;
      break;
    case Maze::PosType::kPowFood:
      _maze->clearFood(new_x, new_y);
      score++;
  }
}

int Game::GetScore() const { return score; }

int Game::GetGridW() const {
  return _maze->getW();
}

int Game::GetGridH() const {
  return _maze->getH();
}