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

  void add(Token);

public:
  Lexer(std::string);
  ~Lexer();

  Token next();
  Token peek(int=0);
  bool  isNext(Token::Type);
  Token match(Token::Type, std::string="");

  void  tokenize();
};

#endif
