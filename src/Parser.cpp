#include <iostream>
#include <fstream>
#include "utils.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "TuringMachine.h"
#include "Exception.h"

using namespace std;

Parser::Parser() : line(new char[(LINE_BUFFER_SIZE + 1) * sizeof(char)]), lineno(0), tm(nullptr) { }

Parser::~Parser() { delete[] line; }

TuringMachine* Parser::parse(ifstream &fdef) {
  tm = new TuringMachine();

  while (!fdef.eof()) {
    fdef.getline(line, LINE_BUFFER_SIZE);
    if (fdef.bad() || fdef.fail()) throw new TMException("cannot read TM definition file");
    if (fdef.gcount() == LINE_BUFFER_SIZE)
      throw new SyntaxError("line too long, please increase LINE_BUFFER_SIZE", format_position());
    lineno++;

    tokenizer.tokenize(line);
    Token tok = tokenizer.next(); if (!tok) continue;
#ifdef DEBUG_PARSER
    cout << tok << endl;
#endif
    if (tok.type == ITEM) {  // definition of B, S, T, Q, q0, F
      if (tok.value == "B") extract_symbol(tm->B);
      else if (tok.value == "S") extract_symbol_set(tm->S);
      else if (tok.value == "T") extract_symbol_set(tm->T);
      else if (tok.value == "q" || tok.value == "q0") extract_state(tm->q0);
      else if (tok.value == "Q") extract_state_set(tm->Q);
      else if (tok.value == "F") extract_state_set(tm->F);
      else if (tok.value == "TIME") extract_number(tm->limit_time);
      else if (tok.value == "SPACE") extract_number(tm->limit_space);
      else throw new SyntaxError("unknown definition item " + quote("#" + tok.value), format_position());
    } else if (tok.type == LITERAL) { // definition of D: one transition rule
      tokenizer.putback(tok); // unget!!
      extract_transition(tm->D);
    } else throw new SyntaxError("definition or transition item needed but got " + quote(to_string(tok)), format_position());
  }

  // defaults
  if (tm->B == -1) tm->B = '_';
  if (!tm->q0) tm->q0 = intern_state("init");
  if (!tm->F->size()) tm->F->insert(intern_state("halt"));

  return tm;
}

inline string Parser::format_position() { return to_string(lineno) + ":" + to_string(tokenizer.offset()); }

void Parser::extract_number(size_t &i) {
  Token tok = tokenizer.next();
  if (tok.type != EQUAL) throw new SyntaxError("missing '='", format_position());
  tok = tokenizer.next();
  if (tok.type != LITERAL) throw new SyntaxError("integer number required", format_position());
  i = atoi(tok.value.c_str());
  tok = tokenizer.next();
  if (!!tok) throw new SyntaxError("line definition closed, but got remaining token " + quote(to_string(tok)), format_position());
}

void Parser::extract_symbol(Symbol &s) {
  Token tok = tokenizer.next();
  if (tok.type != EQUAL) throw new SyntaxError("missing '='", format_position());
  tok = tokenizer.next();
  if (tok.value.length() != 1) throw new SyntaxError("single char needed, but got " + quote(tok.value), format_position());
  s = tok.value[0];
  tok = tokenizer.next();
  if (!!tok) throw new SyntaxError("line definition closed, but got remaining token " + quote(to_string(tok)), format_position());
}

void Parser::extract_symbol_set(set<Symbol>* &s) {
  Token tok = tokenizer.next();
  if (tok.type != EQUAL) throw new SyntaxError("missing '='", format_position());
  tok = tokenizer.next();
  if (tok.type != LBRACKET) throw new SyntaxError("missing '{'", format_position());
  tok = tokenizer.next();

  while (!!tok && tok.type != RBRACKET) {
    if (tok.value.length() != 1) throw new SyntaxError("single char needed, but got " + quote(tok.value), format_position());
    s->insert(tok.value[0]);
    tok = tokenizer.next();
    if (tok.type != RBRACKET) {
      if (tok.type == SEPERATOR) tok = tokenizer.next();
      else throw new SyntaxError("missing seperator " + quote(ITEM_SEPERATOR), format_position());
    }
  }
  if (tok.type != RBRACKET) throw new SyntaxError("missing '}'", format_position());
  tok = tokenizer.next();
  if (!!tok) throw new SyntaxError("line definition closed, but got remaining token " + quote(to_string(tok)), format_position());
}

