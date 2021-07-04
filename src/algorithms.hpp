
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <bits/stdc++.h>
#include <map>
#include <tuple>
#include "othello.hpp"
#include "utils.hpp"

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a > b ? b : a

using namespace std;

enum tt_type_t {TOTAL, DEPTH, RANDOM};
/*
 * Clase para almacenar la tabla de transposicion de othello.
 */
class othello_TT_t {
  private:
    // Mapea el valor de hash de un estado a su valor de juego.
    map<tuple<bool, unsigned char, unsigned, unsigned>, int> values_;
    // Maxima profundidad a la que se almacenaran estados.
    int max_depth_;
    int init_depth_;
    // Probabilidad de almacenar un estado.
    float p_;
    // Tipo de tabla de transposicion
    tt_type_t type_;

    // Almacena siempre un estado y su valor.
    void add_total(othello_t state, bool color, int value) { 
      this->values_[{color, state.t(), state.pos(), state.free()}] = value; 
    }

    // Almacenamos un estado y su valor solo si se encuentra a menor profundidad que la 
    // maxima indicada.
    void add_depth(othello_t state, bool color, int value, int depth) { 
      if (init_depth_ - depth <= max_depth_)
        this->values_[{color, state.t(), state.pos(), state.free()}] = value; 
    }

    // Almacenamos un estado probabilisticamente.
    void add_random(othello_t state, bool color, int value) {
      if ((lrand48() % 100) <= p_) {
        this->values_[{color, state.t(), state.pos(), state.free()}] = value; 
      }
    }

  public:
    othello_TT_t(void) : type_(TOTAL) { }
    othello_TT_t(float prob) : p_(prob), type_(RANDOM) { }
    othello_TT_t(int max_depth, int init_depth) : max_depth_(max_depth), init_depth_(init_depth), type_(DEPTH) { }

    // Almacena un estado y su valor.
    void add(othello_t state, bool color, int value, int depth) {
      switch (type_) {
        case TOTAL: add_total(state, color, value); break;

        case DEPTH: add_depth(state, color, value, depth); break;

        case RANDOM: add_random(state, color, value); break;
      }
    }

    // Verificamos si un estado se encuentra almacenado.
    bool find(othello_t state, bool color) { 
      return this->values_.count({color, state.t(), state.pos(), state.free()}) > 0;
    }

    int get_value(othello_t state, bool color) {
      return this->values_[{color, state.t(), state.pos(), state.free()}];
    }

    // Eliminamos todos los estados almacenados.
    void clear(void) { this->values_.clear(); }
};

// Variable global para usar la tabla de transposicion.
othello_TT_t TT;




enum tt_flag_t {EXACT, LOWERBOUND, UPPERBOUND};
using tt_entry = pair<int, tt_flag_t>;
/*
 * Clase para almacenar la tabla de transposicion de othello con poda alpha-beta.
 */
class othello_TTab_t {
  private:
    // Mapea el valor de hash de un estado a su valor de juego.
    map<tuple<bool, unsigned char, unsigned, unsigned>, tt_entry> values_;
    tt_flag_t flag;
    // Maxima profundidad a la que se almacenaran estados.
    int max_depth_;
    int init_depth_;
    // Probabilidad de almacenar un estado.
    float p_;
    // Tipo de tabla de transposicion
    tt_type_t type_;

    // Almacena siempre un estado y su valor.
    void add_total(othello_t state, bool color, tt_entry value) { 
      this->values_[{color, state.t(), state.pos(), state.free()}] = value; 
    }

    // Almacenamos un estado y su valor solo si se encuentra a menor profundidad que la 
    // maxima indicada.
    void add_depth(othello_t state, bool color, tt_entry value, int depth) { 
      if (init_depth_ - depth <= max_depth_)
        this->values_[{color, state.t(), state.pos(), state.free()}] = value; 
    }

    // Almacenamos un estado probabilisticamente.
    void add_random(othello_t state, bool color, tt_entry value) {
      if ((lrand48() % 100) <= p_) {
        this->values_[{color, state.t(), state.pos(), state.free()}] = value; 
      }
    }

