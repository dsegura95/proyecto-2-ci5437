/*
 *  Copyright (C) 2012 Universidad Simon Bolivar
 * 
 *  Permission is hereby granted to distribute this software for
 *  non-commercial research purposes, provided that this copyright
 *  notice is included with any such distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 *  EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
 *  SOFTWARE IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU
 *  ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 *  
 *  Blai Bonet, bonet@ldc.usb.ve
 *
 *  Last revision: 1/11/2016
 *
 */

#include <cassert>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <tuple>

using namespace std;

#define DIM           36
#define N             6

// rows[pos] contiene la fila a la que pertenece pos, donde pos es una casilla del
// tablero cuya enumeracion se explicara mas adelante. El valor -1 indica el final
// de cada fila.
const int rows[][7] = {
    { 4, 5, 6, 7, 8, 9,-1 }, { 4, 5, 6, 7, 8, 9,-1 }, { 4, 5, 6, 7, 8, 9,-1 },
    { 4, 5, 6, 7, 8, 9,-1 }, { 4, 5, 6, 7, 8, 9,-1 }, { 4, 5, 6, 7, 8, 9,-1 },
    {10,11,12,13,14,15,-1 }, {10,11,12,13,14,15,-1 }, {10,11,12,13,14,15,-1 },
    {10,11,12,13,14,15,-1 }, {10,11,12,13,14,15,-1 }, {10,11,12,13,14,15,-1 },
    {16,17, 0, 1,18,19,-1 }, {16,17, 0, 1,18,19,-1 },
    {16,17, 0, 1,18,19,-1 }, {16,17, 0, 1,18,19,-1 },
    {20,21, 2, 3,22,23,-1 }, {20,21, 2, 3,22,23,-1 },
    {20,21, 2, 3,22,23,-1 }, {20,21, 2, 3,22,23,-1 },
    {24,25,26,27,28,29,-1 }, {24,25,26,27,28,29,-1 }, {24,25,26,27,28,29,-1 },
    {24,25,26,27,28,29,-1 }, {24,25,26,27,28,29,-1 }, {24,25,26,27,28,29,-1 },
    {30,31,32,33,34,35,-1 }, {30,31,32,33,34,35,-1 }, {30,31,32,33,34,35,-1 },
    {30,31,32,33,34,35,-1 }, {30,31,32,33,34,35,-1 }, {30,31,32,33,34,35,-1 }
};
// cols[pos] contiene la columna a la que pertenece pos, donde pos es una casilla del
// tablero cuya enumeracion se explicara mas adelante. El valor -1 indica el final
// de cada columna.
const int cols[][7] = {
    { 4,10,16,20,24,30,-1 }, { 5,11,17,21,25,31,-1 }, { 6,12, 0, 2,26,32,-1 },
    { 7,13, 1, 3,27,33,-1 }, { 8,14,18,22,28,34,-1 }, { 9,15,19,23,29,35,-1 },
    { 4,10,16,20,24,30,-1 }, { 5,11,17,21,25,31,-1 }, { 6,12, 0, 2,26,32,-1 },
    { 7,13, 1, 3,27,33,-1 }, { 8,14,18,22,28,34,-1 }, { 9,15,19,23,29,35,-1 },
    { 4,10,16,20,24,30,-1 }, { 5,11,17,21,25,31,-1 },
    { 8,14,18,22,28,34,-1 }, { 9,15,19,23,29,35,-1 },
    { 4,10,16,20,24,30,-1 }, { 5,11,17,21,25,31,-1 },
    { 8,14,18,22,28,34,-1 }, { 9,15,19,23,29,35,-1 },
    { 4,10,16,20,24,30,-1 }, { 5,11,17,21,25,31,-1 }, { 6,12, 0, 2,26,32,-1 },
    { 7,13, 1, 3,27,33,-1 }, { 8,14,18,22,28,34,-1 }, { 9,15,19,23,29,35,-1 },
    { 4,10,16,20,24,30,-1 }, { 5,11,17,21,25,31,-1 }, { 6,12, 0, 2,26,32,-1 },
    { 7,13, 1, 3,27,33,-1 }, { 8,14,18,22,28,34,-1 }, { 9,15,19,23,29,35,-1 }
};
// dia1[pos] contiene la diagonal normal (decreciente) a la que pertenece pos, donde 
// pos es una casilla del tablero cuya enumeracion se explicara mas adelante. El 
// valor -1 indica el final de cada diagonal.
const int dia1[][7] = {
    { 4,11, 0, 3,28,35,-1 }, { 5,12, 1,22,29,-1,-1 }, { 6,13,18,23,-1,-1,-1 },
    { 7,14,19,-1,-1,-1,-1 }, { 8,15,-1,-1,-1,-1,-1 }, { 9,-1,-1,-1,-1,-1,-1 },
    {10,17, 2,27,34,-1,-1 }, { 4,11, 0, 3,28,35,-1 }, { 5,12, 1,22,29,-1,-1 },
    { 6,13,18,23,-1,-1,-1 }, { 7,14,19,-1,-1,-1,-1 }, { 8,15,-1,-1,-1,-1,-1 },
    {16,21,26,33,-1,-1,-1 }, {10,17, 2,27,34,-1,-1 },
    { 6,13,18,23,-1,-1,-1 }, { 7,14,19,-1,-1,-1,-1 },
    {20,25,32,-1,-1,-1,-1 }, {16,21,26,33,-1,-1,-1 },
    { 5,12, 1,22,29,-1,-1 }, { 6,13,18,23,-1,-1,-1 },
    {24,31,-1,-1,-1,-1,-1 }, {20,25,32,-1,-1,-1,-1 }, {16,21,26,33,-1,-1,-1 },
    {10,17, 2,27,34,-1,-1 }, { 4,11, 0, 3,28,35,-1 }, { 5,12, 1,22,29,-1,-1 },
    {30,-1,-1,-1,-1,-1,-1 }, {24,31,-1,-1,-1,-1,-1 }, {20,25,32,-1,-1,-1,-1 },
    {16,21,26,33,-1,-1,-1 }, {10,17, 2,27,34,-1,-1 }, { 4,11, 0, 3,28,35,-1 }
};
// dia1[pos] contiene la diagonal inversa (creciente) a la que pertenece pos, donde 
// pos es una casilla del tablero cuya enumeracion se explicara mas adelante. El 
// valor -1 indica el final de cada diagonal.
const int dia2[][7] = {
    { 4,-1,-1,-1,-1,-1,-1 }, { 5,10,-1,-1,-1,-1,-1 }, { 6,11,16,-1,-1,-1,-1 },
    { 7,12,17,20,-1,-1,-1 }, { 8,13, 0,21,24,-1,-1 }, { 9,14, 1, 2,25,30,-1 },
    { 5,10,-1,-1,-1,-1,-1 }, { 6,11,16,-1,-1,-1,-1 }, { 7,12,17,20,-1,-1,-1 },
    { 8,13, 0,21,24,-1,-1 }, { 9,14, 1, 2,25,30,-1 }, {15,18, 3,26,31,-1,-1 },
    { 6,11,16,-1,-1,-1,-1 }, { 7,12,17,20,-1,-1,-1 },
    {15,18, 3,26,31,-1,-1 }, {19,22,27,32,-1,-1,-1 },
    { 7,12,17,20,-1,-1,-1 }, { 8,13, 0,21,24,-1,-1 },
    {19,22,27,32,-1,-1,-1 }, {23,28,33,-1,-1,-1,-1 },
    { 8,13, 0,21,24,-1,-1 }, { 9,14, 1, 2,25,30,-1 }, {15,18, 3,26,31,-1,-1 },
    {19,22,27,32,-1,-1,-1 }, {23,28,33,-1,-1,-1,-1 }, {29,34,-1,-1,-1,-1,-1 },
    { 9,14, 1, 2,25,30,-1 }, {15,18, 3,26,31,-1,-1 }, {19,22,27,32,-1,-1,-1 },
    {23,28,33,-1,-1,-1,-1 }, {29,34,-1,-1,-1,-1,-1 }, {35,-1,-1,-1,-1,-1,-1 }
};

