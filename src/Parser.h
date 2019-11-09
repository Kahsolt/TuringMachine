#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <set>
#include <map>
#include "config.h"
#include "Tokenizer.h"
#include "TuringMachine.h"

using namespace std;

class Parser {
private:
  Tokenizer tokenizer;
  char* line; int lineno;
  TuringMachine* tm;  // for .intern_state()
public:
  Parser();
  ~Parser();
  TuringMachine* parse(ifstream &fdef);
private:
  inline string format_position();
private:
  void extract_number(size_t &i);
  void extract_symbol(Symbol &s);
  void extract_symbol_set(set<Symbol>* &s);
  void extract_state(State* &s);
  void extract_state_set(set<State*>* &s);
  void extract_transition(map<State*, map<Symbol, Transition*>*>* &m);
  State* intern_state(State state);  // for object reuse and optimize
};

#endif  // PARSER_H