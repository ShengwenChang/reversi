#include "aps105.h"

// DeepSeek implemented reversi AI-alogorithm to choose the best move
int makeMove(const char board[][26], int n, char turn, int* row, int* col) {
  int maxFlips = 0;
  char opponent;

  if (turn == 'B') {
    opponent = 'W';
  } else {
    opponent = 'B';
  }

  *row = -1;
  *col = -1;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] != 'U') {
        continue;
      }

      int currentFlips = 0;
      for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
          if (dx == 0 && dy == 0) {
            continue;
          }

          int tempFlips = 0;
          int r = i + dx;
          int c = j + dy;

          while (r >= 0 && r < n && c >= 0 && c < n) {
            if (board[r][c] == opponent) {
              tempFlips++;
              r += dx;
              c += dy;
            } else if (board[r][c] == turn && tempFlips > 0) {
              currentFlips += tempFlips;
              break;
            } else {
              break;
            }
          }
        }
      }

      if (currentFlips > maxFlips) {
        maxFlips = currentFlips;
        *row = i;
        *col = j;
      }
    }
  }

  // Return 1 if a valid move was found, 0 otherwise
  if (maxFlips > 0) {
    return 1;
  } else {
    return 0;
  }
}