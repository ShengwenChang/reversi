#ifndef MAIN_H
#define MAIN_H

#include <sdl3/SDL.h>
#include <array>
#include <utility>
#include <vector>

#define WINDOW_SIZE 800
#define BOARD_SIZE 8
#define CELL_SIZE (WINDOW_SIZE / BOARD_SIZE)

enum class Piece { EMPTY, BLACK, WHITE };
constexpr std::array<std::pair<int, int>, 8> DIRECTIONS = {
    {{-1, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

#endif  // MAIN_H
