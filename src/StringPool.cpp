#include "StringPool.h"

StringPool::~StringPool() {
  for (auto it : pool) { delete it; } pool.clear();
}

string* StringPool::intern(string s) {
  string* that = nullptr;
  for (auto it: pool)
    if (*it == s)   // string compare
      { that = it; break; }
  if (!that) {
    that = new string(s);
    pool.insert(that);
  }
  return that;
}

string* StringPool::intern(string *s) {
  string* that = nullptr;
  for (auto it: pool)
    if (*it == *s)   // string compare
      { that = it; break; }
  if (!that) {
    that = new string(*s);
    pool.insert(that);
  }
  return that;
}

string* StringPool::find(string *s) {
  for (auto it: pool) if (*it == *s) return it;
  return nullptr;
}

string* StringPool::find(string s) {
  for (auto it: pool) if (*it == s) return it;
  return nullptr;
}