  public:
    othello_TTab_t(void) : type_(TOTAL) { }
    othello_TTab_t(float prob) : p_(prob), type_(RANDOM) { }
    othello_TTab_t(int max_depth, int init_depth) : max_depth_(max_depth), init_depth_(init_depth), type_(DEPTH) { }

    // Almacena un estado y su valor.
    void add(othello_t state, bool color, tt_entry value, int depth) {
      switch (type_) {
        case TOTAL: add_total(state, color, value); break;

        case DEPTH: add_depth(state, color, value, depth); break;

        case RANDOM: add_random(state, color, value); break;
      }
    }

    // Verificamos si un estado se encuentra almacenado.
    bool find(othello_t state, bool color) { 
      return this->values_.count({color, state.t(), state.pos(), state.free()}) > 0;
    }

    tt_entry get_value(othello_t state, bool color) {
      return this->values_[{color, state.t(), state.pos(), state.free()}];
    }

    // Eliminamos todos los estados almacenados.
    void clear(void) { this->values_.clear(); }
};

// Variable global para usar la tabla de transposicion con poda alpha-beta.
othello_TTab_t TTab;




clock_t start = 0;
unsigned long expanded = 0;
unsigned long generated = 0;
/*
 * Imprime los resultados de un algoritmo respecto a eficiencia.
 */
pair<pair<unsigned long, unsigned long>, double> get_results(int verbose) {
  double time_taken = double(clock() - start) / double(CLOCKS_PER_SEC);

  if (verbose > 0) {
    cout << "\033[1mExpanded\033[0m = " << expanded << "\n"
      << "\033[1mGenerated\033[0m = " << generated << "\n"
      << "\033[1mSeconds\033[0m = " << time_taken << "\n"
      << "\033[1mGenerated / Seconds\033[0m = " << generated / time_taken << "\n\n\n\n";
  }

  return {{expanded, generated}, time_taken};
}




/*
 * Implementacion del algoritmo negamax.
 */
int negamax(othello_t state, int depth, bool color, bool use_tt = false) {
  // Verificamos si ya tenemos almacenados el valor en la tabla de transposicion
  if (use_tt && TT.find(state, color)) return TT.get_value(state, color);

  // Si la profundidad es 0 o el estado es terminal, retornamos el valor del estado.
  if (depth == 0 || state.terminal()) return (2 * color - 1) * state.value();
  expanded += 1;

  // Obtenemos el score maximo entre el negativo del negamax de los sucesores del 
  // estado actual.
  int score = -INT32_MAX;

  for (int pos : state.valid_moves(color)) {
    othello_t child = state.move(color, pos);
    generated += 1;

    score = MAX(score , -negamax(child, depth-1, !color, use_tt));
  }

  // Almacenamos el valor en la tabla de transposicion
  if (use_tt) TT.add(state, color, score, depth);

  return score;
}

/*
 * Escoge la mejor jugada para un color dado un estado usando negamax
 */
pair<int, int> best_move_negamax(othello_t state, int depth, bool color, bool use_tt = false) {

  // Si el estado esta lleno, no hay movimiento posible.
  if (state.is_full()) return {-1, 0};

  int best_score = -INT32_MAX, best_move = DIM, score;

  // El mejor estado sera aquel que maximice el valor del siguiente estado.
  for (int pos : state.valid_moves(color)) {
    othello_t child = state.move(color, pos);
    generated += 1;
    score = -negamax(child, depth-1, !color, use_tt);
    if (score > best_score) {
      best_move = pos;
      best_score = score;
    }
  }
  return {best_move, (2 * color - 1) * best_score};
}



/*
 * Implementacion del algoritmo negamax con poda alpha-beta.
 */
