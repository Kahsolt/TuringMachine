#include <iostream>
#include <fstream>
#include <cmath>
#include "utils.h"
#include "Exception.h"
#include "Parser.h"   // replicated cycle include
#include "TuringMachine.h"

TuringMachine::TuringMachine()
    : N(0), B(-1), q0(nullptr), limit_time(-1), limit_space(-1),
      steps(0), q(nullptr), level(SIMPLE) {
  if (LIMIT_ENABLE) {
    limit_time = LIMIT_TIME;
    limit_space = LIMIT_SPACE;
  }
}

void TuringMachine::run(string finfp, string foutfp, string ferrfp) {
  if (level == VERBOSE) print_definition();
  setup(finfp, foutfp, ferrfp);
  if (check_input()) launch();
  teardown();
}

inline void TuringMachine::setup(string finfp, string foutfp, string ferrfp) {
  fin.clear(); fin.open(finfp);
  if (!fin) { fin.close(); throw new RuntimeError("cannot open input file"); }
  fout.clear(); fout.open(foutfp);
  if (!fout) { fout.close(); throw new RuntimeError("cannot open output file"); }
  ferr.clear(); ferr.open(ferrfp);
  if (!ferr) { ferr.close(); throw new RuntimeError("cannot open console file"); }

  steps = 0; q = q0;
  for (auto &tape : tapes) tape.reset();
}

inline bool TuringMachine::check_input() {
  auto &tape = tapes[0];
  char c; while (~(c = fin.get()))
    tape.push_back(c);

  bool ok = true;
  if (S.size())  // check input if #S given
    for (auto it : tape)
      if (S.find(it) == S.end())
        { ok = false; c = it; break; }

  for (auto &tape : tapes) if (!tape.size()) tape.push_back(B);  // dummy
  print_text("Input: ", false); print_tape0();
  if (!ok) {
    print_section("ERR");
    print_text("The input " + quote(format_tape0()) + " is illegal");
  }
  return ok;
}

inline void TuringMachine::launch() {
  print_section("RUN");

  while (F.find(q) == F.end()) {
    if (level != BRIEF) { print_configuration(); } steps++;
    for (auto &tape : tapes) if (limit_space > 0 && tape.size() > limit_space) throw new RuntimeError("space limit exceeded");
    if (limit_time > 0 && steps > limit_time) throw new RuntimeError("time limit exceeded");

    // find tx for state
    auto sst = D.find(q);
    if (sst == D.end()) throw new RuntimeError("no transitions on state " + quote(*q));

    // find tx for (state, symbols)
    sb_reset();
    for (auto &tape : tapes) sb << *tape.head;
    string cur_syms = sb.str();
    int len = cur_syms.length();

    Transition* tx = nullptr; int minwc = len + 1; // use wildcards as less as possible
    for (auto &st : sst->second) {
      Symbols syms = *st.first;
      int wc = 0;
      for (int i=0; wc!=-1 && i<=len; i++)
        if (cur_syms[i] != syms[i]) {
          if (syms[i] == WILDCARD_MARK) wc++; // allow wildcards
          else wc = -1;                       // fail if mismatch
        }
      if (wc != -1 && wc <= minwc) {
        tx = &st.second; minwc = wc;
      }
    }
    if (!tx) throw new RuntimeError("no transition on state " + quote(*q) + " of symbol(s) " + quote(cur_syms));

    // apply this tx
    for (size_t i=0; i<tapes.size(); i++) {
      auto &tape = tapes[i];
      if ((*tx->symbols_new)[i] != WILDCARD_MARK) *tape.head = (*tx->symbols_new)[i];
      switch (to_enum((*tx->directions)[i])) {
        case RIGHT:
          if (tape.head + 1 == tape.end())
            tape.push_back(B);
          tape.head += 1;
          break;
        case LEFT:
          if (tape.head == tape.begin())
            tape.push_front(B);
          tape.head -= 1;
          break;
        case STALL: break;
      }
    }
    q = tx->state_next; // state shift
  }

  if (level != BRIEF) print_configuration();  // the last step
  print_text("Result: ", false); print_tape0();
}

inline void TuringMachine::teardown() {
  print_section("END");
  fin.close(); fout.close(); ferr.close();
}

inline void TuringMachine::print_text(string text, bool newline, bool output) {
  cout << text; if (newline) cout << endl;
  ferr << text; if (newline) ferr << endl;
  if (output) { fout << text; } if (newline) fout << endl; 
}

