#ifndef PROTY_PARSER_HH
#define PROTY_PARSER_HH

#include "lexer.hh"
#include "ast.hh"

class Parser {
private:
  Lexer* lexer;

public:
  Parser(Lexer* lexer);

  Node* parse();
  Node* parseBlock();
  Node* parseExpression();
  Node* parseOperation(Node*, int);
  Node* parsePrimary();
  Node* parseCall(Node*);
  Node* parseIf();
  Node* parseWhile();
  Node* parseTry();
};

#endif
