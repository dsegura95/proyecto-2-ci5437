// Game of Othello -- Example of main
// Universidad Simon Bolivar, 2012.
// Author: Blai Bonet
// Last Revision: 1/11/16
// Modified by: 

#include <iostream>
#include <limits>
#include "othello.hpp"
#include "algorithms.hpp"
#include "utils.hpp"

#define DEPTH 50

using namespace std;

// En las siguientes variables almacenaremos los resultados de las distintas ejecuciones
// para luego poder graficarlas
vector<unsigned long> states_generated, states_expanded;
vector<double> times;
pid_t child;

/*
  Imprime los resultados actuales y finaliza la ejecucion.
*/
void print_results(int s){
  kill(child, 1);
  // Imprimimos los resultados.
  cout << "\n\n\033[1;3mRESULTS:\033[0m\nTOTAL GENERATED: ";
  Utils::print_vector<unsigned long>(states_generated);
  cout << "TOTAL EXPANDED: ";
  Utils::print_vector<unsigned long>(states_expanded);
  cout << "TOTAL TIME: ";
  Utils::print_vector<double>(times);
  exit(0); 
}

int main(int argc, const char **argv) {
  Utils::set_handler(print_results);

  // pv contiene los estados de la variacion principal, tal que el ultimo elemento es el 
  // estado inicial.
  othello_t pv[34];

  // Contamos el numero de estados de la variacion principal.
  int npv = 0;
  for( int i = 0; PV[i] != -1; ++i ) ++npv;

  // Verificacion de argumentos.
  int algorithm = 0, seconds = 60, max_depth = 0, verbose = 1;
  int use_tt = 0;
  bool verb_flag = false;
  float prob = 0;
  if (argc > 1) {
    if (((string) argv[1]) == "-v") {
      verb_flag = true;
      verbose = 0;
    } else if (((string) argv[1]) == "-vv") {
      verb_flag = true;
      verbose = 1;
    } else if (((string) argv[1]) == "-vvv") {
      verb_flag = true;
      verbose = 2;
    }
  }
  else if (argc == 1) {
    // Imprimiendo la sintaxis.
    cout << "\033[1;31mSyntax error\033[0m. Remember that the script sinopsis is:\n\n"
      << "    \033[1mothello.out\033[0m [ \033[4mVERBOSE\033[0m ] \033[4mALGORITHM\033[0m [ \033[4mSECONDS\033[0m [ \033[4mTT\033[0m ] ]\n\n"
      << "where: \n"
      << "    \033[4mVERBOSE\033[0m can be:\n"
      << "        -  -v => Only print global results\n"
      << "        -  -vv => Print results of every state and global results (default)\n"
      << "        -  -vvv => Print results and a representation of every state and global results\n"
      << "    \033[4mALGORITHM\033[0m can be:\n"
      << "        -  0 => Negamax\n"
      << "        -  1 => Negamax with alpha-beta pruning\n"
      << "        -  2 => Scout\n"
      << "        -  3 => Negascout\n"
      << "    \033[4mSECONDS\033[0m must be a natural number (default: 60).\n"
      << "    \033[4mTT\033[0m can be:\n"
      << "        -  0           => Without transposition table (default)\n"
      << "        -  1           => Total pruning\n"
      << "        -  2 [ \033[4mDEPTH\033[0m ] => Pruning until \033[4mDEPTH\033[0m (default 0)\n"
      << "        -  3 [ \033[4mPROB\033[0m ]  => Pruning with probability \033[4mPROB\033[0m% (default 0)\n";
      exit(1);
  }
  if (argc > 1 + verb_flag) algorithm = atoi(argv[1 + verb_flag]);;
  if (argc > 2 + verb_flag) seconds = atoi(argv[2 + verb_flag]);
  if (argc > 3 + verb_flag) use_tt = atoi(argv[3 + verb_flag]);
  if (argc > 4 + verb_flag) {
    if (use_tt == 2) max_depth = atoi(argv[4 + verb_flag]);
    else if (use_tt == 3) {
      prob = MAX(0, atof(argv[4 + verb_flag]));
      prob = MIN(100, prob);
    }
  }
  
  cout << "\033[1;3mOthello's Principal Variation Solver\033[0m\n\n";

  // Verificacion de argumentos.
  // Algoritmo que usaremos:
  pair<int, int> (*best_move) (othello_t, int, bool, bool);
  cout << "Run script with ";
  switch (algorithm) {
    case 1: 
      cout << "\e[1;3mNegamax with alpha-beta\e[0m, "; 
      best_move = best_move_negamax_ab;
      break;
    case 2: 
      cout << "\e[1;3mScout\e[0m, "; 
      best_move = best_move_scout;
      break;
    case 3: 
      cout << "\e[1;3mNegascout\e[0m, "; 
      best_move = best_move_negascout;
      break;
    default: 
      cout << "\e[1;3mNegamax\e[0m, "; 
      best_move = best_move_negamax;
      break;
  }
  cout << "max-seconds = " << seconds << " and ";
  switch (use_tt) {
    case 1: 
    cout << "total transposition table.\n"; 
    TT = othello_TT_t();
    TTab = othello_TTab_t();
    break;
    
    case 2: 
    cout << "transposition table with a max-depth of " << max_depth << ".\n"; 
    TT = othello_TT_t(max_depth, DEPTH);
    TTab = othello_TTab_t(max_depth, DEPTH);
    break;
    
    case 3: 
    cout << "transposition table with a probability of " << prob << "%.\n"; 
    TT = othello_TT_t(prob);
    TTab = othello_TTab_t(prob);
    break;
    
    default:
      cout << "without transposition table.\n";
      use_tt = 0;
      break;
  }

  // Obtenemos los estados de la variacion principal.
  othello_t state;
  cout << "\nExtracting principal variation (PV) with " << npv << " plays ... ";
  for( int i = 0; PV[i] != -1; ++i ) {
      bool player = i % 2 == 0; // black moves first!
      int pos = PV[i];
      pv[npv - i] = state;
      state = state.move(player, pos);
  }
  pv[0] = state;
  cout << "done!\n" << endl;


  // El color inicial es blanco porque en el ultimo estado de PV era el turno de las blancas.
  bool color = false;
  pair<int, int> move;
  pair<pair<unsigned long, unsigned long>, double> result;
  pid_t parent = getpid();

  for (int i = 0; i <= npv; i++) { 
    if (verbose > 0) cout << "STATE " << i << "\n";
    if (verbose > 1) cout << pv[i];
    TT.clear();
    TTab.clear();

    // Iniciamos el conteo del tiempo.
    start = clock();
    expanded = 0;
    generated = 0;

    if ((child = fork()) == 0) Utils::cronometer(seconds, parent);
    
    // Aplicamos el algoirtmo al i-esimo estado.
    move = best_move(pv[i], DEPTH , color, use_tt);

    kill(child, 1);

    // Imprimimos los resultados.
    if (verbose > 0) {
      cout << "BEST MOVE PREDICTED (" << (color ? "BLACKS" : "WHITES") << "): "  << move.first << "\n"
        << "BEST MOVE IN PV: " << PV[npv - i] << "\n"
        << "STATE VALUE: " << move.second << "\n";
    }

    result = get_results(verbose);

    // Almacenamos los resultados.
    states_expanded.push_back(result.first.first);
    states_generated.push_back(result.first.second);
    times.push_back(result.second);

    // Turno del otro jugador
    color = ! color; 
  }

  print_results(0);

  return 0;
}