string TuringMachine::format_definition() {
  bool isfirst;

  string tmp = format_setcion("DEFINITION", BANNER_WIDTH);
  sb_reset();
  sb << tmp << endl;  // avoid reuse collision

  sb << "TIME = " << (limit_time > 0 ? to_string(limit_time) : "INF") << endl;
  sb << "SPACE = " << (limit_space > 0 ? to_string(limit_space) : "INF") << endl;
  
  sb << "N = " << N << endl;
  
  sb << "S = { ";
  isfirst = true;
  for (auto it : S) {
    if (!isfirst) sb << ", ";
    else isfirst = false;
    sb << it;
  }
  sb << " }" << endl;

  sb << "T = { ";
  isfirst = true;
  for (auto it : T) {
    if (!isfirst) sb << ", ";
    else isfirst = false;
    sb << it;
  }
  sb << " }" << endl;

  sb << "B = " << B << endl;

  sb << "Q = { ";
  isfirst = true;
  for (auto it : Q) {
    if (!isfirst) sb << ", ";
    else isfirst = false;
    sb << *it;
  }
  sb << " }" << endl;

  sb << "q0 = " << *q0 << endl;

  sb << "F = { ";
  isfirst = true;
  for (auto it : F) {
    if (!isfirst) sb << ", ";
    else isfirst = false;
    sb << *it;
  }
  sb << " }" << endl;
  
  sb << "D = [" << endl;
  for (auto &sst : D) {
    sb << "  " << *sst.first << ':' << endl;
    auto st = sst.second;
    for (auto &tx : st) {
      sb << "    " << *tx.first << " -> " << *tx.second.symbols_new;
      sb << " | ";
      for (size_t i=0; i<tx.second.directions->length(); i++)
        sb << to_char(to_enum((*tx.second.directions)[i]));
      sb << '/' << *tx.second.state_next << endl;
    }
  }
  sb << ']' << endl;
  
  for (size_t i=1; i<=BANNER_WIDTH; i++) sb << BANNER_SEPERATOR;

  return sb.str();
}

string TuringMachine::format_setcion(string title, size_t width) {
  size_t len = title.length();
  size_t sym_cnt = (width - len - 2) >> 1;
  sb_reset();
  for (size_t i=0; i<sym_cnt; i++) sb << BANNER_SECTOR;
  sb << ' ' << title << ' ';
  for (size_t i=0; i<sym_cnt; i++) sb << BANNER_SECTOR;
  if (len & 0x01) sb << BANNER_SECTOR;
  return sb.str();
}

string TuringMachine::format_configuration() {
  vector<int> pads; // cache paddings
  sb_reset();
  
  sb << "Step   : " << steps << endl;

  for (size_t i=0; i<tapes.size(); i++) {
    auto &tape = tapes[i];

    // boundary non-empty cells
    auto begpos = tape.begin();
    while (begpos != tape.head && *begpos == B) begpos++;
    auto endpos = tape.end(); endpos--;
    while (endpos != tape.head && *endpos == B) { endpos--; } endpos++; // offset adjust

    sb << "Index" << i << " :";
    for (auto it=begpos; it!=endpos; ++it) {
      int idx = abs(it - tape.origin);
      sb << ' ' << idx;
      int w = idx ? (1 + (int)floor(log10(idx))) : 1;
      pads.push_back(w);
    }
    sb << endl;

    sb << "Tape" << i << "  :";
    for (auto it=begpos; it!=endpos; ++it)
      sb << ' ' << rpad(*it, pads[it - begpos]);
    sb << endl;

    sb << "Head" << i << "  :";
    for (auto it=begpos; it!=endpos; ++it)
      sb << ' ' << rpad(it != tape.head ? ' ' : '^', pads[it - begpos]);
    sb << endl;
  }
  
  sb << "State  : " << *q << endl;

  for (size_t i=1; i<=BANNER_WIDTH; i++) sb << BANNER_SEPERATOR;
  return sb.str();
}

string TuringMachine::format_tape0() {
  auto &tape = tapes[0];
  auto s = tape.begin(); while (s != tape.end() && *s == B) s++; 
  auto e = tape.end() - 1; while (e != tape.begin() && *e == B) e--;
  sb_reset();
  while (s <= e) sb << *s++;
  return sb.str();
}