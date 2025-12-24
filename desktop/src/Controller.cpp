#include "Controller.h"
#include "aps105.h"

// constructor
Controller::Controller(Piece _player_color) : game_board(_player_color) {
  if (!SDL_Init(SDL_INIT_EVENTS)) {
    SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    return;
  }

  window = SDL_CreateWindow("SDL3 Window", WINDOW_SIZE, WINDOW_SIZE,
                            SDL_WINDOW_ALWAYS_ON_TOP);
  renderer = SDL_CreateRenderer(window, NULL);
  player_color = _player_color;
  current_turn = Piece::BLACK;  // always starts with BLACK

  SDL_Log("Initializing GUI successfully.");
}

// destructor
Controller::~Controller() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool Controller::run() {
  bool is_quit = false;
  bool player_moved = false;
  int player_x = 0, player_y = 0;
  game_board.draw_board(renderer, current_turn);

  // main run loop
  while (!is_quit && !game_board.game_end()) {
    player_moved = false;

    // polls SDL events
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          is_quit = true;
          break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          if (event.button.button == SDL_BUTTON_LEFT) {
            player_x = event.button.x / CELL_SIZE;
            player_y = event.button.y / CELL_SIZE;
            player_moved = true;
            break;
          }
        default: {
          break;
        }
      }
    }

    if (!game_board.has_moves(current_turn)) {
      current_turn = current_turn == Piece::BLACK ? Piece::WHITE : Piece::BLACK;
      SDL_Log("No valid moves available for %s. Turn skipped.",
              current_turn == Piece::BLACK ? "BLACK" : "WHITE");
      continue;
    }

    if (current_turn == player_color) {
      if (player_moved) {
        Controller::handle_player_move(player_x, player_y);
      }
    } else {
      Controller::handle_ai_move();
    }

    // refreshes the entire screen
    game_board.draw_board(renderer, current_turn);
    SDL_RenderPresent(renderer);
  }

  SDL_Log("\nWinner: %s", game_board.get_winner().c_str());
  SDL_Log("Game exit successfully.");
  return 0;
}

// handles player move
void Controller::handle_player_move(int x, int y) {
  if (!game_board.make_move(x, y, current_turn)) {
    SDL_Log("Invalid player move at (%d, %d)", x, y);
  } else {
    current_turn = current_turn == Piece::BLACK ? Piece::WHITE : Piece::BLACK;
    SDL_Log("Player placed at (%d, %d)", x, y);
  }
}

// handles AI move
void Controller::handle_ai_move() {
  // calls c AI functions
  int ai_x = 0, ai_y = 0;
  char turn = current_turn == Piece::BLACK ? 'B' : 'W';
  char char_board[8][26];
  game_board.to_char(char_board);
  makeMove(char_board, BOARD_SIZE, turn, &ai_x, &ai_y);

  if (!game_board.make_move(ai_x, ai_y, current_turn)) {
    SDL_Log("Invalid AI move at (%d, %d)", ai_x, ai_y);
  } else {
    current_turn = current_turn == Piece::BLACK ? Piece::WHITE : Piece::BLACK;
    SDL_Log("AI placed at (%d, %d)", ai_x, ai_y);
  }
}