// Moves on the principal variation
static int PV[] = {
    12, 21, 26, 13, 22, 18,  7,  6,  5, 27, 33, 23, 17, 11, 19, 15,
    14, 31, 20, 32, 30, 10, 25, 24, 34, 28, 16,  4, 29, 35, 36,  8,
    9, -1
};

class othello_t {
  private:
    // t_ representa las 4 casillas centrales del tablero, las cuales nunca estan libres.
    // Enumera las casillas centrales de la siguiente forma:
    // 00 01
    // 02 03
    // tal que si la i-esima casilla es 1, entonces la ficha es negra, blanca en caso contrario.
    unsigned char t_; 

    // free_ son 32 bits tal que, si el i-esimo bit a partir de la derecha es 0, entonces la 
    // i-esima posicion esta libre. Las casillas son enumeradas de la siguiente manera:
    // 00 01 02 03 04 05
    // 06 07 08 09 10 11
    // 12 13 -- -- 14 15
    // 16 17 -- -- 18 19
    // 20 21 22 23 24 25
    // 26 27 28 29 30 31
    unsigned free_;

    // pos_ funciona de forma similar a free_. Si la i-sima posicion es 1, entonces la casilla
    // es negra, blanca en caso contrario.
    // Las variables 'pos' que son argumentos en las funciones posteriores enumeraran las 
    // casillas de la siguiente manera:
    // 04 05 06 07 08 09
    // 10 11 12 13 14 15
    // 16 17 00 01 18 19
    // 20 21 02 03 22 23
    // 24 25 26 27 28 29
    // 30 31 32 33 34 35
    unsigned pos_;

