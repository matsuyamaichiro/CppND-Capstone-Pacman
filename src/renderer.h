#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "maze.h"
#include <memory>
#include <thread>

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();
  void SetMaze(std::shared_ptr<Maze> maze);
  void AppendSnake(std::shared_ptr<Snake> snake);
  void RunRenderer(int target_frame_duration);
  void StopRenderer(void);
  void SetScore(int);

 private:
  void RenderMaze();
  void RenderSnakes();
  void Render();
  void MainLoop();
  void UpdateWindowTitle(int fps);
  std::shared_ptr<Maze> _maze;
  std::vector<std::shared_ptr<Snake>> _snakes;
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  int _target_frame_duration;
  bool _running{false}; // thread safe
  int _score{0}; // thread safe
  std::thread _thread;
};

#endif