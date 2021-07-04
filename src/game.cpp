#include <iostream>
#include <limits>
#include <map>
#include "othello.hpp"
#include "algorithms.hpp"

using namespace std;

map<string, int> moves = {
  {"1a\n",  4}, {"1b\n",  5}, {"1c\n",  6}, {"1d\n",  7}, {"1e\n",  8}, {"1f\n",  9}, 
  {"2a\n", 10}, {"2b\n", 11}, {"2c\n", 12}, {"2d\n", 13}, {"2e\n", 14}, {"2f\n", 15}, 
  {"3a\n", 16}, {"3b\n", 17}, {"3c\n",  0}, {"3d\n",  1}, {"3e\n", 18}, {"3f\n", 19}, 
  {"4a\n", 20}, {"4b\n", 21}, {"4c\n",  2}, {"4d\n",  3}, {"4e\n", 22}, {"4f\n", 23}, 
  {"5a\n", 24}, {"5b\n", 25}, {"5c\n", 26}, {"5d\n", 27}, {"5e\n", 28}, {"5f\n", 29}, 
  {"6a\n", 30}, {"6b\n", 31}, {"6c\n", 32}, {"6d\n", 33}, {"6e\n", 34}, {"6f\n", 35},
  {"P\n" , 36} 
};

int main(int argc, const char **argv) {
  if (argc != 3) {
    cout << "\033[1;31mSyntax error\033[0m. Remember that the script sinopsis is:\n\n"
      << "    \033[1mgame.out\033[0m \033[4mDEPTH\033[0m \033[4mBLACKS\033[0m\n";
    return 1;
  }
  int depth = atoi(argv[1]);
  bool is_black = atoi(argv[2]);
  othello_t state;
  bool color = true;
  char str[124];
  TTab = othello_TTab_t(60.0);

  state.print_board(cout);
  cout << "\n\n";

  while (! state.terminal()) {
    if (color == is_black) {
      cout << "Indique su jugada: ";
      while (
        (fgets(str, sizeof(str), stdin) == NULL) || 
        moves.count((string) str) == 0 || 
        ! state.outflank(color, moves[(string) str])
      ) {
        cout << "Error: jugada invalida.\n";
        cout << "Indique su jugada: ";
      }
      state = state.move(color, moves[(string) str]);

    } else {
      TTab.clear();
      state = state.move(color, best_move_scout(state, depth , color, false).first);
    }
    state.print_board(cout);
    cout << "\n\n";
    color = ! color;
  }

  if (state.value() == 0) {
    cout << "Tables\n";
  } else if (is_black == (state.value() > 0)) {
    cout << "You Win!\n";
  } else {
    cout << "You Lose\n";
  }
  return 0;
}