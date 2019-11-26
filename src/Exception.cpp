#include "Exception.h"

TMException::TMException(string msg) : msg(msg) { }
string TMException::what() { return "[TMException] " + msg; }

DefinitionError::DefinitionError(string msg, string location) : TMException(msg), location(location) { }
string DefinitionError::what() { return "[DefinitionError] " + location + " " + msg; }

SyntaxError::SyntaxError(string msg, string location) : DefinitionError(msg, location) { }
string SyntaxError::what() { return "[SyntaxError] " + location + " " + msg; }

RuntimeError::RuntimeError(string msg) : TMException(msg) { }
string RuntimeError::what() { return "[RuntimeError] " + msg; }