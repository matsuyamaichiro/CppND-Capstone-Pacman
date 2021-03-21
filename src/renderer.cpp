#include "renderer.h"
#include <iostream>
#include <string>
#include "circle.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::SetMaze(std::shared_ptr<Maze> maze) {
  _maze = std::move(maze);
}

void Renderer::AppendSnake(std::shared_ptr<Snake> snake) {
  _snakes.emplace_back(std::move(snake));
}

void Renderer::RenderMaze() {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  for (int j = 0; j < _maze->getH(); j++) {
    for (int i = 0; i < _maze->getW(); i++) {
      switch (_maze->getPosType(i, j)) {
        case Maze::PosType::kWall:
          if (_maze->IsGrowing()) {
            SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF); // write
          } else {
            SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0xFF, 0xFF); // blue
          }
          break;
        case Maze::PosType::kGate:
          SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF); // white
          break;
        default:
          SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xFF); // black
      }
      block.x = i * block.w;
      block.y = j * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }
  }

  // Render food
  SDL_Rect fblock;
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF); // white
  for (int j = 0; j < _maze->getH(); j++) {
    for (int i = 0; i < _maze->getW(); i++) {
      switch (_maze->getPosType(i, j)) {
        case Maze::PosType::kFood:
          fblock.w = block.w / 8;
          fblock.h = block.h / 8;
          fblock.x = i * block.w + block.w * 7 / 16;
          fblock.y = j * block.h + block.h * 7 / 16;
          SDL_RenderFillRect(sdl_renderer, &fblock);
          break;
        case Maze::PosType::kPowFood:
          fblock.w = block.w / 4;
          fblock.h = block.h / 4;
          fblock.x = i * block.w + block.w * 3 / 8;
          fblock.y = j * block.h + block.h * 3 / 8;
          SDL_RenderFillRect(sdl_renderer, &fblock);
      }
    }
  }
}

void Renderer::RenderSnakes() {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  for (int i = 0; i < _snakes.size(); i++) {
    float x, y;
    _snakes[i]->GetPos(x, y);
    block.x = static_cast<int>(x * block.w);
    block.y = static_cast<int>(y * block.h);
    switch (_snakes[i]->GetColor()) {
      case Snake::Color::kYellow:
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0x00, 0xFF);
        break;
      case Snake::Color::kRed:
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
        break;
      case Snake::Color::kOrange:
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xB8, 0x52, 0xFF);
        break;
      case Snake::Color::kPink:
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xB8, 0xFF, 0xFF);
        break;
      case Snake::Color::kCyan:
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0xFF, 0xFF);
        break;
      case Snake::Color::kWhite:
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        break;
      case Snake::Color::kBlue:
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0xFF, 0xFF);
        break;
      case Snake::Color::kGray:
      default:
        SDL_SetRenderDrawColor(sdl_renderer, 0x80, 0x80, 0x80, 0xFF);
    }
    SDL_RenderFillCircle(sdl_renderer, block.x + block.w / 2, block.y + block.h / 2, block.w / 2);
  }
}

void Renderer::Render() {
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);
  RenderMaze();
  RenderSnakes();
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::MainLoop() {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count{0};
  while (_running) {
    frame_start = SDL_GetTicks();
    Render();
    frame_end = SDL_GetTicks();
    frame_count++;
    frame_duration = frame_end - frame_start;
    if (frame_end - title_timestamp >= 1000) {
      UpdateWindowTitle(frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }
    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < _target_frame_duration) {
      SDL_Delay(_target_frame_duration - frame_duration);
    }
  }
}

void Renderer::RunRenderer(int target_frame_duration) {
  _target_frame_duration = target_frame_duration;
  _running = true;
  _thread = std::thread(&Renderer::MainLoop, this);
}

void Renderer::StopRenderer() {
  _running = false;
  _thread.join();
}

void Renderer::SetScore(int score) {
  _score = score;
}

void Renderer::UpdateWindowTitle(int fps) {
  std::string title{"Pacman Score: " + std::to_string(_score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}