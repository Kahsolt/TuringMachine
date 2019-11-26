#ifndef STRINGPOOL_H
#define STRINGPOOL_H

#include <string>
#include <set>

using namespace std;

class StringPool {
private:
  set<string*> pool;
public:
  ~StringPool();
  string* intern(string s);
  string* intern(string *s);
  string* find(string s);
  string* find(string *s);
};

#endif  // STRINGPOOL_H