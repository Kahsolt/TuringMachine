#include "Exception.h"

TMException::TMException(string msg) : msg(msg) { }
string TMException::what() { return "[TMException] " + msg; }

SyntaxError::SyntaxError(string msg, string location) : TMException(msg), location(location) { }
string SyntaxError::what() { return "[SyntaxError] " + location + " " + msg; }

RuntimeError::RuntimeError(string msg) : TMException(msg) { }
string RuntimeError::what() { return "[RuntimeError] " + msg; }