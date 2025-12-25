# Reversi (Desktop)

This folder contains a small GUI implementation of Reversi (Othello) using C++ and SDL3. It is intended to be easy to build locally with CMake and to allow cutomized AI implementation in C (for APS105 students).

## Requirements

- C++ Compiler
- CMake
- SDL3

## Build (Windows - PowerShell)

From the the `desktop/` directory, create a build directory and run CMake:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

The `CMake Tools` extension in vscode is recommended that significantly simplifies the configuration and build process.

## Run

After a successful build, the executable will be in `build\` (for example `build\reversi_desktop.exe`). On Windows you can run the game by double-clicking `build\reversi_desktop.exe` or from PowerShell:

```powershell
.\build\reversi_desktop.exe
```

## Customize the AI (for APS105)

To use your own C AI implementation, replace the content in `desktop/src/aps105.c` with your own `C` implementation , then re-run the `build` and `run` steps above. The application will link against the `aps105.c` implementation so your AI logic will be used by the desktop GUI.

Remeber there must have been a function with the following signature in your implementation.

```c
int makeMove(const char board[][26], int n, char turn, int* row, int* col);
```

Happy hacking!
