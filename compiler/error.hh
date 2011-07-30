#ifndef PROTY_ERROR_HH
#define PROTY_ERROR_HH

#include <iostream>

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
  std::string filename;
  int lineno;
  std::string message;

public:
  ParseError(std::string filename, int lineno, std::string msg)
    : filename(filename), lineno(lineno), message(msg) {}

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