int negamax_ab(othello_t state, unsigned depth, bool color, bool use_tt = false, 
    int alpha = -INT32_MAX, int beta = INT32_MAX) {
  // Almacenamos el valor original de alpha
  int last_alpha = alpha;

  // Verificamos el valor en la tabla de transposicion
  if (use_tt && TTab.find(state, color)) {
    tt_entry e = TTab.get_value(state, color);

    switch(e.second) {
      case EXACT: 
        return e.first;

      case LOWERBOUND: 
        alpha = MAX(alpha, e.first);
        break;

      case UPPERBOUND:
        beta = MIN(beta, e.first);
        break;
    }

    // Poda alpha-beta.
    if (alpha >= beta) return e.first;
  }

  // Si la profundidad es 0 o el estado es terminal, retornamos el valor del estado.
  if (depth == 0 || state.terminal()) return (2 * color - 1) * state.value();
  expanded += 1;

  // Obtenemos el score maximo entre el negativo del negamax de los sucesores del 
  // estado actual.
  int score = -INT32_MAX;

  for (int pos : state.valid_moves(color)) {
    othello_t child = state.move(color, pos);
    generated += 1;

    // Obtenemos el valor del hijo.
    score = MAX(score, -negamax_ab(child, depth-1, !color, use_tt, -beta, -alpha));

    // Aplicamos la poda alpha-beta.
    alpha = MAX(alpha, score);
    if (alpha >= beta) break;
  }

  // Almacenamos el valor en la tabla de transposicion
  if (use_tt) {
    tt_entry e = {score, EXACT};
    if (score <= last_alpha) e.second = UPPERBOUND;
    else if (score >= beta) e.second = LOWERBOUND;
    TTab.add(state, color, e, depth);
  }

  return score;
}

/*
 * Escoge la mejor jugada para un color dado un estado usando negamax con poda alpha-beta.
 */
pair<int, int> best_move_negamax_ab(othello_t state, int depth, bool color, bool use_tt = false) {
  // Si el estado esta lleno, no hay movimiento posible.
  if (state.is_full()) return {-1, 0};

  int best_score = -INT32_MAX, best_move = DIM, score;

  // El mejor estado sera aquel que maximice el valor del siguiente estado.
  for (int pos : state.valid_moves(color)) {
    othello_t child = state.move(color, pos);
    generated += 1;
    score = -negamax_ab(child, depth-1, !color, use_tt);
    if (score >= best_score) {
      best_move = pos;
      best_score = score;
    }
  }
  return {best_move, (2 * color - 1) * best_score};
}



// Funciones auxiliares para test.
bool gt(int a, int b) { return a > b; }
bool geq(int a, int b) { return a >= b; }
/*
 * Test usado por scout para verificar que existe algun estado sucesor con un valor mayor al
 * score indicado en caso de ser una ficha negra; o algun sucesor con un valor menor al score
 * indicado en caso de ser una ficha blanca.
 */
bool test(othello_t state, int depth, bool color, int score, bool (*cond)(int, int)) {
  // Si la profundidad es 0 o el estado es terminal.
  if (depth == 0 || state.terminal()) return cond(state.value(), score);
  expanded += 1;

  for (int pos : state.valid_moves(color)) {
    othello_t child = state.move(color, pos);
    generated += 1;

    if (color && test(child, depth-1, !color, score, cond)) return true;
    if (!color && ! test(child, depth-1, !color, score, cond)) return false;
  }
  return !color;
}

/*
 * Implementacion del algoritmo scout.
 */
int scout(othello_t state, int depth, bool color, bool use_tt = false) {
  // Verificamos si ya tenemos almacenados el valor en la tabla de transposicion
  if (use_tt && TT.find(state, color)) return TT.get_value(state, color);

  // Si la profundidad es 0 o el estado es terminal, retornamos el valor del estado.
  if (depth == 0 || state.terminal()) return state.value();
  expanded += 1;

  // Obtenemos el score del primer sucesor
  vector<int> valid_moves = state.valid_moves(color);
  othello_t child = state.move(color, valid_moves[0]);
  generated += 1;
  int score = scout(child, depth-1, !color, use_tt);
  
  for (long unsigned i = 1; i < valid_moves.size(); i++) {
    othello_t child = state.move(color, valid_moves[i]);
    generated += 1;
    
    if (color && test(child, depth-1, !color, score, gt))  {
      // Obtenemos el valor del hijo.
      score = scout(child, depth-1, !color, use_tt);
    }
    if (!color && ! test(child, depth-1, !color, score, geq)) {
      // Obtenemos el valor del hijo.
      score = scout(child, depth-1, !color, use_tt);
    }
  }

  // Almacenamos el valor en la tabla de transposicion
  if (use_tt) TT.add(state, color, score, depth);

  return score;
}

