#include "main.h"
#include <iostream>
#include "Board.h"
#include "Controller.h"

int main() {
  // handles user input
  cout << "Please enter the user color (black/white): " << endl;
  string user_input;
  cin >> user_input;
  while (cin.fail() || (user_input != "black" && user_input != "white")) {
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "Invalid input. Please enter 'black' or 'white': " << endl;
    cin >> user_input;
  }
  cout << endl;

  Controller controller(user_input == "black" ? Piece::BLACK : Piece::WHITE);
  controller.run();

  return 0;
}
