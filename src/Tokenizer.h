#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <sstream>
#include "config.h"

using namespace std;

typedef enum {
  EMPTY,
  ITEM, LITERAL, 
  EQUAL, LBRACKET, RBRACKET, SEPERATOR,
} TokenType;

inline string to_string(TokenType type) {
  switch (type) {
    case ITEM: return "ITEM";
    case LITERAL: return "VALUE";
    case EQUAL: return "EQU";
    case LBRACKET: return "LBR";
    case RBRACKET: return "RBR";
    case SEPERATOR: return "SEP";
    case EMPTY: return "NUL";
    default: return "_BAD_";
  }
}

struct Token {
  static Token EMPTY_TOKEN;
  TokenType type;
  string value;
  Token() : type(EMPTY), value("") { }
  Token(TokenType type, string value) : type(type), value(value) { }
  bool operator!() { return type == EMPTY; }
  friend ostream& operator<<(ostream &out, Token &token) { out << '<' << to_string(token.type) << ": " << token.value << '>'; return out; }
};

inline string to_string(Token &token) {
  return "<" + to_string(token.type) + ": " + token.value + ">";
}

class Tokenizer {
private:
  char *line, *p;
  bool use_last_token; Token last_token;
  stringstream sb;
public:
  Tokenizer();
  void tokenize(char* line);
  Token next();
  inline size_t offset() { return p - line + 1; }
  inline void putback(Token token) { use_last_token = true; last_token = token; }
private:
  inline string extract_literal();
private:
  inline char cur() { return *p; }
  inline void nxt() { p++; }
  inline void skip_nonsense() { while (isspace(*p)) p++; }
  inline bool isnull() { return !p; }
  inline bool iseol() { return *p == '\n' || *p == '\r' || *p == '\0'; }
  inline bool isliteral() { return *p != ' ' && isprint(*p) && !isreserved(); }
  inline bool isidentifier() { return isalnum(*p) || *p == '_'; }
  /* reserved structural symbols */
  inline bool isreserved() { return ishash() || isequal() || islbracket() || isrbracket() || issep() || iscmt(); }
  inline bool ishash() { return *p == '#'; }
  inline bool isequal() { return *p == '='; }
  inline bool islbracket() { return *p == '{'; }
  inline bool isrbracket() { return *p == '}'; }
  inline bool issep() { return *p == ITEM_SEPERATOR; }
  inline bool iscmt() { return *p == COMMENT_MARK; }
};

#endif  // TOKENIZER_H