  public:
    // El estado es inicializado con t_ = 6, ya que en bits seria 0110, lo que coincide con 
    // la posicion inicial de las casillas centrales
    // WHITE  BLACK
    // BLACK  WHITE
    explicit othello_t(unsigned char t = 6) : t_(t), free_(0), pos_(0) { }

    unsigned char t() const { return t_; }
    unsigned free() const { return free_; }
    unsigned pos() const { return pos_; }
    
    /* 
     * Se hace XOR entre las 3 variable que definen el estado. Es posible que hayan estados
     * con el mismo valor de hash, pero parece que es poco probable. Ademas, para almacenar
     * todos los estados se necesitarian 64 bits, lo que duplicaria el tipo de retorno, y 
     * por lo tanto, la memoria para almacenar el hash de los estados.
     */
    size_t hash() const { return free_ ^ pos_ ^ t_; }

    /*
     * Para verificar si una posicion es negra, primero verificamos si es menor que 4, pues 
     * las primeras 4 posiciones corresponde a las centrales. En caso de ser asi, hacemos 
     * shift left a 1 tantas veces como pos indique. Por lo tanto, el resultado de esta 
     * operacion seria:
     *      pos = 0  =>  0001  
     *      pos = 1  =>  0010  
     *      pos = 2  =>  0100  
     *      pos = 3  =>  1000
     * Luego, hacemos AND con t_. Si el bit con 1 de la operacion anterior tambien aparece 
     * con 1 en t_, entonces la casilla es negra, blanca en caso contrario. Si la posicion 
     * es mayor a 3, entonces hacemos practicamente la misma operacion, pero el shift left
     * se hara pos-4 veces (para eliminar las 4 posiciones ya mencionadas), y se compara con
     * pos_ en lugar de t_. 
     */ 
    bool is_black(int pos) const { return pos < 4 ? t_ & (1 << pos) : pos_ & (1 << (pos - 4)); }
    // Funciona similar a is_black, pero se retorna su negado.
    bool is_white(int pos) const { return ! (pos < 4 ? t_ & (1 << pos) : pos_ & (1 << (pos - 4))); }

