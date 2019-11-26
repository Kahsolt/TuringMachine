#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>
#include <set>
#include <map>
#include "config.h"
#include "Exception.h"
#include "Tokenizer.h"
#include "TuringMachine.h"

using namespace std;

class Parser {
private:
  Tokenizer tokenizer;
  char* line; int lineno;
  TuringMachine *tm;  // `for tm->strpool.intern()`
public:
  Parser();
  ~Parser();
  TuringMachine* parse(string fdeffp);
private:
  inline string format_position();  // when syntax error
private:
  void extract_number(size_t &i);
  void extract_symbol(Symbol &s);
  void extract_symbol_set(set<Symbol> &s);
  void extract_state(State* &s);
  void extract_state_set(set<State*> &s);
  void extract_transition(map<State*, map<Symbols*, Transition>> &m);
};

#endif  // PARSER_H