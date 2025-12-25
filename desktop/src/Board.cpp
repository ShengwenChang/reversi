#include "Board.h"

// private helper functions

// draws a piece as a filled circle
void Board::draw_filled_circle(SDL_Renderer* renderer,
                               float centerX,
                               float centerY,
                               float radius,
                               SDL_Color color) const {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  int radiusInt = static_cast<int>(radius);
  int centerXInt = static_cast<int>(centerX);
  int centerYInt = static_cast<int>(centerY);

  for (int dy = -radiusInt; dy <= radiusInt; dy++) {
    for (int dx = -radiusInt; dx <= radiusInt; dx++) {
      if (dx * dx + dy * dy <= radiusInt * radiusInt) {
        SDL_RenderPoint(renderer, centerXInt + dx, centerYInt + dy);
      }
    }
  }
}

// flips a piece on the board
void Board::flip(int row, int col) {
  switch (board[row][col]) {
    case Piece::BLACK:
      board[row][col] = Piece::WHITE;
      break;

    case Piece::WHITE:
      board[row][col] = Piece::BLACK;
      break;

    case Piece::EMPTY:
      break;
  }
}

bool Board::in_bound(int row, int col) const {
  return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

// returns pos of flips at a pos for a color
vector<pair<int, int>> Board::get_flips(int row, int col, Piece color) const {
  vector<pair<int, int>> flips;

  if (!Board::in_bound(row, col))
    return flips;
  if (board[row][col] != Piece::EMPTY)
    return flips;

  Piece opponent = (color == Piece::BLACK) ? Piece::WHITE : Piece::BLACK;

  for (const auto& [dx, dy] : DIRECTIONS) {
    vector<pair<int, int>> current_flips;
    int cr = row + dx;
    int cc = col + dy;

    // checks neighboring cell
    if (!Board::in_bound(cr, cc) || board[cr][cc] != opponent) {
      continue;
    }

    // traverses in direction
    while (Board::in_bound(cr, cc) && board[cr][cc] == opponent) {
      current_flips.push_back({cr, cc});
      cr += dx;
      cc += dy;
    }

    // checks end cell
    if (Board::in_bound(cr, cc) && board[cr][cc] == color) {
      flips.insert(flips.end(), current_flips.begin(), current_flips.end());
    }
  }

  return flips;
}

// returns pos of valid moves for a color
vector<pair<int, int>> Board::get_valid_moves(Piece color) const {
  vector<pair<int, int>> valid_moves;
  if (color == Piece::EMPTY)
    return valid_moves;

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j] == Piece::EMPTY &&
          !Board::get_flips(i, j, color).empty()) {
        valid_moves.push_back({i, j});
      }
    }
  }
  return valid_moves;
}

// updates cached valid moves for both colors
void Board::update_next_moves() {
  valid_moves_black = Board::get_valid_moves(Piece::BLACK);
  valid_moves_white = Board::get_valid_moves(Piece::WHITE);
}

// public functions

// constructor
Board::Board(Piece _player_color) {
  player_color = _player_color;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      board[i][j] = Piece::EMPTY;
    }
  }
  board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2 - 1] = Piece::WHITE;
  board[BOARD_SIZE / 2][BOARD_SIZE / 2] = Piece::WHITE;
  board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2] = Piece::BLACK;
  board[BOARD_SIZE / 2][BOARD_SIZE / 2 - 1] = Piece::BLACK;
  update_next_moves();
}

// casts ths board into a char array
void Board::to_char(char char_board[8][26]) const {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      switch (board[i][j]) {
        case Piece::WHITE:
          char_board[i][j] = 'W';
          break;
        case Piece::BLACK:
          char_board[i][j] = 'B';
          break;
        default:
          char_board[i][j] = 'U';
          break;
      }
    }
    // Fill the rest of the row with empty
    for (int j = 8; j < 26; j++) {
      char_board[i][j] = 'U';
    }
  }
}

// returns true if game ends
bool Board::game_end() const {
  bool full_board = true;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j] == Piece::EMPTY) {
        full_board = false;
        break;
      }
    }
  }
  return full_board && valid_moves_black.empty() && valid_moves_white.empty();
}

// returns true if a color has valid moves
bool Board::has_moves(Piece color) const {
  if (color == Piece::EMPTY)
    return false;
  return color == Piece::BLACK ? !valid_moves_black.empty()
                               : !valid_moves_white.empty();
}

// gets the winner of the game
string Board::get_winner() const {
  int black_count = 0;
  int white_count = 0;

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j] == Piece::BLACK) {
        black_count++;
      } else if (board[i][j] == Piece::WHITE) {
        white_count++;
      }
    }
  }

  if (black_count > white_count) {
    return "Black";
  } else if (white_count > black_count) {
    return "White";
  } else {
    return "Tie";
  }
}

// draws the entire board onto the screen
void Board::draw_board(SDL_Renderer* renderer, Piece color) const {
  // draws a white background
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  // draws grid lines
  for (int i = 0; i < BOARD_SIZE; i++) {
    SDL_RenderLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_SIZE);
    SDL_RenderLine(renderer, 0, i * CELL_SIZE, WINDOW_SIZE, i * CELL_SIZE);
  }

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      // draws alternating cell colors
      (i + j) % 2 == 0 ? SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255)
                       : SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
      SDL_FRect cell = {i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE};
      SDL_RenderFillRect(renderer, &cell);

      // draws pieces
      switch (board[i][j]) {
        case Piece::BLACK:
          Board::draw_filled_circle(renderer, i * CELL_SIZE + CELL_SIZE / 2,
                                    j * CELL_SIZE + CELL_SIZE / 2,
                                    CELL_SIZE / 2 - 10,
                                    SDL_Color{1, 1, 1, 255});
          break;

        case Piece::WHITE:
          Board::draw_filled_circle(renderer, i * CELL_SIZE + CELL_SIZE / 2,
                                    j * CELL_SIZE + CELL_SIZE / 2,
                                    CELL_SIZE / 2 - 10,
                                    SDL_Color{252, 252, 252, 255});
          break;

        case Piece::EMPTY:
          break;
      }
    }
  }

  // draws next available spots for player
  if (color == player_color) {
    const vector<pair<int, int>>& next_moves =
        (player_color == Piece::BLACK) ? valid_moves_black : valid_moves_white;
    for (const auto& [i, j] : next_moves) {
      Board::draw_filled_circle(renderer, i * CELL_SIZE + CELL_SIZE / 2,
                                j * CELL_SIZE + CELL_SIZE / 2,
                                CELL_SIZE / 2 - 10,
                                SDL_Color{245, 223, 77, 255});
    }
  }
}

// makes a move and flips all related pieces
bool Board::make_move(int row, int col, Piece color) {
  // Validate inputs
  if (!Board::in_bound(row, col) || color == Piece::EMPTY ||
      board[row][col] != Piece::EMPTY)
    return false;

  vector<pair<int, int>> flips = Board::get_flips(row, col, color);
  if (flips.empty())
    return false;

  board[row][col] = color;
  for (const auto& [flip_row, flip_col] : flips) {
    Board::flip(flip_row, flip_col);
  }
  update_next_moves();
  return true;
}
