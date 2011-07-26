#ifndef PROTY_ERROR_HH
#define PROTY_ERROR_HH

#include <iostream>
#include "lexer.hh"

class ParseError {
private:
  Location loc;
  std::string message;

public:
  ParseError(Location loc, std::string msg)
    : loc(loc), message(msg) {}

  void printMessage();
};

#endif