/*
 * Escoge la mejor jugada para un color dado un estado usando scout
 */
pair<int, int> best_move_scout(othello_t state, int depth, bool color, bool use_tt = false) {

  // Si el estado esta lleno, no hay movimiento posible.
  if (state.is_full()) return {-1, 0};

  // Obtenemos el score del primer sucesor
  vector<int> valid_moves = state.valid_moves(color);
  othello_t child = state.move(color, valid_moves[0]);
  generated += 1;
  int score = scout(child, depth-1, !color, use_tt);
  int best_move = valid_moves[0];

  for (long unsigned i = 1; i < valid_moves.size(); i++) {
    othello_t child = state.move(color, valid_moves[i]);
    generated += 1;
    
    if (color && test(child, depth-1, !color, score, gt))  {
      score = scout(child, depth-1, !color, use_tt);
      best_move = valid_moves[i];
    }
    if (!color && ! test(child, depth-1, !color, score, geq)) {
      score = scout(child, depth-1, !color, use_tt);
      best_move = valid_moves[i];
    }
  }

  return {best_move, score};
}



/*
 * Implementacion del algoritmo negascout.
 */
int negascout(othello_t state, unsigned depth, bool color, bool use_tt = false, 
    int alpha = -INT32_MAX, int beta = INT32_MAX) {
  // Almacenamos el valor original de alpha.
  int last_alpha = alpha;
  
  // Verificamos el valor en la tabla de transposicion
  if (use_tt && TTab.find(state, color)) {
    tt_entry e = TTab.get_value(state, color);

    switch(e.second) {
      case EXACT: 
        return e.first;

      case LOWERBOUND: 
        alpha = MAX(alpha, e.first);
        break;

      case UPPERBOUND:
        beta = MIN(beta, e.first);
        break;
    }

    // Poda alpha-beta.
    if (alpha >= beta) return e.first;
  }

  // Si la profundidad es 0 o el estado es terminal, retornamos el valor del estado.
  if (depth == 0 || state.terminal()) return (2 * color - 1) * state.value();
  expanded += 1;

  int b = beta;

  for (int pos : state.valid_moves(color)) {
    othello_t child = state.move(color, pos);
    generated += 1;
    
    // Calculamos el valor del nodo hijo.
    alpha = MAX(alpha, -negascout(child, depth-1, !color, use_tt, -b, -alpha));

    // Poda alpha-beta
    if (alpha >= beta) break;

    if (alpha >= b) {
      alpha = -negascout(child, depth-1, !color, use_tt, -beta, -alpha);
      // Poda alpha-beta
      if (alpha >= beta) break;

    }
    b = alpha + 1;
  }

  // Almacenamos el valor en la tabla de transposicion
  if (use_tt) {
    tt_entry e = {alpha, EXACT};
    if (alpha <= last_alpha) e.second = UPPERBOUND;
    else if (alpha >= beta) e.second = LOWERBOUND;
    TTab.add(state, color, e, depth);
  }

  return alpha;
}

/*
 * Escoge la mejor jugada para un color dado un estado usando negascout
 */
pair<int, int> best_move_negascout(othello_t state, int depth, bool color, bool use_tt = false) {

  // Si el estado esta lleno, no hay movimiento posible.
  if (state.is_full()) return {-1, 0}; 

  int alpha = -INT32_MAX, beta = INT32_MAX, best_move = -1;
  int a, b = beta;

  for (int pos : state.valid_moves(color)) {
    othello_t child = state.move(color, pos);
    generated += 1;
    
    a = -negascout(child, depth-1, !color, use_tt, -b, -alpha);
    if (a > alpha) {
      alpha = a;
      best_move = pos;
    }
    // Poda alpha-beta
    if (alpha >= beta) return {best_move, alpha};
    if (alpha >= b) {
      alpha = -negascout(child, depth-1, !color, use_tt, -beta, -alpha);
      best_move = pos;
      // Poda alpha-beta
      if (alpha >= beta) return {best_move, alpha};
    }
    b = alpha + 1;
  }

  return {best_move, (2 * color - 1) * alpha};
}

#endif