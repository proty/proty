#ifndef PROTY_LEXER_HH
#define PROTY_LEXER_HH

#include <iostream>
#include <vector>
#include "token.hh"

class Lexer {
private:
  std::string filename;
  std::vector<Token>* tokens;
  std::istream* stream;

public:
  Lexer(std::string);

  void add(Token);
  Token next();
  Token peek(int=0);
  void tokenize();
};

#endif