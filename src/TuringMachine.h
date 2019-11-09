#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <string>
#include <set>
#include <map>
#include <deque>
#include "config.h"

using namespace std;

typedef char Symbol;
typedef string State;
typedef enum { STALL = 0, LEFT = -1, RIGHT = 1 } Direction;

inline char to_char(Direction dir) {
  switch (dir) {
    case RIGHT: return 'R';
    case STALL: return 'S';
    case LEFT: return 'L';
    default: return '?';
  }
}

struct Transition { // inner struct of TuringMachine
  Symbol symbol_new;
  State *state_next;
  Direction direction;
  Transition(Symbol symbol_new, State *state_next, Direction direction)
    : symbol_new(symbol_new), state_next(state_next), direction(direction) { }
};

class TuringMachine {
  friend class Parser;
private:
  /* definition */
  Symbol B;             // blank symbol
  set<Symbol> *S;       // input symbol set
  set<Symbol> *T;       // complete symbol set
  set<State*> *Q;       // state set
  State *q0;            // start state
  set<State*> *F;       // halt state set
  map<State*, map<Symbol, Transition*>*> *D;  // transfer function
  /* runtime */
  size_t limit_time;       // resource limit
  size_t limit_space;
  ifstream *fin;        // I/O devices
  ofstream *fout;
  ofstream *ferr;
  State *q;             // current state, init: q0
  size_t steps;         // init: 0
  deque<Symbol> *tape;  // the infinite tape
  deque<Symbol>::iterator origin; // origin position on tape, aka. 'index-0'
  deque<Symbol>::iterator head;   // current head position on tape
private:
  stringstream sb;

public:
  TuringMachine();
  ~TuringMachine();
  void run(ifstream &fin, ofstream &fout, ofstream &ferr);
private:
  inline void setup(ifstream &fin, ofstream &fout, ofstream &ferr);
  inline bool check_input();
  inline void launch();
  inline void teardown();

public:
  void show_definition();
private:
  inline void print_text(string text, bool newline=true);
  inline void print_section(string title);
  inline void print_configuration();
  inline void print_tape(bool output=false);
private:
  string format_setcion(string title, size_t width);
  string format_configuration();
  string format_tape();
private:
  inline void sb_reset() { sb.str(""); sb.clear(); }
};

#endif  // TURINGMACHINE_H