#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Board.h"
#include "main.h"

class Controller {
 private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Event event;
  Piece player_color;
  Piece current_turn;
  Board game_board;

  void handle_player_move(int x, int y);
  void handle_ai_move();

 public:
  Controller(Piece _player_color);
  ~Controller();

  bool run();
};
#endif
