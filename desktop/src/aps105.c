#include "aps105.h"
int makeMove(const char board[][26], int n, char turn, int* row, int* col) {
  // Variable to track the maximum number of flips found
  int maxFlips = 0;
  char opponent;

  // Determine opponent's color
  if (turn == 'B') {
    opponent = 'W';
  } else {
    opponent = 'B';
  }

  // Initialize the best move to invalid values
  *row = -1;
  *col = -1;

  // Iterate through all board positions
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      // Skip if position is not empty
      if (board[i][j] != 'U') {
        continue;
      }

      int currentFlips = 0;

      // Check all 8 directions
      for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
          // Skip the current position (dx = 0, dy = 0)
          if (dx == 0 && dy == 0) {
            continue;
          }

          int tempFlips = 0;
          int r = i + dx;
          int c = j + dy;

          // Check along this direction
          while (r >= 0 && r < n && c >= 0 && c < n) {
            if (board[r][c] == opponent) {
              tempFlips++;
              r += dx;
              c += dy;
            } else if (board[r][c] == turn && tempFlips > 0) {
              // Found current player's piece after opponent pieces
              currentFlips += tempFlips;
              break;
            } else {
              // Empty or other condition
              break;
            }
          }
        }
      }

      // Update best move if this one flips more pieces
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