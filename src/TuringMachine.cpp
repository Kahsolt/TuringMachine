#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <set>
#include <map>
#include <deque>
#include <vector>
#include "utils.h"
#include "Exception.h"
#include "TuringMachine.h"

using namespace std;

TuringMachine::TuringMachine()
  : B(-1), S(new set<Symbol>()), T(new set<Symbol>()),
    Q(new set<State*>()), q0(nullptr), F(new set<State*>()),
    D(new map<State*, map<Symbol, Transition*>*>()),
    limit_time(-1), limit_space(-1), q(nullptr), steps(0), 
    tape(new deque<Symbol>()), origin(tape->begin()), head(tape->begin()) {
  if (LIMIT_ENABLE) {
    limit_time = LIMIT_TIME;
    limit_space = LIMIT_SPACE;
  }
}

TuringMachine::~TuringMachine() {
  if (S) { delete S; S = nullptr; }
  if (T) { delete T; T = nullptr; }
  if (F) { delete F; F = nullptr; }
  if (tape) { delete tape; tape = nullptr; }
  if (Q) {
    for (auto it : *Q)
      delete it;
    delete Q; Q = nullptr;
  }
  if (D) {
    for (auto it : *D) {
      auto DD = it.second;
      for (auto it : *DD)
        delete it.second;
      delete DD;
    }
    delete D; D = nullptr;
  }
}

void TuringMachine::run(ifstream &fin, ofstream &fout, ofstream &ferr) {
  setup(fin, fout, ferr);
  if (check_input()) launch();
  teardown();
}

inline void TuringMachine::setup(ifstream &fin, ofstream &fout, ofstream &ferr) {
  if (!fin.good()) throw new RuntimeError("cannot open input file");
  this->fin = &fin;
  if (!fout.good()) throw new RuntimeError("cannot open output file");
  this->fout = &fout;
  if (!ferr.good()) throw new RuntimeError("cannot open console file");
  this->ferr = &ferr;

  tape->clear();
  origin = head = tape->begin();
  steps = 0; q = q0;
}

inline bool TuringMachine::check_input() {
  char c;
  while (~(c = fin->get()))
    tape->push_back(c);

  bool ok = true;
  if (S->size())  // check input if #S given
    for (auto it : *tape)
      if (S->find(it) == S->end())
        { ok = false; c = it; break; }

  if (!tape->size()) tape->push_back(B);  // dummy
  print_text("Input: ", false); print_tape();
  if (!ok) {
    print_section("ERR");
    print_text("The input " + quote(format_tape()) + " is illegal");
  }

  return ok;
}

inline void TuringMachine::launch() {
  print_section("RUN");

  while (F->find(q) == F->end()) {
    print_configuration(); steps++;
    if (limit_space > 0 && tape->size() > limit_space) throw new RuntimeError("space limit exceeded");
    if (limit_time > 0 && steps > limit_time) throw new RuntimeError("time limit exceeded");

    auto it1 = D->find(q);
    if (it1 == D->end() || !it1->second) throw new RuntimeError("no transition on state " + quote(*q));

    Transition* trans = nullptr;
    auto it2 = it1->second->find(*head);
    if (it2 != it1->second->end() || !it2->second) trans = it2->second;
    if (!trans) {
      auto it2 = it1->second->find(WILDCARD_MARK);
      if (it2 != it1->second->end() || !it2->second) trans = it2->second;
    }
    if (!trans) throw new RuntimeError("no transition on state " + quote(*q) + " of symbol " + quote(*head));
    
    q = trans->state_next;
    if (trans->symbol_new != WILDCARD_MARK) *head = trans->symbol_new;
    switch (trans->direction) {
      case RIGHT:
        if (head + 1 == tape->end())
          tape->push_back(B);
        head += 1;
        break;
      case STALL: break;
      case LEFT:
        if (head == tape->begin())
          tape->push_front(B);
        head -= 1;
        break;
      default: throw new RuntimeError("unknown direction " + trans->direction);
    }
  }

  print_configuration();
  print_text("Result: ", false); print_tape(true);
}

