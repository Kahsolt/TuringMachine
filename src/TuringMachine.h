#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include "config.h"
#include "Parser.h"
#include "StringPool.h"

using namespace std;

typedef enum { BRIEF, SIMPLE, VERBOSE } LogLevel;

typedef char Symbol;
typedef string State;
typedef string Symbols;
typedef string Directions;
typedef enum { STALL = 0, LEFT = -1, RIGHT = 1 } Direction;

inline char to_char(Direction dir) {
  switch (dir) {
    case RIGHT: return 'R';
    case LEFT: return 'L';
    case STALL: return 'S';
    default: throw new RuntimeError("unknown direction during runtime");
  }
}

inline Direction to_enum(Symbol sym_dir) {
  switch (sym_dir) {
    case 'r': return RIGHT;
    case 'l': return LEFT;
    case '*': return STALL;
    default: throw new RuntimeError("unknown direction during runtime");
  }
}

struct Transition { // inner struct of TuringMachine
  Symbols *symbols_new;
  Directions *directions;
  State *state_next;
  Transition(Symbols *symbols_new, Directions *directions, State *state_next)
    : symbols_new(symbols_new), directions(directions), state_next(state_next) { }
  inline string to_string() { return *symbols_new + " " + *directions + "/" + *state_next; }
};

struct Tape : public deque<Symbol> {  // inner struct of TuringMachine
  deque<Symbol>::iterator origin;    // origin position on tape, aka. 'index-0'
  deque<Symbol>::iterator head;
  Tape() : origin(this->begin()), head(this->begin()) { }
  inline void reset() { this->clear(); origin = head = this->begin(); }
};

class TuringMachine {
  friend class Parser;
private:
  /* definition */
  size_t N;             // number of tapes
  set<Symbol> T;        // symbol set
  set<Symbol> S;        // input symbol set
  Symbol B;             // blank symbol
  set<State*> Q;        // state set
  State *q0;            // start state
  set<State*> F;        // halt state set 
  map<State*, map<Symbols*, Transition>> D;  // transfer function
  /* device */
  ifstream fin;         // I/O devices
  ofstream fout;
  ofstream ferr;
  size_t limit_time;    // resource limit
  size_t limit_space;
  /* runtime */
  size_t steps;         // step counter, init: 0
  State *q;             // current state, init: q0
  vector<Tape> tapes;   // the infinite tapes, tapes.size() == N
  /* opts */
  LogLevel level;
private:
  stringstream sb;
  StringPool strpool;   // optimize: reuse const string objects

private:
  TuringMachine();  // cannot publically create this object, only via Parser::parse
public:
  void opts(LogLevel level) { this->level = level; }
  void run(string finfp, string foutfp, string ferrfp);
private:
  inline void setup(string finfp, string foutfp, string ferrfp);
  inline bool check_input();
  inline void launch();
  inline void teardown();

private:
  void print_text(string text, bool newline=true, bool output=false);
  inline void print_definition() { print_text(format_definition()); }
  inline void print_section(string title) { print_text(format_setcion(title, BANNER_WIDTH)); }
  inline void print_configuration() { print_text(format_configuration()); }
  inline void print_tape0() { print_text(format_tape0(), true, true); }
private:
  string format_definition();
  string format_setcion(string title, size_t width);
  string format_configuration();
  string format_tape0();
private:
  inline void sb_reset() { sb.str(""); sb.clear(); }
};

#endif  // TURINGMACHINE_H