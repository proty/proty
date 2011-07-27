#ifndef PROTY_ERROR_HH
#define PROTY_ERROR_HH

#include <iostream>
#include "lexer.hh"

class Error {
public:
  virtual void printMessage() = 0;
};

class GeneralError : public Error {
private:
  std::string message;

public:
  GeneralError(std::string msg)
    : message(msg) {}

  void printMessage();
};

class ParseError : public Error {
private:
  Location loc;
  std::string message;

public:
  ParseError(Location loc, std::string msg)
    : loc(loc), message(msg) {}

  void printMessage();
};

class CompileError : public Error {
private:
  std::string message;

public:
  CompileError(std::string msg)
    : message(msg) {}

  void printMessage();
};

#endif