inline void TuringMachine::teardown() { print_section("END"); }

void TuringMachine::show_definition() {
  cout << format_setcion("DEFINITION", BANNER_WIDTH) << endl;

  cout << "TIME = " << (limit_time > 0 ? to_string(limit_time) : "INF") << endl;
  cout << "SPACE = " << (limit_space > 0 ? to_string(limit_space) : "INF") << endl;
  
  cout << "S = { ";
  for (auto it : *S) {
    if (it != *S->begin()) cout << ", ";
    cout << it;
  }
  cout << " }" << endl;

  cout << "T = { ";
  for (auto it : *T) {
    if (it != *T->begin()) cout << ", ";
    cout << it;
  }
  cout << " }" << endl;

  cout << "B = " << B << endl;

  cout << "Q = { ";
  for (auto &it : *Q) {
    if (it != *Q->begin()) cout << ", ";
    cout << *it;
  }
  cout << " }" << endl;

  cout << "q0 = " << *q0 << endl;

  cout << "F = { ";
  for (auto &it : *F) {
    if (it != *F->begin()) cout << ", ";
    cout << *it;
  }
  cout << " }" << endl;
  
  cout << "D = [" << endl;
  for (auto &it : *D) {
    cout << "  " << *it.first << ':' << endl;
    auto DD = it.second;
    for (auto &it : *DD) {
      cout << "    " << it.first << " -> " << it.second->symbol_new;
      cout << " | " << to_char(it.second->direction);
      cout << '/' << *it.second->state_next << endl;
    }
  }
  cout << ']' << endl;
  
  for (size_t i=1; i<=BANNER_WIDTH; i++) sb << BANNER_SEPERATOR;
}

inline void TuringMachine::print_text(string text, bool newline) {
  cout << text; if (newline) cout << endl;
  *ferr << text; if (newline) *ferr << endl;
}

inline void TuringMachine::print_section(string title) { print_text(format_setcion(title, BANNER_WIDTH)); }

inline void TuringMachine::print_configuration() { print_text(format_configuration()); }

inline void TuringMachine::print_tape(bool output) { string s = format_tape(); print_text(s); if (output) *fout << s << endl; }

string TuringMachine::format_setcion(string title, size_t width) {
  size_t len = title.length();
  size_t sym_cnt = (width - len - 2) >> 1;
  sb_reset();
  for (size_t i=0; i<sym_cnt; i++) sb << BANNER_SECTION;
  sb << ' ' << title << ' ';
  for (size_t i=0; i<sym_cnt; i++) sb << BANNER_SECTION;
  return sb.str();
}

string TuringMachine::format_configuration() {
  vector<int> pads; // cache paddings
  sb_reset();
  
  sb << "Step  : " << steps << endl;

  auto begpos = tape->begin();
  while (begpos != head && *begpos == B) begpos++;
  auto endpos = tape->end(); endpos--;
  while (endpos != head && *endpos == B) endpos--;
  endpos++; // offset adjust

  sb << "Index :";
  for (auto it=begpos; it!=endpos; ++it) {
    int idx = abs(it - origin);
    sb << ' ' << idx;
    int w = idx ? (1 + (int)floor(log10(idx))) : 1;
    pads.push_back(w);
  }
  sb << endl;

  sb << "Tape  :";
  for (auto it=begpos; it!=endpos; ++it)
    sb << ' ' << rpad(*it, pads[it - begpos]);
  sb << endl;

  sb << "Head  :";
  for (auto it=begpos; it!=endpos; ++it)
    sb << ' ' << rpad(it != head ? ' ' : '^', pads[it - begpos]);
  sb << endl;
  
  sb << "State : " << *q << endl;

  for (size_t i=1; i<=BANNER_WIDTH; i++) sb << BANNER_SEPERATOR;

  return sb.str();
}

string TuringMachine::format_tape() {
  auto s = tape->begin(); while (s != tape->end() && *s == B) s++; 
  auto e = tape->end() - 1; while (e != tape->begin() && *e == B) e--;
  sb_reset();
  while (s <= e) sb << *s++;
  return sb.str();
}