#ifndef BOARD_H
#define BOARD_H

#include "main.h"
#include "string"
using namespace std;

class Board {
 private:
  Piece player_color;
  Piece board[BOARD_SIZE][BOARD_SIZE];
  vector<pair<int, int>> valid_moves_black;
  vector<pair<int, int>> valid_moves_white;

  void draw_filled_circle(SDL_Renderer* renderer,
                          float centerX,
                          float centerY,
                          float radius,
                          SDL_Color color) const;
  bool in_bound(int x, int y) const;
  void flip(int row, int col);
  vector<pair<int, int>> get_flips(int x, int y, Piece color) const;
  vector<pair<int, int>> get_valid_moves(Piece color) const;
  void update_next_moves();

 public:
  Board(Piece player_color);
  void draw_board(SDL_Renderer* renderer, Piece color) const;
  bool make_move(int x, int y, Piece piece);
  bool game_end() const;
  bool has_moves(Piece color) const;
  void to_char(char char_board[8][26]) const;
  string get_winner() const;
};

#endif  // BOARD_H
