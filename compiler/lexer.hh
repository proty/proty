#ifndef PROTY_LEXER_HH
#define PROTY_LEXER_HH

#include <iostream>
#include <vector>
#include "token.hh"

class Lexer {
private:
  std::vector<Token>* tokens;
  std::istream* stream;
  int pos;

  std::string filename;
  int lineno;

  void add(Token);

public:
  Lexer(std::istream*, std::string);
  ~Lexer();

  Token next();
  Token peek(int=0);
  bool isNext(Token::Type);
  Token match(Token::Type, std::string="");
  void unexpected(Token, std::string);

  void tokenize();

  const std::string getFilename() { return filename; }
};

#endif
