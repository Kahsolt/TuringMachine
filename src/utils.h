#ifndef UTILS_H
#define UTILS_H

// NOTICE: should all be inline functions

#include <string>

using namespace std;

inline string to_string(char c) { string s = string(" "); s[0] = c; return s;}
inline string quote(string s) { return "'" + s + "'"; }
inline string quote(char c) { return quote(to_string(c)); }
inline string rpad(string s, size_t width) {
  size_t lenpad = width - s.length();
  if (lenpad > 0) while (lenpad--) s += ' ';
  return s;
}
inline string rpad(char c, size_t width) { return rpad(to_string(c), width); }

#endif  // UTILS_H