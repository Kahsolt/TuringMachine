#include <iostream>
#include "utils.h"
#include "Parser.h"

Parser::Parser() : line(new char[LINE_BUFFER_SIZE * sizeof(char)]), lineno(0), tm(nullptr) { }

Parser::~Parser() { delete[] line; }

TuringMachine* Parser::parse(string fdeffp) {
  ifstream fdef(fdeffp);
  if (!fdef) { fdef.close(); throw new TMException("cannot read TM definition file"); }

  tm = new TuringMachine();
  while (!fdef.eof()) {
    fdef.getline(line, LINE_BUFFER_SIZE);
    if (fdef.gcount() == LINE_BUFFER_SIZE)
      throw new SyntaxError("line too long, please increase LINE_BUFFER_SIZE", format_position());
    lineno++;

    tokenizer.tokenize(line);
    Token tok = tokenizer.next(); if (!tok) continue;
#ifdef DEBUG_PARSER
    cout << tok << endl;
#endif
    if (tok.type == ITEM) {  // definition of B, S, T/G, Q, q/q0, F
      if (tok.value == "B") extract_symbol(tm->B);
      else if (tok.value == "S") extract_symbol_set(tm->S);
      else if (tok.value == "T" || tok.value == "G") extract_symbol_set(tm->T);
      else if (tok.value == "q" || tok.value == "q0") extract_state(tm->q0);
      else if (tok.value == "Q") extract_state_set(tm->Q);
      else if (tok.value == "F") extract_state_set(tm->F);
      else if (tok.value == "N") extract_number(tm->N);
      else if (tok.value == "TIME") extract_number(tm->limit_time);
      else if (tok.value == "SPACE") extract_number(tm->limit_space);
      else throw new DefinitionError("unknown definition item " + quote("#" + tok.value), format_position());
    } else if (tok.type == LITERAL) { // definition of D: one transition rule
      tokenizer.putback(tok); // unget!!
      extract_transition(tm->D);
    } else throw new SyntaxError("definition or transition item needed but got " + quote(to_string(tok)), format_position());
  }
  fdef.close();

  // definition defaults
  if (tm->B == -1) tm->B = TM_DEFAULT_B;
  if (!tm->q0) tm->q0 = tm->strpool.intern(TM_DEFAULT_q);
  if (!tm->F.size()) tm->F.insert(tm->strpool.intern(TM_DEFAULT_F));
  // N should be given explictly or infered from transition function rules
  for (size_t i=0; i<tm->N; i++) tm->tapes.push_back(Tape());
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

void Parser::extract_symbol_set(set<Symbol> &s) {
  Token tok = tokenizer.next();
  if (tok.type != EQUAL) throw new SyntaxError("missing '='", format_position());
  tok = tokenizer.next();
  if (tok.type != LBRACKET) throw new SyntaxError("missing '{'", format_position());
  tok = tokenizer.next();

  while (!!tok && tok.type != RBRACKET) {
    if (tok.value.length() != 1) throw new SyntaxError("single char needed, but got " + quote(tok.value), format_position());
    s.insert(tok.value[0]);
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
  s = tm->strpool.intern(tok.value);
  tok = tokenizer.next();
  if (!!tok) throw new SyntaxError("line definition closed, but got remaining token " + quote(to_string(tok)), format_position());
}

void Parser::extract_state_set(set<State*> &s) {
  Token tok = tokenizer.next();
  if (tok.type != EQUAL) throw new SyntaxError("missing '='", format_position());
  tok = tokenizer.next();
  if (tok.type != LBRACKET) throw new SyntaxError("missing '{'", format_position());
  tok = tokenizer.next();

  while (!!tok && tok.type != RBRACKET) {
    s.insert(tm->strpool.intern(tok.value));
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

void Parser::extract_transition(map<State*, map<Symbols*, Transition>> &m) {
  Token stat_cr_t = tokenizer.next();
  if (!stat_cr_t || stat_cr_t.type != LITERAL) throw new SyntaxError("bad definition for 'current_state' in " + quote(string(line)), format_position());
  State *stat_cr = tm->strpool.intern(stat_cr_t.value);
  tm->Q.insert(stat_cr);

  Token sym_cr_t = tokenizer.next();
  if (!sym_cr_t || stat_cr_t.type != LITERAL) throw new SyntaxError("bad definition for 'current_symbol' in " + quote(string(line)), format_position());
  Token sym_nx_t = tokenizer.next();
  if (!sym_nx_t || stat_cr_t.type != LITERAL) throw new SyntaxError("bad definition for 'next_symbol' in " + quote(string(line)), format_position());
  Symbols *sym_cr = tm->strpool.intern(sym_cr_t.value),
          *sym_nx = tm->strpool.intern(sym_nx_t.value);
  for (auto s : *sym_cr) tm->T.insert(s);
  for (auto s : *sym_nx) tm->T.insert(s);
  size_t len = sym_cr->length();
  if (tm->N == 0) tm->N = len;
  if (tm->N != len) throw new DefinitionError("tape number seems to be inconsistent", format_position());
  len = sym_nx->length();
  if (tm->N != len) throw new DefinitionError("tape number seems to be inconsistent", format_position());

  Token dir_t = tokenizer.next();
  if (!dir_t || dir_t.type != LITERAL) throw new SyntaxError("bad definition for 'direction' in " + quote(string(line)), format_position());
  for (auto it : dir_t.value)
    if (it != 'l' && it != 'r' && it != '*')
      throw new DefinitionError("unknown direction " + quote(it), format_position());
  Directions* dirs = tm->strpool.intern(dir_t.value);
  len = dirs->length();
  if (tm->N != len) throw new DefinitionError("tape number seems to be inconsistent", format_position());

  Token stat_nx_t = tokenizer.next();
  if (!stat_nx_t || stat_nx_t.type != LITERAL) throw new SyntaxError("bad definition for 'next_state' in " + quote(string(line)), format_position());
  State *stat_nx = tm->strpool.intern(stat_nx_t.value);
  tm->Q.insert(stat_nx);

  Token tok = tokenizer.next();
  if (!!tok) throw new SyntaxError("line definition closed, but got remaining token " + quote(to_string(tok)), format_position());

  if (!m.count(stat_cr)) m.insert(make_pair<>(stat_cr, map<Symbols*, Transition>())); // branches on a new state
  auto pr = m.find(stat_cr);
  if (pr != m.end() && pr->second.count(sym_cr))
    throw new DefinitionError("redefinition found for transfer (" + *stat_cr + ", " + *sym_cr + ") -> _", format_position());
  pr->second.insert(make_pair<>(sym_cr, Transition(sym_nx, dirs, stat_nx)));
}