    /*
     * Funciona practicamente igual que las funciones anteriores. Solo que si pos < 4, 
     * entonces se retorna false, pues las posiciones centrales nunca estan libres. En caso
     * de ser mayor que 3, entonces se compara con free_ en lugar de t_ como en la funcion
     * if_black.
     */ 
    bool is_free(int pos) const { return pos < 4 ? false : !(free_ & (1 << (pos - 4))); }

    // Se verifica que el negado de free_ sea 0, es decir, que todos sus bits sean 1.
    bool is_full() const { return ~free_ == 0; }

    // Retorna el valor de juego de un estado.
    int value() const;
    // Verifica si un estado es terminal.
    bool terminal() const;
    // Verifica si se puede colocar una pieza en una posicion.
    bool outflank(bool color, int pos) const;
    // Verifica si una ficha negra se puede colocar en la posicion indicada.
    // TODO: No se para que se realiza la comparacion pos == DIM, en teoria la posicion
    // DIM no corresponde a ninguna dentro del tablero.
    bool is_black_move(int pos) const { return (pos == DIM) || outflank(true, pos); }
    // Verifica si una ficha negra se puede colocar en la posicion indicada.
    bool is_white_move(int pos) const { return (pos == DIM) || outflank(false, pos); }
    // Coloca una casilla en un determinado color.
    void set_color(bool color, int pos);
    // Realiza un movimiento de un determinado color sobre una casilla.
    othello_t move(bool color, int pos) const;
    // Realiza un movimiento de las fichas negras en una determinada posicion.
    othello_t black_move(int pos) { return move(true, pos); }
    // Realiza un movimiento de las fichas blancas en una determinada posicion.
    othello_t white_move(int pos) { return move(false, pos); }

    
    /*
     * Obtenemos los movimientos validos de un color determinado
     */
    std::vector<int> get_valid_moves(bool color) {
      std::vector<int> valid_moves;
      // Por cada posible posicion.
      for( int pos = 0; pos < DIM; pos++ ) {
        // Si se puede realizar el movimiento de ese color.
        if( (color && is_black_move(pos)) || (!color && is_white_move(pos)) ) {
          // Lo agregamos al vector de movimientos validos.
          valid_moves.push_back(pos);
        }
      }
      // Si no hay movimientos validos, retornamos DIM
      if(valid_moves.empty()) {
        return {DIM};
      }
      return valid_moves;
    }

    /*
     * Obtenemos un movimiento aleatorio de un color determinado.
     */
    int get_random_move(bool color) {
      //Se verifica que hayan movimientos validos
      if (terminal()) return -1;
      std::vector<int> valid_moves = get_valid_moves(color);
      // Si hay movimientos validos, se retorna un movimiento valido.
      return valid_moves[lrand48() % valid_moves.size()];
    }

    bool operator<(const othello_t &s) const {
        return (free_ < s.free_) || ((free_ == s.free_) && (pos_ < s.pos_));
    }
    bool operator==(const othello_t &state) const {
        return (state.t_ == t_) && (state.free_ == free_) && (state.pos_ == pos_);
    }
    const othello_t& operator=(const othello_t &state) {
      t_ = state.t_;
      free_ = state.free_;
      pos_ = state.pos_;
      return *this;
    }

    // Imprimimos una representacion del tablero.
    void print(std::ostream &os) const;
    // Imprimimos los bits que representan al estado.
    void print_bits(std::ostream &os) const;
};

/*
 * Retorna el valor de juego de un estado, el cual se calcula como la suma del numero
 * de casillas negras menos el numero de casillas blancas.
 */
inline int othello_t::value() const {
  int v = 0;
  for( int pos = 0; pos < DIM; ++pos ) {
    if( !is_free(pos) ) v += is_black(pos) ? 1 : -1;
  }
  return v;
}

/*
 * Verifica si un estado es terminal. Un estado de othello es terminal si esta full
 * o no existen movimientos validos.
 */
inline bool othello_t::terminal() const {
    if( is_full() ) return true;
    for( unsigned b = 0; b < DIM; ++b )
        if( is_black_move(b) || is_white_move(b) ) return false;
    return true;
}

