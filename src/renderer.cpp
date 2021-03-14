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
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
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

void Renderer::RenderMaze(Maze const maze) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render maze
  for (int j = 0; j < maze.getH(); j++) {
    for (int i = 0; i < maze.getW(); i++) {
      switch (maze.getPosType(i, j)) {
        case Maze::PosType::kWall:
          if (maze.IsGrowing()) {
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
  for (int j = 0; j < maze.getH(); j++) {
    for (int i = 0; i < maze.getW(); i++) {
      switch (maze.getPosType(i, j)) {
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

void Renderer::RenderSnake(Snake const snake) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  // Render snake's head
  block.x = static_cast<int>(snake.GetX() * block.w);
  block.y = static_cast<int>(snake.GetY() * block.h);
  if (snake.alive) {
    switch (snake.GetColor()) {
      case Snake::Color::kYellow:
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0x00, 0xFF);
        break;
      case Snake::Color::kRed:
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
        break;
      default: // White
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillCircle(sdl_renderer, block.x + block.w / 2, block.y + block.h / 2, block.w / 2);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