void Parser::extract_state(State* &s) {
  Token tok = tokenizer.next();
  if (tok.type != EQUAL) throw new SyntaxError("missing '='", format_position());
  tok = tokenizer.next();
  s = intern_state(tok.value);
  tok = tokenizer.next();
  if (!!tok) throw new SyntaxError("line definition closed, but got remaining token " + quote(to_string(tok)), format_position());
}

void Parser::extract_state_set(set<State*>* &s) {
  Token tok = tokenizer.next();
  if (tok.type != EQUAL) throw new SyntaxError("missing '='", format_position());
  tok = tokenizer.next();
  if (tok.type != LBRACKET) throw new SyntaxError("missing '{'", format_position());
  tok = tokenizer.next();

  while (!!tok && tok.type != RBRACKET) {
    s->insert(intern_state(tok.value));
    tok = tokenizer.next();
    if (tok.type != RBRACKET) {
      if (tok.type == SEPERATOR) tok = tokenizer.next();
      else throw new SyntaxError("missing seperator " + quote(ITEM_SEPERATOR), format_position());
    }
  }
  if (tok.type != RBRACKET) throw new SyntaxError("missing '}'", format_position());
  tok = tokenizer.next();
  if (!!tok) throw new SyntaxError("line definition closed, but got remaining token " + quote(to_string(tok)), format_position());
}

void Parser::extract_transition(map<State*, map<Symbol, Transition*>*>* &m) {
  Token stat_cr_t = tokenizer.next();
  if (!stat_cr_t || stat_cr_t.type != LITERAL) throw new SyntaxError("bad definition for 'current_state' in " + quote(string(line)), format_position());
  Token sym_cr_t = tokenizer.next();
  if (!sym_cr_t || sym_cr_t.value.length() != 1) throw new SyntaxError("bad definition for 'current_symbol' in " + quote(string(line)), format_position());
  Token sym_nx_t = tokenizer.next();
  if (!sym_nx_t || sym_nx_t.value.length() != 1) throw new SyntaxError("bad definition for 'next_symbol' in " + quote(string(line)), format_position());
  Token dir_t = tokenizer.next();
  if (!dir_t || dir_t.type != LITERAL) throw new SyntaxError("bad definition for 'direction' in " + quote(string(line)), format_position());
  Token stat_nx_t = tokenizer.next();
  if (!stat_nx_t || stat_nx_t.type != LITERAL) throw new SyntaxError("bad definition for 'next_state' in " + quote(string(line)), format_position());
  Token tok = tokenizer.next();
  if (!!tok) throw new SyntaxError("line definition closed, but got remaining token " + quote(to_string(tok)), format_position());

  State *stat_cr = intern_state(stat_cr_t.value), *stat_nx = intern_state(stat_nx_t.value);
  Symbol sym_cr = sym_cr_t.value[0], sym_nx = sym_nx_t.value[0];
  Direction dir; switch (dir_t.value[0]) {
    case 'l': dir = LEFT; break;
    case 'r': dir = RIGHT; break;
    case '*': dir = STALL; break;
    default: throw new SyntaxError("unknown direction " + quote(dir_t.value[0]), format_position());
  }
  
  if (!m->count(stat_cr)) m->insert(make_pair<>(stat_cr, new map<Symbol, Transition*>()));
  auto mm = m->find(stat_cr);
  if (mm != m->end() && mm->second->count(sym_cr))
    throw new SyntaxError("redefinition found for transfer (" + *stat_cr + ", " + sym_cr + ") -> _", format_position());
  mm->second->insert(make_pair<>(sym_cr, new Transition(sym_nx, stat_nx, dir)));
}

State* Parser::intern_state(State state) {
  State* that = nullptr;
  for (auto it: *tm->Q)
    if (*it == state)
      { that = it; break; }
  if (!that) {
    that = new string(state);
    tm->Q->insert(that);
  }
  return that;
}