/*
 * Verifica si se puede colocar una pieza de un determinado color en una determinada
 * posicion. Esto ocurre si flanquea a una o mas fichas de color distinto. Si color 
 * es true, se considerara negra a la pieza, en caso contraria se considerara blanca.
 */
inline bool othello_t::outflank(bool color, int pos) const {
  // Si la posicion indicada no esta libre, entonces no se puede colocar ninguna pieza.
  if( !is_free(pos) ) return false;

  const int *p = 0;

  // Find if some stones are outflanked

  // Verificamos la fila.
  // x contendra la direccion con la posicion de la casilla dentro de la fila en la que 
  // se encuentra
  const int *x = rows[pos - 4];
  while( *x != pos ) ++x;
  // Si x no es la ultima posicion de la fila, verificamos el resto de la fila.
  if( *(x+1) != -1 ) {
    // Aumentamos p en la fila mientras no sea -1, no este libre y su color no sea el
    // del parametro.
    for( p = x + 1; (*p != -1) && !is_free(*p) && (color ^ is_black(*p)); ++p );
    // Si p esta a al menos dos casillas de x, no es -1 y no esta libre, significa que
    // x y p son del mismo color y entre ellas hay piezas de distinto color, por lo que 
    // la pieza se puede colocar en x.
    if( (p > x + 1) && (*p != -1) && !is_free(*p) ) return true;
  }
  // Si x no es el inicio de la fila, verificamos lo que hay antes en la fila.
  if( x != rows[pos - 4] ) {
    // Disminuimos p en la fila mientras no llegue al inicio de esta, no este libre y su 
    // color no sea el del parametro.
    for( p = x - 1; (p >= rows[pos - 4]) && !is_free(*p) && (color ^ is_black(*p)); --p );
    // Si p esta a al menos dos casillas de x, no es el inicio de la fila y no esta 
    // libre, significa que p y x son del mismo color y entre ellas hay piezas de distinto 
    // color, por lo que la pieza se puede colocar en x.
    if( (p < x - 1) && (p >= rows[pos - 4]) && !is_free(*p) ) return true;
  }

  // Verificamos la columna.
  // x contendra la direccion con la posicion de la casilla dentro de la columna en la que 
  // se encuentra. La verificacion es exactamente igual que con las filas, pero revisando
  // la variable cols en lugar de rows.
  x = cols[pos - 4];
  while( *x != pos ) ++x;
  if( *(x+1) != -1 ) {
      for( p = x + 1; (*p != -1) && !is_free(*p) && (color ^ is_black(*p)); ++p );
      if( (p > x + 1) && (*p != -1) && !is_free(*p) ) return true;
  }
  if( x != cols[pos - 4] ) {
      for( p = x - 1; (p >= cols[pos - 4]) && !is_free(*p) && (color ^ is_black(*p)); --p );
      if( (p < x - 1) && (p >= cols[pos - 4]) && !is_free(*p) ) return true;
  }

  // Verificamos la diagonal normal (decreciente).
  // x contendra la direccion con la posicion de la casilla dentro de la diagonal en la que 
  // se encuentra. La verificacion es exactamente igual que con las filas, pero revisando
  // la variable dia1 en lugar de rows.
  x = dia1[pos - 4];
  while( *x != pos ) ++x;
  if( *(x+1) != -1 ) {
      for( p = x + 1; (*p != -1) && !is_free(*p) && (color ^ is_black(*p)); ++p );
      if( (p > x + 1) && (*p != -1) && !is_free(*p) ) return true;
  }
  if( x != dia1[pos - 4] ) {
      for( p = x - 1; (p >= dia1[pos - 4]) && !is_free(*p) && (color ^ is_black(*p)); --p );
      if( (p < x - 1) && (p >= dia1[pos - 4]) && !is_free(*p) ) return true;
  }

  // Verificamos la diagonal inversa (creciente).
  // x contendra la direccion con la posicion de la casilla dentro de la diagonal en la que 
  // se encuentra. La verificacion es exactamente igual que con las filas, pero revisando
  // la variable dia2 en lugar de rows.
  x = dia2[pos - 4];
  while( *x != pos ) ++x;
  if( *(x+1) != -1 ) {
      for( p = x + 1; (*p != -1) && !is_free(*p) && (color ^ is_black(*p)); ++p );
      if( (p > x + 1) && (*p != -1) && !is_free(*p) ) return true;
  }
  if( x != dia2[pos - 4] ) {
      for( p = x - 1; (p >= dia2[pos - 4]) && !is_free(*p) && (color ^ is_black(*p)); --p );
      if( (p < x - 1) && (p >= dia2[pos - 4]) && !is_free(*p) ) return true;
  }

  // Si la verificacion falla en la fila, columna y diagonales en las que se encuentra la
  // posicion indicada, entonces no se puede colocar la ficha alli.
  return false;
}

