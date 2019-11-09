#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

using namespace std;

class TMException {
protected:
  string msg;
public:
  TMException(string msg);
  virtual string what();
};

class SyntaxError: public TMException {
  string location;  // "lineno:pos"
public:
  SyntaxError(string msg, string location);
  virtual string what();
};

class RuntimeError : public TMException {
public:
  RuntimeError(string msg);
  virtual string what();
};

#endif  // EXCEPTION_H