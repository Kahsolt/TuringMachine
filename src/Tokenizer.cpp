#include <iostream>
#include "utils.h"
#include "Exception.h"
#include "Tokenizer.h"

Token Token::EMPTY_TOKEN = Token();

Tokenizer::Tokenizer() : line(nullptr), p(nullptr), use_last_token(false), last_token(Token::EMPTY_TOKEN) { }

void Tokenizer::tokenize(char* line) { this->line = p = line; }

Token Tokenizer::next() {
  if (isnull()) return Token::EMPTY_TOKEN;
  if (use_last_token) { use_last_token = false; return last_token; }
  skip_nonsense(); if (iseol()) return Token::EMPTY_TOKEN;
  
  if (iscmt())           { while (!iseol()) nxt(); return Token::EMPTY_TOKEN; }
  else if (ishash())     { nxt(); return Token(ITEM, extract_literal()); }
  else if (isliteral())  {        return Token(LITERAL, extract_literal()); }
  else if (issep())      { nxt(); return Token(SEPERATOR, to_string(ITEM_SEPERATOR)); }
  else if (isequal())    { nxt(); return Token(EQUAL, to_string('=')); }
  else if (islbracket()) { nxt(); return Token(LBRACKET, to_string('{')); }
  else if (isrbracket()) { nxt(); return Token(RBRACKET, to_string('}')); }
  throw new SyntaxError("illegal char " + quote(cur()), "?:" + to_string(offset()));
}

inline string Tokenizer::extract_literal() {
  sb.str(""); sb.clear();
  while (isliteral()) { sb << cur(); nxt(); }
  return sb.str();
}