/*
 * Colocamos una casilla en un determinado color.
 */
inline void othello_t::set_color(bool color, int pos) {
  // Si es negro.
  if( color ) {
    // Si la posicion es menor a 4, entonces estamos en las posiciones centrales,
    // por lo que establecemos la posicion correspondiente en t_ en 1 usando
    // la operacion OR.
    if( pos < 4 ) {
      t_ |= 1 << pos;
    } 
    // Si no, establecemos la posicion correspondiente en free_ como 1 (indicando
    // que ya no esta libre) y en pos_ como 1 tambien (indicando que tiene una 
    // pieza negra).
    else {
      free_ |= 1 << (pos - 4);
      pos_ |= 1 << (pos - 4);
    }

  } 
  // Si es blanco.
  else {
    // Si la posicion es menor a 4, entonces estamos en las posiciones centrales,
    // por lo que establecemos la posicion correspondiente en t_ en 0 usando
    // la operacion AND.
    if( pos < 4 ) {
      t_ &= ~(1 << pos);
    } 
    // Si no, establecemos la posicion correspondiente en free_ como 1 (indicando
    // que ya no esta libre) y en pos_ como 0 (indicando que tiene una pieza
    // blanca).
    else {
      free_ |= 1 << (pos - 4);
      pos_ &= ~(1 << (pos - 4));
    }
  }
}

/*
 * Realiza un movimiento de un determinado color sobre una casilla, modificando las
 * casillas correspondientes debido a dicho movimiento.
 */
