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
  int pos;

public:
  Lexer(std::string);

  void  add(Token);
  Token next();
  Token peek(int=0);
  bool  isNext(Token::Type);
  Token match(Token::Type, std::string="");
  void  tokenize();
};

#endif