inline othello_t othello_t::move(bool color, int pos) const {
  // Creamos un estado exactamente igual al actual.
  othello_t s(*this);

  // Las posiciones llegan hasta DIM-1, por lo que alguno superior a ese corresponde
  // a no realizar ningun movimiento.
  if( pos >= DIM ) return s;

  // Para evitar calculos redundantes, quitaremos esta verificacion.
  //assert(outflank(color, pos));

  s.set_color(color, pos);

  // Flip color of outflanked stones

  // Verificamos la fila.
  // El proceso de voltear las fichas flanqueadas es exactamente el mismo que el de 
  // verificar si hay fichas flanqueadas de la funcion outflanked, con la diferencia 
  // que, al encontrar dichas fichas, estas se voltean en lugar de retornar true.
  const int *p = 0, *x = rows[pos - 4];
  while( *x != pos ) ++x;
  if( *(x+1) != -1 ) {
    for( p = x + 1; (*p != -1) && !is_free(*p) && (color ^ is_black(*p)); ++p );
    if( (p > x + 1) && (*p != -1) && !is_free(*p) ) {
      for( const int *q = x + 1; q < p; ++q ) s.set_color(color, *q);
    }
  }
  if( x != rows[pos - 4] ) {
    for( p = x - 1; (p >= rows[pos - 4]) && !is_free(*p) && (color ^ is_black(*p)); --p );
    if( (p < x - 1) && (p >= rows[pos - 4]) && !is_free(*p) ) {
      for( const int *q = x - 1; q > p; --q ) s.set_color(color, *q);
    }
  }

  // Verificamos la columna.
  x = cols[pos - 4];
  while( *x != pos ) ++x;
  if( *(x+1) != -1 ) {
    for( p = x + 1; (*p != -1) && !is_free(*p) && (color ^ is_black(*p)); ++p );
    if( (p > x + 1) && (*p != -1) && !is_free(*p) ) {
      for( const int *q = x + 1; q < p; ++q ) s.set_color(color, *q);
    }
  }
  if( x != cols[pos - 4] ) {
    for( p = x - 1; (p >= cols[pos - 4]) && !is_free(*p) && (color ^ is_black(*p)); --p );
    if( (p < x - 1) && (p >= cols[pos - 4]) && !is_free(*p) ) {
      for( const int *q = x - 1; q > p; --q ) s.set_color(color, *q);
    }
}

  // Verificamos la diagonal normal (decreciente).
  x = dia1[pos - 4];
  while( *x != pos ) ++x;
  if( *(x+1) != -1 ) {
    for( p = x + 1; (*p != -1) && !is_free(*p) && (color ^ is_black(*p)); ++p );
    if( (p > x + 1) && (*p != -1) && !is_free(*p) ) {
      for( const int *q = x + 1; q < p; ++q ) s.set_color(color, *q);
    }
  }
  if( x != dia1[pos - 4] ) {
    for( p = x - 1; (p >= dia1[pos - 4]) && !is_free(*p) && (color ^ is_black(*p)); --p );
    if( (p < x - 1) && (p >= dia1[pos - 4]) && !is_free(*p) ) {
      for( const int *q = x - 1; q > p; --q ) s.set_color(color, *q);
    }
  }

  // Verificamos la diagonal inversa (creciente).
  x = dia2[pos - 4];
  while( *x != pos ) ++x;
  if( *(x+1) != -1 ) {
    for( p = x + 1; (*p != -1) && !is_free(*p) && (color ^ is_black(*p)); ++p );
    if( (p > x + 1) && (*p != -1) && !is_free(*p) ) {
      for( const int *q = x + 1; q < p; ++q ) s.set_color(color, *q);
    }
  }
  if( x != dia2[pos - 4] ) {
    for( p = x - 1; (p >= dia2[pos - 4]) && !is_free(*p) && (color ^ is_black(*p)); --p );
    if( (p < x - 1) && (p >= dia2[pos - 4]) && !is_free(*p) ) {
      for( const int *q = x - 1; q > p; --q ) s.set_color(color, *q);
    }
  } 

  return s;
}

/*
 * Imprimimos una representacion grafica del tablero en la terminal.
 */
inline void othello_t::print(std::ostream &os) const {
  os << "┌";
  for( int j = 0; j < 2*N; ++j ) os << "─";
  os << "┐" << std::endl;

  int pos = 4;
  for( int i = 0; i < N; ++i ) {
    os << "│\e[100m";
    for( int j = 0; j < N; ++j ) {
      if( ((i != 2) && (i != 3)) || ((j != 2) && (j != 3)) ) {
        os << (is_free(pos) ? "  " : (is_black(pos) ? "\e[30m\u2B24 \e[0;100m" : "\u2B24 "));
        ++pos;
      } else {
        int p = ((i-2) << 1) + (j-2);
        os << (is_free(p) ? "  " : (is_black(p) ? "\e[30m\u2B24 \e[0;100m" : "\u2B24 "));
      }
    }
    os << "\e[0m│" << std::endl;
  }

  os << "└";
  for( int j = 0; j < 2*N; ++j ) os << "─";
  os << "┘" << std::endl;
}

/*
 * Imprimimos los bits que representan al estado de la forma:
 * t_:pos_:free_
 */
inline void othello_t::print_bits(std::ostream &os) const {
    for( int i = 3; i >= 0; --i ) os << (t_ & (1 << i) ? '1' : '0');
    os << ":";
    for( int i = 31; i >= 0; --i ) os << (pos_ & (1 << i) ? '1' : '0');
    os << ":";
    for( int i = 31; i >= 0; --i ) os << (free_ & (1 << i) ? '1' : '0');
}

inline std::ostream& operator<<(std::ostream &os, const othello_t &state) {
    state.print(os);
    return os;
}

